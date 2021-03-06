#include "julia.h"
#include "mandelbrot.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/* help message function */
void print_help(const char *program);
int verify_args(int argc, char **argv);

#define MIN_WIDTH       (800)
#define MAX_WIDTH       (1920)
#define MIN_HEIGHT      (600)
#define MAX_HEIGHT      (1080)
#define MIN_ITERATIONS  (100)
#define MAX_ITERATIONS  (10000)

#define CLOCK_DIFF_US(a, b) (b.tv_sec * 1000000UL + b.tv_nsec / 1000UL) - \
                            (a.tv_sec * 1000000UL + a.tv_nsec / 1000UL)

#define ZOOM_X (0.40)
#define ZOOM_Y (0.40)
#define SHIFT_X (-1020)
#define SHIFT_Y (-500)

/* maps for renderer */
struct complex_number space_map(int p_x, int p_y, int d_w, int d_h);
image_color_t color_map(unsigned int k, unsigned int m);

int
main(
    int argc,
    char **argv)
{
    /* verify input arguments */
    if (verify_args(argc, argv) < 0)
    {
        print_help(argv[0]);
        return -1;
    }

    /* get arguments */
    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    unsigned int m = atoi(argv[3]);

    struct image_resolution resolution =
    {
        .width = w,
        .height = h
    };

    image_t *image;

    /* render a mandelbrot image */
    mandelbrot_image_t *mandelbrot = mandelbrot_image_init(&resolution, m,
                                            space_map, color_map);
    while (mandelbrot_render_lines(mandelbrot, 108));
    image = mandelbrot_get_image(mandelbrot);
    image_save(image, "./mandelbrot.rgb");
    mandelbrot_image_free(mandelbrot);

    /* render a julia image */
    julia_image_t *julia = julia_image_init(&resolution, m, complex_number(-1.0, 0.0),
                                            space_map, color_map);
    while (julia_render_lines(julia, 108));
    image = julia_get_image(julia);
    image_save(image, "./julia.rgb");
    julia_image_free(julia);

    /* success ! */
    exit(EXIT_SUCCESS);
}

void
print_help(
    const char *program)
{
    printf("usage : %s <width> <height> <iterations>\n", program);
    printf("    width      : The width of the output image.\n");
    printf("    height     : The height of the output image.\n");
    printf("    iterations : The number of iterations to perform on the function.\n");
}

int
verify_args(
    int argc,
    char **argv)
{
    if (argc < 4)
    {
        printf("Missing a required argument.\n\n");
        return -1;
    }

    int w = atoi(argv[1]);
    int h = atoi(argv[2]);
    int m = atoi(argv[2]);

    if (w < MIN_WIDTH || w > MAX_WIDTH)
    {
        printf("Incorrect width, %d < width < %d\n\n",
               MIN_WIDTH, MAX_WIDTH);
        return -1;
    }

    if (h < MIN_HEIGHT || h > MAX_HEIGHT)
    {
        printf("Incorrect height, %d < height < %d\n\n",
               MIN_HEIGHT, MAX_HEIGHT);
        return -1;
    }

    if(m < MIN_ITERATIONS || m > MAX_ITERATIONS)
    {
        printf("Incorrect iteration count, %d < iterations < %d\n\n",
               MIN_ITERATIONS, MAX_ITERATIONS);
        return -1;
    }

    return 0;
}

struct complex_number
space_map(
    int p_x,
    int p_y,
    int d_w,
    int d_h)
{
    p_x += SHIFT_X;
    p_y += SHIFT_Y;
    struct complex_number c;
    c = complex_number((double)p_x * (1.0 / ZOOM_X) / d_w,
                       (double)p_y * (1.0 / ZOOM_Y) / d_h);
    return c;
}

image_color_t
color_map(
    unsigned int k,
    unsigned int m)
{
    unsigned int v = (k * 0x00ffffff) / m;
    return (v << 8) | (0x000000ff);
}
