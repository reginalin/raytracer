#ifndef PPM_EXAMPLE_H
#define PPM_EXAMPLE_H

typedef struct pixel_struct pixel_t;

struct pixel_struct {
    unsigned char r, g, b; // a pixel contains three bytes, name r, g, and b
};  // don't forget the ; at the end of the struct declaration!

typedef struct img_struct {
    pixel_t *data;
    int w, h;
} img_t;

img_t *new_img(int w, int h);
void destroy_img(img_t **img);

img_t *read_ppm(const char *fname);
void  write_ppm(const img_t *img, const char *fname);

void grayscale(img_t *img);
void flip(img_t *img);
void flop(img_t *img);
void transpose(img_t *img);
void boxblur(img_t *img, int n);
void median(img_t *img, int n);
void gaussian(img_t *img, int n, int s);
void sobel(img_t *img);
void sizePPM(img_t *img, int width, int height);
void scale(img_t *img, float factor);

#endif // PPM_EXAMPLE_H
