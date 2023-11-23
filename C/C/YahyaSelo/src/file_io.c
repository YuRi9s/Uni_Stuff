#include "file_io.h"
#include <string.h>

#include <assert.h>
#include <stdlib.h>

#include "util.h"

int image_read(image_t *img, FILE *fin)
{
    char magic[3];
    fscanf(fin, "%s", magic);
    if (magic[0] != 'P' || (magic[1] != '3'))
    {
        return -1;
    }
    int max_pixel_value;
    int width;
    int height;
    fscanf(fin, "%d %d %d", &width, &height, &max_pixel_value);
    if (width <= 0 || height <= 0 || max_pixel_value != 255)
    {
        return -1;
    }

    pixel_t *pixel_array;
    pixel_t pixel;
    pixel_array = malloc(width * height * sizeof(pixel_t));
    img->img = pixel_array;
    int r, g, b;
    for (int i = 0; i < width * height; i++)
    {
        if (fscanf(fin, "%d %d %d", &r, &g, &b) != EOF)
        {
            if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
            {
                return -1;
            }
            pixel.r = r;
            pixel.g = g;
            pixel.b = b;
        }
        img->img[i] = pixel;
    }
    img->w = width;
    img->h = height;
    return 0;
}

void image_write(const image_t *img, FILE *fout)
{

    fputs("P3 ", fout);
    fprintf(fout, "%d %d ", img->w, img->h);
    fputs("255", fout);
    for (int i = 0; i < img->w * img->h; i++)
    {
        fprintf(fout, " %d %d %d", img->img[i].r, img->img[i].g, img->img[i].b);
    }
}

void image_free(image_t *img)
{
    if (img->img != NULL)
    {
        free(img->img);
    }
    img->w = img->h = 0;
    img->img = NULL;
}
