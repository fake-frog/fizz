#include "buffers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static TBuffer new_tbuffer(int size, int width, int height);
static int insert_char_chunk(char *buff, int start, int len, char c);

// returns ending pos
static int insert_char_chunk(char *buff, int start, int len, char c) {
  char ins_buff[len];
  memset(ins_buff, c, len);
  int tail_len = strlen(buff) - start;

  memmove(buff + start + len, buff + start, tail_len + 1);
  memcpy(buff + start, ins_buff, len);

  return start + len;
}


static TBuffer new_tbuffer(int size, int width, int height) {

  char *new_buff = (char *)calloc(size + 1, sizeof(char));
  new_buff[size] = '\0';
  /*
  char *new_buff_padded = (char *)calloc(size_padded, sizeof(char));
  
  for (int i = width; i < size_padded; i += width + 2) {
    new_buff_padded[i] = '\n';
    new_buff_padded[i + 1] = '\r';
  }

  new_buff_padded[size_padded - 2] = '\0';
  */
  return (TBuffer){size, width, height, new_buff};
}

TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h) {

  TBufferPair tbuffers = new_tbuffer_pair(buff_w, buff_h);
  return (TScreen){win_w, win_h, tbuffers};
}

char *pad_tbuffer(TScreen *tscreen) {
  int tscreen_size = tscreen->win_w * tscreen->win_h;
  char* new_buffer = (char *)calloc(tscreen_size + 1, sizeof(char));
  memcpy(new_buffer, tscreen->tbuffers.front.buff, tscreen->tbuffers.front.size);
  
  // insert top
  int last_pos = insert_char_chunk(new_buffer, 0,
		 ((tscreen->win_h - tscreen->tbuffers.front.height) / 2)
		 * tscreen->win_w, ' ');

  // insert sides
  for (int i = 0; i < tscreen->tbuffers.front.height; i++) {
    last_pos = insert_char_chunk(new_buffer, last_pos,
	       (tscreen->win_w - tscreen->tbuffers.front.width) / 2, ' ');
    
    last_pos = insert_char_chunk(new_buffer, last_pos
	       + tscreen->tbuffers.front.width,
	       (tscreen->win_w - tscreen->tbuffers.front.width) / 2, ' ');
  }
  
  new_buffer[tscreen_size] = '\0';
  return new_buffer;
}

TBufferPair new_tbuffer_pair(int width, int height) {
  int size = width * height;
  TBuffer back = new_tbuffer(size, width, height);
  TBuffer front = new_tbuffer(size, width, height);
  return (TBufferPair){front, back};
}


void free_tbuffer_pair(TBufferPair *tbuffers) {
  free(tbuffers->front.buff);
  free(tbuffers->back.buff);
}


void write_to_back_tbuffer(TBufferPair *tbuffers, char c, int x, int y) {

  if (x >= tbuffers->back.width || x < 0)
    return;

  if (y >= tbuffers->back.height || y < 0)
    return;

  int loc = (y * tbuffers->back.width) + x;

  if (loc < 0 || loc > tbuffers->back.size) {
    return;
  }

  tbuffers->back.buff[loc] = c;
}


void display_front_tbuffer(TScreen *tscreen) {
  
  memcpy(tscreen->tbuffers.front.buff, tscreen->tbuffers.back.buff, tscreen->tbuffers.back.size);
  // this needs to return a new buffer
  char *padded_buffer = pad_tbuffer(tscreen);
  printf("%s", padded_buffer);
  free(padded_buffer);

  fflush(stdout);
}
