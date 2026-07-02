#include "buffers.h"
#include "term_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void fill_buffer(TScreen *tscreen, char c, int bw, int bh) {
  
  for (int i = 0; i < bw * bh; i++) {
    char ca = c;

    if (i < bw || (i / bw) == bh - 1) {
      ca = '-';
    }
    
    if ((i % bw) == bw - 1 || (i % bw) == 0) {
      ca = '|';
    }
    
    set_cell(tscreen, ca, i % bw, i / bw);
  }
  
}


int main(int argsc, char **argsv) {

  //         setup
  // ------------------------------------------------

  enable_raw_mode();
  clear_screen();
  fflush(stdout);
  WindowSize win_size = get_window_size();
  
  int buff_w = 140;
  int buff_h = 40;
  TScreen tscreen = new_tscreen(win_size.char_x, win_size.char_y, buff_w, buff_h);

  
  //         run
  // ------------------------------------------------
 
  char c;
  char break_key = 'q';
  char update_key = 'l';

  fill_buffer(&tscreen, ' ', buff_w, buff_h);
  
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
	win_size = get_window_size();
	tscreen = new_tscreen(win_size.char_x, win_size.char_y, buff_w, buff_h);
	fill_buffer(&tscreen, ' ', buff_w, buff_h);
	set_cell(&tscreen, '+', buff_w / 2, buff_h /2);
	set_cell(&tscreen, '+', 0, 0);
	set_cell(&tscreen, '+', buff_w - 1, 0);
        set_cell(&tscreen, '+', 0, buff_h - 1);
	set_cell(&tscreen, '+', buff_w - 1, buff_h -1);

	display(&tscreen);
      }
    }
    
    usleep(16667);
  }
  
  
  //           end
  // -----------------------------------------------
  clear_screen();
  disable_raw_mode();
  
}
