#include "buffers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ENDLINE = "\n\r"

static TBuffer new_tbuffer(int size, int size_padded, int width, int height) {

  char *new_buff = (char *)calloc(size, sizeof(char));
  char *new_buff_padded = (char *)calloc(size_padded, sizeof(char));
  for (int i = width; i < size_padded; i += width + 2) {
    new_buff_padded[i] = '\n';
    new_buff_padded[i + 1] = '\r';
  }

  new_buff_padded[size_padded - 2] = '\0';

  return (TBuffer){size, size_padded, width, height, new_buff, new_buff_padded};
}

TBufferPair new_tbuffer_pair(int width, int height) {

  int size = width * height;
  int size_padded = size + height * 2; // make room for \n and \r

  TBuffer back = new_tbuffer(size, size_padded, width, height);
  TBuffer front = new_tbuffer(size, size_padded, width, height);
  return (TBufferPair){front, back};
}

void free_tbuffer_pair(TBufferPair *tbuffers) {

  free(tbuffers->front.buff);
  free(tbuffers->back.buff);
  free(tbuffers->front.buff_padded);
  free(tbuffers->back.buff_padded);
}

void write_to_back_tbuffer(TBufferPair *tbuffers, char c, int x, int y) {

  if (x >= tbuffers->back.width || x < 0)
    return;

  if (y >= tbuffers->back.height || y < 0)
    return;

  int loc = (y * tbuffers->back.width) + x;
  int loc_padded = (y * (tbuffers->back.width + 2)) + x;

  if (loc < 0 || loc > tbuffers->back.size) {
    return;
  }
  if (loc_padded < 0 || loc_padded > tbuffers->back.size_padded) {
    return;
  }

  tbuffers->back.buff[loc] = c;
  tbuffers->back.buff_padded[loc_padded] = c;
}

void display_front_tbuffer(TBufferPair *tbuffers) {
  memcpy(tbuffers->front.buff, tbuffers->back.buff, tbuffers->back.size);
  memcpy(tbuffers->front.buff_padded, tbuffers->back.buff_padded,
         tbuffers->back.size_padded);
  printf("%s", tbuffers->front.buff_padded);

  fflush(stdout);
}
