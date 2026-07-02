#include "buffers.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


static TBuffer new_tbuffer(int width, int height);
static int     insert_buff(char *buff, int capacity, char *ins_buff, int start, int len);
static int     insert_char_chunk(char *buff, int capacity, int start, int len, char c);
static int     insert_newline(char *buff, int capacity, int start);
static int     remove_char_chunk(char *buff, int capacity, int start, int len);
static char   *center_tbuffer(TScreen *tscreen);

// returns ending pos
static int insert_buff(char *buff, int capacity, char *ins_buff, int start, int len) {

  int tail_len = strlen(buff) - start;

  if (tail_len < 0 || start < 0 || len < 0) {
    fprintf(stderr, "Bad Bounds\n");
    abort();
  }

  if (start + len > capacity) {
    fprintf(stderr, "Out of Bounds\n");
    abort();
  }
  
  memmove(buff + start + len, buff + start, tail_len + 1);
  memcpy(buff + start, ins_buff, len);
  
  return start + len;
}


static int insert_char_chunk(char *buff, int capacity, int start, int len, char c) {
  
  char ins_buff[len];
  memset(ins_buff, c, len);
  
  return insert_buff(buff, capacity, ins_buff, start, len);
}


static int insert_newline(char *buff, int capacity, int start) {
  
  const int len = 2;
  char ins_buff[2] = "\r\n";
  
  return insert_buff(buff, capacity, ins_buff, start, len);
}


static int remove_char_chunk(char *buff, int capacity, int start, int len) {
  
  int tail_len = strlen(buff) - start;

  if (tail_len < 0 || start < 0 || len < 0) {
    fprintf(stderr, "Bad Bounds\n");
    abort();
  }

  if (start + len > capacity) {
    fprintf(stderr, "Out of Bounds\n");
    abort();
  }
  
  memmove(buff + start, buff + start + len, tail_len + 1);
  
  return start;  
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


static char *center_tbuffer(TScreen *tscreen) {
  
  int buffer_h = tscreen->tbuffer.height;
  int buffer_w = tscreen->tbuffer.width;
  int top_pad  = (tscreen->win_h - buffer_h) / 2;
  int left_pad = (tscreen->win_w - buffer_w) / 2;
  
  if (top_pad < 0)  top_pad = 0;
  if (left_pad < 0) left_pad = 0;

  int padded_w = (left_pad + buffer_w + 2);
    
  // the top padding added plus the left padding * the width + \r\n len
  int tscreen_size = (top_pad * 2) + (buffer_h * padded_w);
  int tbuffer_size = tscreen->tbuffer.size;
  
  char* new_buffer = (char *)calloc(tscreen_size + 1, sizeof(char)); // extra char for \0
  memcpy(new_buffer, tscreen->tbuffer.buff, tbuffer_size);
  new_buffer[tscreen_size] = '\0';

  int next_pos = 0; // next insert position

  //insert top padding
  for (int i = 0; i < top_pad; i++) {
    next_pos = insert_newline(new_buffer, tscreen_size, next_pos);
  }

  //insert left padding
  for (int i = 0; i < buffer_h; i++) {
    next_pos  = insert_char_chunk(new_buffer, tscreen_size + 1, next_pos, left_pad, ' ');
    
    if (buffer_w > tscreen->win_w) {
      next_pos += tscreen->win_w;
      remove_char_chunk(new_buffer, tscreen_size + 1, next_pos, buffer_w - tscreen->win_w);
    } else {
      next_pos += buffer_w;
    }
    
    next_pos  = insert_newline(new_buffer, tscreen_size + 1, next_pos);
  }
  
  new_buffer[tscreen_size] = '\0';

  return new_buffer;
}


void set_cell(TScreen *tscreen, char c, int x, int y) {
  
  int buffer_h = tscreen->tbuffer.height;
  int buffer_w = tscreen->tbuffer.width;
  int buffer_s = tscreen->tbuffer.size;
  
  int loc = (y * buffer_w) + x;

  if (x >= buffer_w  || x < 0)   return; // outside x
  if (y >= buffer_h  || y < 0)   return; // outside y
  if (loc > buffer_s || loc < 0) return; // is valid loc
  
  tscreen->tbuffer.buff[loc] = c;
  
}


void display(TScreen *tscreen) {
  
  char *centered_buffer = center_tbuffer(tscreen);
  printf("%s", centered_buffer);
  free(centered_buffer); // this is wastfull; maybe keep in tscreen?
  fflush(stdout);
  
}
