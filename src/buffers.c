#include "buffers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static TBuffer new_tbuffer(int width, int height);
static int insert_char_chunk(char *buff, int start, int len, char c);
static int insert_newline(char *buff, int start);


// returns ending pos
static int insert_char_chunk(char *buff, int start, int len, char c) {
  
  char ins_buff[len];
  memset(ins_buff, c, len);
  int tail_len = strlen(buff) - start;

  memmove(buff + start + len, buff + start, tail_len + 1);
  memcpy(buff + start, ins_buff, len);

  return start + len;
}


// returns ending pos
static int insert_newline(char *buff, int start) {
  
  const int len = 2;
  char ins_buff[2] = "\r\n";
  int tail_len = strlen(buff) - start;

  memmove(buff + start + len, buff + start, tail_len + 1);
  memcpy(buff + start, ins_buff, len);
  
  return start + len;
}


static TBuffer new_tbuffer(int width, int height) {
  
  int size = width * height;
  char *new_buff = (char *)calloc(size + 1, sizeof(char));
  new_buff[size] = '\0';
  
  return (TBuffer){size, width, height, new_buff};
}


TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h) {
  
  TBuffer tbuffer = new_tbuffer(buff_w, buff_h);
  
  return (TScreen){win_w, win_h, tbuffer};
}


void free_tscreen(TScreen *tscreen) {
  
  free(tscreen->tbuffer.buff);
  
}


char *pad_tbuffer(TScreen *tscreen) {
  
  int top_pad  = (tscreen->win_h - tscreen->tbuffer.height) / 2;
  int left_pad = (tscreen->win_w - tscreen->tbuffer.width)  / 2;

  // the top padding added plus the left padding * the width plus \r\n
  
  int tscreen_size = (top_pad * 2)
    + tscreen->tbuffer.height
    * (left_pad + tscreen->tbuffer.width + 2) + 1;
   
  char* new_buffer = (char *)calloc(tscreen_size + 1, sizeof(char));
  memcpy(new_buffer, tscreen->tbuffer.buff, tscreen->tbuffer.size);

  int next_pos = 0; // next insert position
  
  // insert top
  for (int i = 0; i < top_pad; i++) {
    next_pos = insert_newline(new_buffer, next_pos);
  }

  // insert sides
  for (int i = 0; i < tscreen->tbuffer.height; i++) {
    next_pos = insert_char_chunk(new_buffer, next_pos, left_pad, ' ');
    next_pos += tscreen->tbuffer.width;
    next_pos = insert_newline(new_buffer, next_pos);
  }
  
  new_buffer[tscreen_size] = '\0';

  return new_buffer;
}


void set_cell(TScreen *tscreen, char c, int x, int y) {

  if (x >= tscreen->tbuffer.width || x < 0)
    return;

  if (y >= tscreen->tbuffer.height || y < 0)
    return;

  int loc = (y * tscreen->tbuffer.width) + x;

  if (loc < 0 || loc > tscreen->tbuffer.size) {
    return;
  }

  tscreen->tbuffer.buff[loc] = c;
  
}


void display(TScreen *tscreen) {
  
  char *padded_buffer = pad_tbuffer(tscreen);
  printf("%s", padded_buffer);
  free(padded_buffer); // this is wastfull
  fflush(stdout);
  
}
