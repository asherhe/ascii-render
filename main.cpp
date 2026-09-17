#include <ncurses.h>

#include <chrono>
#include <cmath>
#include <string>
#include <thread>
#include <vector>

#include "scene.h"
#include "vec3.h"

int main() {
  // ncurses initialization
  initscr();
  cbreak();               // disable line buffering (read keys immediately)
  noecho();               // don't echo input keys to screen
  curs_set(0);            // hide terminal cursor
  nodelay(stdscr, TRUE);  // non-blocking input (for smooth animation loop)
  keypad(stdscr, TRUE);   // ennable arrow/function key processing

  // enable colors if terminal supports it
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attron(COLOR_PAIR(1));
  }

  Scene scene{1.0, 0.6};

  bool running = true;
  double time = 0.0;

  // main Render Loop
  while (running) {
    // 'q' or ESC to exit
    int ch = getch();
    if (ch == 'q' || ch == 'Q' || ch == 27) {
      running = false;
    }

    // live terminal dimensions to calculate absolute center
    int maxRows, maxCols;
    getmaxyx(stdscr, maxRows, maxCols);
    int centerY = maxRows / 2;
    int centerX = maxCols / 2;

    // clear in-memory render buffer (prevents flickering vs clear())
    erase();

    // display help text
    std::string title = "(Press 'q' to Quit)";
    mvprintw(maxRows - 1, centerX - (title.length() / 2), "%s", title.c_str());

    // mvaddch(y0, x0, '#');

    // flush the off-screen buffer to terminal screen at once
    refresh();

    // cap framerate (~30 FPS)
    std::this_thread::sleep_for(std::chrono::milliseconds(33));
    time += 0.033;
  }

  // clean up NCurses environment before exiting
  endwin();

  return 0;
}