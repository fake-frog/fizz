#include "buffers.h"
#include "term_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argsc, char **argsv) {

  //         setup
  // --------------------------

  enable_raw_mode();
  clear_screen();
  fflush(stdout);

  WindowSize win_size = get_window_size();
  
  int width = 155;
  int height = 50;
  TScreen tscreen = new_tscreen(win_size.char_x, win_size.char_y, width, height);
  
  // --------------------------

  for (int i = 0; i < width * height; i++) {
    char c = '-';
    if ((i % width) == width - 1 || (i % width) == 0) {
      c = '|';
    }
    
    set_cell(&tscreen, c, i % width, i / width);
  }

  char c;
  char break_key = 'q';
  char update_key = 'l';

  display(&tscreen);

  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, 1);
    if (bytes_read == 1) {
      if (c == break_key) {
        break;
      }
      
      // there is a bug here (double free or curuption)
      if (c == update_key) {
	clear_screen();
	set_cell(&tscreen, '*', width / 2, height /2);
	set_cell(&tscreen, '*', width -1, 0);
	display(&tscreen);
      }
    }
    usleep(16667);
  }
  
  //           end
  // -------------------------
  clear_screen();
  disable_raw_mode();
}
