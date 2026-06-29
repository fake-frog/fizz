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
  
  int width = 120;
  int height = 40;
  TScreen tscreen = new_tscreen(win_size.char_x, win_size.char_y, width, height);
  
  // --------------------------

  for (int i = 0; i < width * height; i++) {
    char c = ' ';
    if ((i % width) == width - 1 || (i % width) == 0) {
      c = '|';
    }
    
    write_to_back_tbuffer(&tscreen.tbuffers, c, i % width, i / width);
  }

  char c;
  char break_key = 'q';
  char update_key = 'l';

  display_front_tbuffer(&tscreen);

  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, 1);
    if (bytes_read == 1) {
      if (c == break_key) {
        break;
      }
      
      // there is a bug here (double free or curuption)
      if (c == update_key) {
	clear_screen();
	write_to_back_tbuffer(&tscreen.tbuffers, '*', width / 2, height /2);
	write_to_back_tbuffer(&tscreen.tbuffers, '*', width -1, 0);
	display_front_tbuffer(&tscreen);
      }
    }
    usleep(16667);
  }
  
  //           end
  // -------------------------
  clear_screen();
  disable_raw_mode();
}
