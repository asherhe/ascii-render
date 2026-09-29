#include <curses.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#include <emscripten/html5.h>
#include <pdcsdl.h>
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

#ifdef __EMSCRIPTEN__
void sync_canvas_size() {
  double width, height;
  emscripten_get_element_css_size("#canvas", &width, &height);
  int canvas_width = static_cast<int>(width);
  int canvas_height = static_cast<int>(height);
  static int previous_width = 0;
  static int previous_height = 0;

  if (canvas_width != previous_width || canvas_height != previous_height) {
    emscripten_set_canvas_element_size("#canvas", canvas_width, canvas_height);
    previous_width = canvas_width;
    previous_height = canvas_height;
  }
}

void sync_curses_size() {
  int width, height;
  emscripten_get_canvas_element_size("#canvas", &width, &height);

  int rows = height / pdc_fheight;
  int cols = width / pdc_fwidth;
  rows = rows > 0 ? rows : 1;
  cols = cols > 0 ? cols : 1;
  int current_rows, current_cols;
  getmaxyx(stdscr, current_rows, current_cols);

  if (rows != current_rows || cols != current_cols) {
    resize_term(rows, cols);
  }
}
#endif

void init_curses() {
#ifdef __EMSCRIPTEN__
  sync_canvas_size();
#endif
#ifdef __EMSCRIPTEN__
  pdc_sdl_render_mode = PDC_SDL_RENDER_SOLID;
#endif
  // ncurses initialization
  initscr();
  cbreak();              // disable line buffering (read keys immediately)
  noecho();              // don't echo input keys to screen
  curs_set(0);           // hide terminal cursor
  nodelay(stdscr, TRUE); // non-blocking input (for smooth animation loop)
  keypad(stdscr, TRUE);  // ennable arrow/function key processing
#ifdef __EMSCRIPTEN__
  sync_curses_size();
#endif

  // enable colors if terminal supports it
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
  }
}

void render_frame(App &app) {
#ifdef __EMSCRIPTEN__
  sync_canvas_size();
  sync_curses_size();
#endif
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

  // display help text
  int centerX = max_cols / 2;
  std::string title = "";

// quitting is not an option in web
#ifndef __EMSCRIPTEN__
  title = " (Press 'q' to quit) ";
#endif

  if (app.paused)
    title = " (PAUSED) ";
  mvprintw(max_rows - 1, centerX - (title.length() / 2), "%s", title.c_str());

  if (app.paused) {
    touchwin(stdscr);
  }

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