#ifndef __JULIA_H__
#define __JULIA_H__

#include "complex.h"
#include "image.h"

typedef struct julia_image julia_image_t;

julia_image_t *julia_image_init(struct image_resolution *resolution, int m, struct complex_number c,
                                struct complex_number (*space_map)(int, int, int, int),
                                image_color_t (*color_map)(unsigned int, unsigned int));
void julia_image_free(julia_image_t *julia);

int julia_render_lines(julia_image_t *julia, unsigned int lines);
image_t *julia_get_image(julia_image_t *julia);

#endif
