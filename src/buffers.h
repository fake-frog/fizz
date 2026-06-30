#ifndef BUFFERS_H
#define BUFFERS_H


typedef struct {
  int   size; // size of buff |  NOTE: != to width * height when padded
  int   width;
  int   height;
  char *buff;
} TBuffer;

typedef struct {
  int win_w;
  int win_h;
  TBuffer tbuffer;
} TScreen;


void set_cell(TScreen *tscreen, char c, int x, int y);
void display(TScreen *tscreen);

TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h);
void free_screen(TScreen *tscreen);
char *pad_tbuffer(TScreen *tscreen);

#endif
