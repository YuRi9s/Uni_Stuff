#include "flood_fill.h"

#include <stdbool.h>
#include <stdlib.h>

#include "util.h"

void flood(image_t *img, int x, int y, pixel_t *target_color)
{
    int height = img->h;
    int width = img->w;
    if (x < 0 || y < 0 || x >= width || y >= height)
    {
        return;
    }
    /*Please note that the code was my idea But I had asked ChatGPT multiple times to search and fix the errors and The memoryleaks (because I had many)
    prompt: Q(1)I want you to tell me how to appreach the Flood-Fill Algorithm
    Q(2)Great I understand I want you to give me a way that I point to the starting point to change its values and correct my 4 if statments if they are incorrect
    I also asked If there are memoryleaks here (I know that this is stupid however I had many I (i wanted to know where so I copied all my codes from here and Image_edite.c to search for the leak)*/

    // here we retrieve the pixel at the starting point (x, y) by calculating the index based on the width and height of the image and we assign the address of the pixel to the startColour pointer.
    pixel_t *startColour = &img->img[y * width + x];
    // This condition checks if the color of the starting pixel is different from the target color.
    if (startColour->r != target_color->r || startColour->g != target_color->g || startColour->b != target_color->b)
    {
        // creates a local copy of the starting pixel color
        pixel_t Colour = *startColour;

        // updates the color of the starting pixel to the target color
        startColour->r = target_color->r;
        startColour->g = target_color->g;
        startColour->b = target_color->b;

        // The code checks if the pixel above (x, y - 1) has the same color as the Colour variable.
        if (y - 1 >= 0)
        {
            pixel_t *up = &img->img[(y - 1) * width + x];
            if (up->r == Colour.r && up->g == Colour.g && up->b == Colour.b)
            {
                flood(img, x, y - 1, target_color);
            }
        }

        // The code checks if the pixel below (x, y + 1) has the same color as the Colour variable.
        if (y + 1 < height)
        {
            pixel_t *down = &img->img[(y + 1) * width + x];
            if (down->r == Colour.r && down->g == Colour.g && down->b == Colour.b)
            {
                flood(img, x, y + 1, target_color);
            }
        }

        // The code checks if the pixel to the left (x - 1, y) has the same color as the Colour variable.
        if (x - 1 >= 0)
        {
            pixel_t *left = &img->img[y * width + (x - 1)];
            if (left->r == Colour.r && left->g == Colour.g && left->b == Colour.b)
            {
                flood(img, x - 1, y, target_color);
            }
        }

        // The code checks if the pixel to the right (x + 1, y) has the same color as the Colour variable.
        if (x + 1 < width)
        {
            pixel_t *right = &img->img[y * width + (x + 1)];
            if (right->r == Colour.r && right->g == Colour.g && right->b == Colour.b)
            {
                flood(img, x + 1, y, target_color);
            }
        }
    }
}
