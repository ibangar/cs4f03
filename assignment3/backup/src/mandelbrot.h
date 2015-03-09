#ifndef __MANDELBROT_H__
#define __MANDELBROT_H__

#include "complex.h"
#include "image.h"

typedef struct mandelbrot_image mandelbrot_image_t;

mandelbrot_image_t *mandelbrot_image_init(struct image_resolution *resolution, int m,
                                          struct complex_number (*space_map)(int, int, int, int),
                                          image_color_t (*color_map)(unsigned int, unsigned int));
void mandelbrot_image_free(mandelbrot_image_t *mandelbrot);

int mandelbrot_render_lines(mandelbrot_image_t *mandelbrot, unsigned int lines);
image_t *mandelbrot_get_image(mandelbrot_image_t *mandelbrot);

#endif
