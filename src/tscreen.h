#ifndef TSCREEN_H
#define TSCREEN_H

typedef struct {
  int   size; 
  int   width;
  int   height;
  char *buff;
} TBuffer;

typedef struct {
  int     win_w;
  int     win_h;
  TBuffer tbuffer;
  char   *display_buff;
} TScreen;


TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h);
void set_cell(TScreen *tscreen, char c, int x, int y);
void display(TScreen *tscreen);
void free_screen(TScreen *tscreen);
void tscreen_print(TScreen *tscreen, char *str, int x, int y);

#endif
