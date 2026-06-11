#ifndef TERM_UTILS_H
#define TERM_UTILS_H

typedef struct {
  unsigned int char_x;
  unsigned int char_y;
  unsigned int pixel_x; // not always reported by terminal
  unsigned int pixel_y; // not always reported by terminal
} WindowSize;

void move_cursor(int x, int y);
void clear_screen();
void disable_raw_mode();
void enable_raw_mode();
void non_blocking_input(); // dont wait for input
WindowSize get_window_size();

#endif
