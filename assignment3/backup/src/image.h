#ifndef __IMAGE_H__
#define __IMAGE_H__

#define ARGB(a, r, g, b) ((((unsigned char)(a)) & 0x000000ff) | \
                          (((unsigned char)(r)) << 8) | \
                          (((unsigned char)(g)) << 16) | \
                          (((unsigned char)(b)) << 24))

struct image_resolution {
    int width;
    int height;
};

enum image_format {
    IMAGE_FORMAT_ARGB
};

typedef struct image image_t;
typedef unsigned int image_color_t;

/* create / delete */
image_t *image_create(struct image_resolution *resoltion, enum image_format format);
void image_free(image_t *image);

/* information */
int image_get_width(image_t *image);
int image_get_height(image_t *image);

/* processing functions */
void image_clear(image_t *image, image_color_t color);
void image_set_pixel(image_t *image, int x, int y, image_color_t color);

/* loading and saving */
int image_save(image_t *image, const char *path);

#endif
