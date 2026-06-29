#ifndef BUFFERS_H
#define BUFFERS_H


typedef struct {
  int   size; // size of buff |  NOTE: != to width * height when padded
  int   width;
  int   height;
  char *buff;
} TBuffer;

typedef struct {
  TBuffer front; // padded
  TBuffer back;  // clean 
} TBufferPair;

typedef struct {
  int win_w;
  int win_h;
  TBufferPair tbuffers;
} TScreen;


TBufferPair new_tbuffer_pair(int width, int height);
void free_tbuffer_pair(TBufferPair *tbuffers);
void write_to_back_tbuffer(TBufferPair *tbuffers, char c, int x, int y);
void display_front_tbuffer(TScreen *tscreen);

TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h);
char *pad_tbuffer(TScreen *tscreen);

#endif
