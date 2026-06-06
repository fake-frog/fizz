#include "term_utils.h"
#include <stdio.h>

int main(int argc, char **argv) {

  //         setup
  // --------------------------
  
  enable_raw_mode();
  clear_screen();
  switch_to_back_buffer();
  fflush(stdout);
  
  // --------------------------


  WindowSize ws = get_window_size();
  move_cursor(ws.char_x / 2, ws.char_y / 2);
  printf("Hello");
  fflush(stdout);
  sleep(1);

  char c;
  char break_key = 'q';

  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, 1);
    if (bytes_read == 1) {
      if (c == break_key) {
        break;
      }
    }
  }


  
  //           end
  // -------------------------
  clear_screen();
  disable_raw_mode();
}
