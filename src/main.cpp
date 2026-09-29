#include <curses.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <chrono>
#include <string>
#include <thread>

#include "render.h"
#include "scene.h"

struct App {
  Scene scene{1.0, 0.6};
  Camera camera;
  Renderer renderer{scene, camera};
  bool running = true;
  bool paused = false;
  double time = 0.0;
};

void init_curses() {
  // ncurses initialization
  initscr();
  cbreak();              // disable line buffering (read keys immediately)
  noecho();              // don't echo input keys to screen
  curs_set(0);           // hide terminal cursor
  nodelay(stdscr, TRUE); // non-blocking input (for smooth animation loop)
  keypad(stdscr, TRUE);  // ennable arrow/function key processing

  // enable colors if terminal supports it
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
  }
}

void render_frame(App &app) {
  int ch = getch();
  // space to pause
  if (ch == ' ') {
    app.paused = !app.paused;
  }
#ifndef __EMSCRIPTEN__
  // 'q' or ESC to exit
  if (ch == 'q' || ch == 'Q' || ch == 27) {
    app.running = false;
  }
#endif

  if (!app.running) {
    return;
  }

  // live terminal dimensions to calculate absolute center
  int max_rows, max_cols;
  getmaxyx(stdscr, max_rows, max_cols);
  app.renderer.resize(max_rows, max_cols);

  // clear in-memory render buffer (prevents flickering vs clear())
  erase();

  app.camera.update(app.time);

  for (int row = 0; row < max_rows; ++row) {
    for (int col = 0; col < max_cols; ++col) {
      char c = app.renderer.rendered_char(row, col);
      mvaddch(row, col, c);
    }
  }

#ifndef __EMSCRIPTEN__
  // display help text
  int centerX = max_cols / 2;
  std::string title = " (Press 'q' to quit) ";
  mvprintw(max_rows - 1, centerX - (title.length() / 2), "%s", title.c_str());
#endif

  // flush the off-screen buffer to terminal screen at once
  refresh();

  if (!app.paused) {
    app.time += 0.033;
  }
}

#ifdef __EMSCRIPTEN__
void render_frame_callback(void *arg) {
  App &app = *static_cast<App *>(arg);
  render_frame(app);
  if (!app.running) {
    endwin();
    emscripten_cancel_main_loop();
  }
}
#endif

int main() {
  init_curses();
  App app;

#ifdef __EMSCRIPTEN__
  emscripten_set_main_loop_arg(render_frame_callback, &app, 30, 1);
#else
  // main Render Loop
  while (app.running) {
    render_frame(app);
    if (app.running) {
      std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }
  }

  // clean up NCurses environment before exiting
  endwin();
#endif

  return 0;
}