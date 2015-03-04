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

#define ZOOM_X (0.050)
#define ZOOM_Y (0.050)

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

    /* create image for mandelbrot set */
    image_t *image = image_create(&resolution, IMAGE_FORMAT_ARGB);

    if (!image)
    {
        printf("failed to create image\n");
        exit(EXIT_FAILURE);
    }

    /* fill image with mandelbrot and save it */
    mandelbrot_render(image, m, space_map, color_map);
    image_save(image, "./mandelbrot.rgb");
    image_free(image);

    /* create image for julia set */
    image = image_create(&resolution, IMAGE_FORMAT_ARGB);

    if (!image)
    {
        printf("failed to create image\n");
        exit(EXIT_FAILURE);
    }

    /* fill image with mandelbrot and save it */
    julia_render(image, m, complex_number(0.0, 1.0), space_map, color_map);
    image_save(image, "./julia.rgb");
    image_free(image);

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
    p_x -= d_w / 1.5;
    p_y -= d_h / 2;
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
