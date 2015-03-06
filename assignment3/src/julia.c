#include "julia.h"

#include <stdlib.h>

#include <pthread.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct julia_image {
    image_t *image;
    pthread_mutex_t mutex;
    int line;
    int m;
    struct complex_number c;
    struct complex_number (*space_map)(int, int, int, int);
    image_color_t (*color_map)(unsigned int, unsigned int);
};

unsigned int _julia_escape_value(struct complex_number z, struct complex_number c, unsigned int m);

julia_image_t *
julia_image_init(
    struct image_resolution *resolution,
    int m,
    struct complex_number c,
    struct complex_number (*space_map)(int, int, int, int),
    image_color_t (*color_map)(unsigned int, unsigned int))
{
    /* allocate context */
    julia_image_t *julia = (julia_image_t *)malloc(sizeof(struct julia_image));

    if (!julia)
    {
        return NULL;
    }

    /* create image holder */
    julia->image = image_create(resolution, IMAGE_FORMAT_ARGB);

    if (!julia->image)
    {
        julia_image_free(julia);
        return NULL;
    }

    /* initialize image properties */
    pthread_mutex_init(&julia->mutex, NULL);
    julia->line = 0;
    julia->m = m;
    julia->c = c;
    julia->space_map = space_map;
    julia->color_map = color_map;

    return julia;
}

void
julia_image_free(
    julia_image_t *julia)
{
    if (!julia)
    {
        return;
    }

    if (julia->image)
    {
        image_free(julia->image);
    }

    pthread_mutex_destroy(&julia->mutex);

    free(julia);
}

int
julia_render_lines(
    julia_image_t *julia,
    unsigned int lines)
{
    int w = image_get_width(julia->image);
    int h = image_get_height(julia->image);

    /* critical section, grabbing a start / end section to render */
    pthread_mutex_lock(&julia->mutex);

    unsigned int line_start = julia->line;
    unsigned int line_end = MIN(w, julia->line + lines);
    julia->line = line_end;

    pthread_mutex_unlock(&julia->mutex);

    /* iterate over every pixel of the image */
    int a, b;
    for (b = line_start; b < line_end; b++)
    {
        for (a = 0; a < w; a++)
        {
            struct complex_number z = julia->space_map(a, b, w, h);
            unsigned int k = _julia_escape_value(z, julia->c, julia->m);
            image_color_t pixel_color = julia->color_map(k, julia->m);
            image_set_pixel(julia->image, a, b, pixel_color);
        }
    }

    /* returns 0 if we're finished, 1 if more to render */
    if (line_end == h)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

image_t *
julia_get_image(
    julia_image_t *julia)
{
    if (!julia)
    {
        return NULL;
    }

    return julia->image;
}

unsigned int
_julia_escape_value(
    struct complex_number z,
    struct complex_number c,
    unsigned int m)
{
    int k;
    struct complex_number z_acc = z;

    for (k = 0; k < m; k++)
    {
        /* z = z^2 + c */
        z_acc = complex_add(complex_mul(z_acc, z_acc), c);

        /* check escape value */
        if (complex_mag(z_acc) > 2)
        {
            return k;
        }
    }

    return m;
}

