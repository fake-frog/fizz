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
  
  int buffer_h = tscreen->tbuffer.height;
  int buffer_w = tscreen->tbuffer.width;
  int top_pad  = (tscreen->win_h - buffer_h) / 2;
  int left_pad = (tscreen->win_w - buffer_w) / 2;
  int padded_w = (left_pad + buffer_w + 2);
    
  // the top padding added plus the left padding * the width + \r\n len
  int tscreen_size = (top_pad * 2) + (buffer_h * padded_w);
   
  char* new_buffer = (char *)calloc(tscreen_size + 1, sizeof(char)); // extra char for \0
  memcpy(new_buffer, tscreen->tbuffer.buff, tscreen->tbuffer.size);

  int next_pos = 0; // next insert position
  
  // insert top
  for (int i = 0; i < top_pad; i++) {
    next_pos = insert_newline(new_buffer, next_pos);
  }

  // insert sides
  for (int i = 0; i < buffer_h; i++) {
    next_pos  = insert_char_chunk(new_buffer, next_pos, left_pad, ' ');
    next_pos += buffer_w; // shift width
    next_pos  = insert_newline(new_buffer, next_pos);
  }
  
  new_buffer[tscreen_size] = '\0';

  return new_buffer;
}


void set_cell(TScreen *tscreen, char c, int x, int y) {
  
  int buffer_h = tscreen->tbuffer.height;
  int buffer_w = tscreen->tbuffer.width;
  
  int loc = (y * buffer_w) + x;
  
  if (x >= buffer_w || x < 0)   return; // outside x
  if (y >= buffer_h || y < 0)  return; // outside y
  if (loc < 0 || loc > tscreen->tbuffer.size) return; // past end
  
  tscreen->tbuffer.buff[loc] = c;
  
}


void display(TScreen *tscreen) {
  
  char *padded_buffer = pad_tbuffer(tscreen);
  printf("%s", padded_buffer);
  free(padded_buffer); // this is wastfull
  fflush(stdout);
  
}
