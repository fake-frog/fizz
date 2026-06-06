#ifndef GRFX_H
#define GRFX_H

typedef struct {
  int fb;
  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;
  char *fbp;
  long screensize;
} FBuffer;

typedef struct {
  unsigned short r;
  unsigned short g;
  unsigned short b;
} ColorRGB;

void set_pixel(FBuffer *fbuffer, int x, int y, ColorRGB color);
FBuffer open_fb(char *fb_dev);
void close_fb(FBuffer *fb);

#endif
