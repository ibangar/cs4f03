#include "image.h"

#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

struct image {
    struct image_resolution resolution;
    enum image_format format;
    void *data;
    size_t data_size;
};

void _image_data_alloc(image_t *image);
void _image_data_free(image_t *image);

image_t *
image_create(
    struct image_resolution *resolution,
    enum image_format format)
{
    /* check arguments */
    if (!resolution)
    {
        return NULL;
    }

    /* allocate struct for image */
    image_t *image = (image_t *)malloc(sizeof(struct image));

    if (!image)
    {
        return NULL;
    }

    /* set image properties */
    image->resolution = *resolution;
    image->format = format;

    /* allocate image data */
    _image_data_alloc(image);

    return image;
}

void
image_free(
    image_t *image)
{
    if (!image)
    {
        return;
    }

    _image_data_free(image);
    free(image);
}

int
image_get_width(
    image_t *image)
{
    if (!image)
    {
        return -1;
    }

    return image->resolution.width;
}

int
image_get_height(
    image_t *image)
{
    if (!image)
    {
        return -1;
    }

    return image->resolution.height;
}

void
image_clear(
    image_t *image,
    image_color_t color)
{
    size_t i;
    image_color_t *pixel = image->data;

    for (i = 0; i < image->data_size / sizeof(image_color_t); i++)
    {
        pixel[i] = color;
    }
}

void
image_set_pixel(
    image_t *image,
    int x,
    int y,
    image_color_t color)
{
    image_color_t *pixel = image->data;

    int w = image->resolution.width;
    pixel[w * y + x] = color;
}

int
image_save(
    image_t *image,
    const char *path)
{
    int fd = open(path, O_CREAT | O_RDWR, 0666);

    if (fd <= 0)
    {
        return -1;
    }

    ssize_t bytes_written = write(fd, image->data, image->data_size);

    if (bytes_written == image->data_size)
    {
        return 0;
    }

    return -1;
}

void
_image_data_alloc(
    image_t *image)
{
    switch (image->format)
    {
        case IMAGE_FORMAT_ARGB:
        {
            image->data_size = image->resolution.width
                             * image->resolution.height
                             * 4;
            image->data = malloc(image->data_size);
            break;
        }
    }
}

void
_image_data_free(
    image_t *image)
{
    if (image->data)
    {
        free(image->data);
    }
}
