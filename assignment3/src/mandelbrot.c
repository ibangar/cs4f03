#include "mandelbrot.h"

unsigned int _mandelbrot_escape_value(struct complex_number c, unsigned int m);

void
mandelbrot_render(
    image_t *image,
    unsigned int m,
    struct complex_number (*space_map)(int, int, int, int),
    image_color_t (*color_map)(unsigned int, unsigned int))
{
    int w = image_get_width(image);
    int h = image_get_height(image);

    /* iterate over every pixel of the image */
    int a, b;
    for (a = 0; a < w; a++)
    {
        for (b = 0; b < h; b++)
        {
            struct complex_number c = space_map(a, b, w, h);
            unsigned int k = _mandelbrot_escape_value(c, m);
            image_color_t pixel_color = color_map(k, m);
            image_set_pixel(image, a, b, pixel_color);
        }
    }
}

unsigned int
_mandelbrot_escape_value(
    struct complex_number c,
    unsigned int m)
{
    int k;
    struct complex_number z = { 0, 0 };

    for (k = 0; k < m; k++)
    {
        /* z = z^2 + c */
        z = complex_add(complex_mul(z, z), c);

        /* check escape value */
        if (complex_mag(z) > 2)
        {
            return k;
        }
    }

    return m;
}

