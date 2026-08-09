#ifndef TSCREEN_H
#define TSCREEN_H

#define DISPLAY_W 100
#define DISPLAY_H 40
#define LOG_W 40
#define LOG_H 45
#define METER_W 100
#define METER_H 5


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
  TBuffer display;
  TBuffer log;
  TBuffer meter;
} TScreen;


TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h);
void set_cell(TBuffer *tbuffer, char c, int x, int y);
void display(TScreen *tscreen);
void free_screen(TScreen *tscreen);
void tscreen_print(TScreen *tscreen, char *str, int x, int y);


#endif
