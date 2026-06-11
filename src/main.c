#include "buffers.h"
#include "term_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {

  //         setup
  // --------------------------

  enable_raw_mode();
  clear_screen();
  fflush(stdout);

  int width = 100;
  TBufferPair tbuffers = new_tbuffer_pair(width, 100);

  // --------------------------

  // WindowSize ws = get_window_size();
  //  move_cursor(ws.char_x / 2, ws.char_y / 2);
  //  fflush(stdout);

  for (int i = 0; i < 10000; i++) {
    char c = '*';
    if ((i % width) == width - 1 || (i % width) == 0) {
      c = '|';
    }
    write_to_back_tbuffer(&tbuffers, c, i % width, i / width);
  }

  write_to_back_tbuffer(&tbuffers, '0', width - 1, width);

  char c;
  char break_key = 'q';

  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, 1);
    if (bytes_read == 1) {
      if (c == break_key) {
        break;
      }
    }
    clear_screen();
    display_front_tbuffer(&tbuffers);
  }

  //           end
  // -------------------------
  clear_screen();
  disable_raw_mode();
}
