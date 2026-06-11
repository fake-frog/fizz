#ifndef BUFFERS_H
#define BUFFERS_H

typedef struct {
  int size;
  int size_padded;
  int width;
  int height;
  char *buff;
  char *buff_padded;
} TBuffer;

typedef struct {
  TBuffer front;
  TBuffer back;
} TBufferPair;

static TBuffer new_tbuffer(int size, int size_padded, int width, int height);
TBufferPair new_tbuffer_pair(int width, int height);
void free_tbuffer_pair(TBufferPair *tbuffers);
void write_to_back_tbuffer(TBufferPair *tbuffers, char c, int x, int y);
void display_front_tbuffer(TBufferPair *tbuffers);

#endif
