#include "tscreen.h"
#include "term_utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

volatile sig_atomic_t resize = 0;

void handle_winch(int sig) {
  resize = 1;  
}


void fill_buffer(TBuffer *tbuffer, char c, int bw, int bh) {

  for (int i = 0; i < bw * bh; i++) {
    char ca = c;

    int top_bottom   = i < bw || (i / bw) == bh - 1;
    int left_right   = (i % bw) == bw - 1 || (i % bw) == 0;
    int top_left     = i == 0;
    int top_right    = i == bw - 1;
    int bottom_left  = i == (bw * bh - bw);
    int bottom_right = i == (bw * bh - 1);
    
    if (top_bottom)   ca = '-';
    if (left_right)   ca = '|';
    if (top_left)     ca = ' ';
    if (top_right)    ca = ' ';
    if (bottom_left)  ca = ' ';
    if (bottom_right) ca = ' ';
    
    set_cell(tbuffer, ca, i % bw, i / bw);
  }
  
}


int main(int argsc, char **argsv) {

  //         setup
  // ------------------------------------------------

  enable_raw_mode();
  clear_screen();
  fflush(stdout);
  WindowSize win_size = get_window_size();
  
  int buff_w = 142;
  int buff_h = 47;
  TScreen tscreen = new_tscreen(win_size.char_x, win_size.char_y, buff_w, buff_h);

  
  //         run
  // ------------------------------------------------
 
  char c;
  char break_key = 'q';
  char update_key = 'l';

  fill_buffer(&tscreen.tbuffer, ' ', DISPLAY_W + LOG_W, LOG_H);
  fill_buffer(&tscreen.display, ' ', DISPLAY_W, DISPLAY_H);
  fill_buffer(&tscreen.log, ' ', LOG_W, LOG_H);
  fill_buffer(&tscreen.meter, ' ', METER_W, METER_H);
  
  display(&tscreen);
  
  signal(SIGWINCH, handle_winch);
  fflush(stdout);
 
  while (1) {
    int bytes_read = read(STDIN_FILENO, &c, 1);

    if (resize) {
      clear_screen();
      free_tscreen(&tscreen);
      win_size = get_window_size();
      tscreen = new_tscreen(win_size.char_x, win_size.char_y, buff_w, buff_h);
      fill_buffer(&tscreen.tbuffer, ' ', DISPLAY_W + LOG_W, LOG_H);
      fill_buffer(&tscreen.display, ' ', DISPLAY_W, DISPLAY_H);
      fill_buffer(&tscreen.log, ' ', LOG_W, LOG_H);
      fill_buffer(&tscreen.meter, ' ', METER_W, METER_H);
  
      display(&tscreen);
      resize = 0;
    }
    
    if (bytes_read == 1) {
      if (c == break_key) {
        break;
      }
      
      if (c == update_key) {
	
      }
    }
    
    usleep(16667);
  }
 
  
  //           end
  // -----------------------------------------------
  clear_screen();
  disable_raw_mode();
}
