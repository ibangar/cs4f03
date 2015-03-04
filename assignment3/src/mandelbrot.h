#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

#include "complex.h"
#include "image.h"

void mandelbrot_render(image_t *image, unsigned int m,
                       struct complex_number (*space_map)(int, int, int, int),
                       image_color_t (*color_map)(unsigned int, unsigned int));

#endif
