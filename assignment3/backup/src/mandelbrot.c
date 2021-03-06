#include "mandelbrot.h"

#include <stdlib.h>

#include <pthread.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))

struct mandelbrot_image {
    image_t *image;
    pthread_mutex_t mutex;
    int line;
    int m;
    struct complex_number (*space_map)(int, int, int, int);
    image_color_t (*color_map)(unsigned int, unsigned int);
};

unsigned int _mandelbrot_escape_value(struct complex_number z, unsigned int m);

mandelbrot_image_t *
mandelbrot_image_init(
    struct image_resolution *resolution,
    int m,
    struct complex_number (*space_map)(int, int, int, int),
    image_color_t (*color_map)(unsigned int, unsigned int))
{
    /* allocate context */
    mandelbrot_image_t *mandelbrot = (mandelbrot_image_t *)malloc(sizeof(struct mandelbrot_image));

    if (!mandelbrot)
    {
        return NULL;
    }

    /* create image holder */
    mandelbrot->image = image_create(resolution, IMAGE_FORMAT_ARGB);

    if (!mandelbrot->image)
    {
        mandelbrot_image_free(mandelbrot);
        return NULL;
    }

    /* initialize image properties */
    pthread_mutex_init(&mandelbrot->mutex, NULL);
    mandelbrot->line = 0;
    mandelbrot->m = m;
    mandelbrot->space_map = space_map;
    mandelbrot->color_map = color_map;

    return mandelbrot;
}

void
mandelbrot_image_free(
    mandelbrot_image_t *mandelbrot)
{
    if (!mandelbrot)
    {
        return;
    }

    if (mandelbrot->image)
    {
        image_free(mandelbrot->image);
    }

    pthread_mutex_destroy(&mandelbrot->mutex);

    free(mandelbrot);
}

int
mandelbrot_render_lines(
    mandelbrot_image_t *mandelbrot,
    unsigned int lines)
{
    int w = image_get_width(mandelbrot->image);
    int h = image_get_height(mandelbrot->image);

    /* critical section, grabbing a start / end section to render */
    pthread_mutex_lock(&mandelbrot->mutex);

    unsigned int line_start = mandelbrot->line;
    unsigned int line_end = MIN(w - 1, mandelbrot->line + lines);
    mandelbrot->line = line_end;

    pthread_mutex_unlock(&mandelbrot->mutex);

    /* iterate over every pixel of the image */
    int a, b;
    for (b = line_start; b < line_end; b++)
    {
        for (a = 0; a < w; a++)
        {
            struct complex_number z = mandelbrot->space_map(a, b, w, h);
            unsigned int k = _mandelbrot_escape_value(z, mandelbrot->m);
            image_color_t pixel_color = mandelbrot->color_map(k, mandelbrot->m);
            image_set_pixel(mandelbrot->image, a, b, pixel_color);
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
mandelbrot_get_image(
    mandelbrot_image_t *mandelbrot)
{
    if (!mandelbrot)
    {
        return NULL;
    }

    return mandelbrot->image;
}

unsigned int
_mandelbrot_escape_value(
    struct complex_number z,
    unsigned int m)
{
    int k;
    struct complex_number z_acc = z;

    for (k = 0; k < m; k++)
    {
        /* z = z^2 + c */
        z_acc = complex_add(complex_mul(z_acc, z_acc), z);

        /* check escape value */
        if (complex_mag(z_acc) > 2)
        {
            return k;
        }
    }

    return m;
}

