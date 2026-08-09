#include "tscreen.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "term_utils.h"

static TBuffer new_tbuffer(int width, int height);
static char *center_tbuffer(TScreen *tscreen);
static void stitch_buffs(TScreen *tscreen);


static TBuffer new_tbuffer(int width, int height) {
  
  int size = width * height;
  char *new_buff = (char *)calloc(size, sizeof(char));
  
  return (TBuffer){size, width, height, new_buff};
}


TScreen new_tscreen(int win_w, int win_h, int buff_w, int buff_h) {
  
  TBuffer tbuffer = new_tbuffer(DISPLAY_W + LOG_W, LOG_H);
  TBuffer display = new_tbuffer(DISPLAY_W, DISPLAY_H);
  TBuffer log     = new_tbuffer(LOG_W, LOG_H);
  TBuffer meter   = new_tbuffer(METER_W, METER_H);
  
  return (TScreen){win_w, win_h, tbuffer, display, log, meter};
}


void free_tscreen(TScreen *tscreen) {
  
  free(tscreen->tbuffer.buff);
  free(tscreen->display.buff);
  free(tscreen->log.buff);
  free(tscreen->meter.buff);
  
}


static void stitch_buffs(TScreen *tscreen) {
    
  int dh = tscreen->display.height;
  int dw = tscreen->display.width;
  int lw = tscreen->log.width;
  int mw = tscreen->meter.width;
  int mh = tscreen->meter.height;

  if (tscreen->tbuffer.size < (dw+lw)*dh + (mw+lw)*mh) {
    fprintf(stderr, "Buffer size too small");
    abort();
  }
  
  // just copy the lines over
  for (int i = 0; i < dh; i++) {
    char *b_start = tscreen->tbuffer.buff + (dw + lw) * i;
    char *d_start = tscreen->display.buff + dw * i;
    char *l_start = tscreen->log.buff + lw * i;
    memcpy(b_start, d_start, dw);
    memcpy(b_start + dw, l_start, lw);
  }

  int offset = (dw + lw) * dh;

  for (int i = 0; i < mh; i++) {
    char *b_start = tscreen->tbuffer.buff + (mw + lw) * i + offset;
    char *m_start = tscreen->meter.buff + mw * i;
    char *l_start = tscreen->log.buff + lw * (dh + i);
    memcpy(b_start, m_start, mw);
    memcpy(b_start + mw, l_start, lw);
  }
}


static char *center_tbuffer(TScreen *tscreen) {
  
  int bh = tscreen->tbuffer.height;
  int bw = tscreen->tbuffer.width;

  int top_pad  = (tscreen->win_h - bh) / 2;
  int left_pad = (tscreen->win_w - bw) / 2;
  if (top_pad  < 0) top_pad  = 0;
  if (left_pad < 0) left_pad = 0;

  int cols  = bw < tscreen->win_w ? bw : tscreen->win_w;   // clip width
  int avail = tscreen->win_h - top_pad - 1;
  int rows  = bh < avail ? bh : avail;                     // clip height
  if (rows < 0) rows = 0;

  int cap = top_pad * 2 + rows * (left_pad + cols + 2) + 1;
  char *out = malloc(cap);

  int p = 0;
  for (int i = 0; i < top_pad; i++) { out[p++] = '\r'; out[p++] = '\n'; }
  for (int y = 0; y < rows; y++) {
    memset(out + p, ' ', left_pad);
    p += left_pad;
    memcpy(out + p, tscreen->tbuffer.buff + y * bw, cols);
    p += cols;
    out[p++] = '\r';
    out[p++] = '\n';
  }
  
  out[p] = '\0';
  
  return out;
}


void set_cell(TBuffer *tbuffer, char c, int x, int y) {
  
  int buffer_h = tbuffer->height;
  int buffer_w = tbuffer->width;
  int buffer_s = tbuffer->size;
  
  int loc = (y * buffer_w) + x;

  if (x >= buffer_w  || x < 0)   return; // outside x
  if (y >= buffer_h  || y < 0)   return; // outside y
  if (loc > buffer_s || loc < 0) return; // is valid loc
  
  tbuffer->buff[loc] = c;
  
}


void tscreen_print(TScreen *tscreen, char *str, int x, int y) {
  int buffer_h = tscreen->tbuffer.height;
  int buffer_w = tscreen->tbuffer.width;
  int buffer_s = tscreen->tbuffer.size;
  
  int loc = (y * buffer_w) + x;

  if (x >= buffer_w  || x < 0)   return; // outside x
  if (y >= buffer_h  || y < 0)   return; // outside y
  if (loc > buffer_s || loc < 0) return; // is valid loc

  for (int i = 0; i < strlen(str); i++) {
    if (loc + i >= tscreen->tbuffer.size) break;  
    tscreen->tbuffer.buff[loc + i] = str[i];
  }

}


void display(TScreen *tscreen) {
  move_cursor(0, 0);
  stitch_buffs(tscreen);
  char *centered_buffer = center_tbuffer(tscreen);
  printf("%s", centered_buffer);
  free(centered_buffer); // this is wastfull; maybe keep in tscreen?
  fflush(stdout);
}
