#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include "grfx.h"

FBuffer open_fb(char *fb_dev) {
  int fb = open(fb_dev, O_RDWR);

  struct fb_var_screeninfo vinfo;
  struct fb_fix_screeninfo finfo;

  ioctl(fb, FBIOGET_VSCREENINFO, &vinfo);
  ioctl(fb, FBIOGET_FSCREENINFO, &finfo);

  long screensize = vinfo.yres_virtual * finfo.line_length;

  char *fbp = mmap(0, screensize, PROT_READ | PROT_WRITE, MAP_SHARED, fb, 0);

  FBuffer fbuffer = {
    .fb = fb,
    .vinfo = vinfo,
    .finfo = finfo,
    .fbp = fbp,
    .screensize = screensize
  };


  return fbuffer;
}

void close_fb(FBuffer *fbuffer) {
  munmap(fbuffer->fbp, fbuffer->screensize);
  close(fbuffer->fb);  
}

void set_pixel(FBuffer *fbuffer, int x, int y, ColorRGB color) {

  if (x < 0 || x >= (int)fbuffer->vinfo.xres || y < 0 ||
      y >= (int)fbuffer->vinfo.yres) {
    return;
  }

  long location =
      (x + fbuffer->vinfo.xoffset) * (fbuffer->vinfo.bits_per_pixel / 8) +
      (y + fbuffer->vinfo.yoffset) * fbuffer->finfo.line_length;

  if (fbuffer->vinfo.bits_per_pixel == 32) {
    fbuffer->fbp[location] = color.b;
    fbuffer->fbp[location + 1] = color.g;
    fbuffer->fbp[location + 2] = color.r;
    fbuffer->fbp[location + 3] = 0;
  }

  /* maybe handle this at somepoint idk?
    else if(fbuffer->vinfo.bits_per_pixel = 16) {}
  */
}
