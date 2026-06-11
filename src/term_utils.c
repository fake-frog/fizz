#include "term_utils.h"
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <termios.h>
#include <unistd.h>

/*
  ====================================

  This is a bunch of junk and flags,
  that a terminal understands to set
  it into the right mode. Also escape
  sequences to set do do things like
  move the cursor.

  ====================================
*/

struct termios orig_termios;

// Move cursor to row, col (1-indexed)
void move_cursor(int x, int y) {
  // negative number set both x and y to 0
  printf("\033[%d;%dH", y, x);
}

static void switch_to_back_buffer() { printf("\033[?1049h"); }
static void switch_to_main_buffer() { printf("\033[?1049l"); }
// Clear screen and move to top-left
void clear_screen() { printf("\033[2J\033[H"); }

void disable_raw_mode() {
  printf("\x1b[?25h"); // show cursor
  switch_to_main_buffer();
  tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void non_blocking_input() {
  // make the input nonblocking
  int flags = fcntl(STDIN_FILENO, F_GETFL);
  fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
}

void enable_raw_mode() {
  switch_to_back_buffer();
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(disable_raw_mode);
  printf("\x1b[?25l"); // hide cursor

  struct termios raw = orig_termios;
  raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
  raw.c_oflag &= ~(OPOST);
  raw.c_cflag |= (CS8);
  raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);

  // Non-blocking read with timeout
  raw.c_cc[VMIN] = 0;
  raw.c_cc[VTIME] = 1;

  tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);

  // full buffering (\n wont triger flush)
  setvbuf(stdout, NULL, _IOFBF, 4096);
}

WindowSize get_window_size() {
  struct winsize w;
  ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
  WindowSize ws = {w.ws_col, w.ws_row, w.ws_xpixel, w.ws_ypixel};
  return ws;
}
