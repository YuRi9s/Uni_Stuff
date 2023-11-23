#include "image_edit.h"

#include <stdlib.h>

#include "util.h"

void rotate_counterclockwise(image_t *img)
{
    /*Please note that the code was my idea But I had asked ChatGPT multiple times to search and fix the errors and The memoryleaks (because I had many)
    prompt:The following code is not working please correct it and fix any memoryleaks you may find*/
    
    /*This line dynamically allocates memory for a temporary array temp that will hold the rotated image.
   The size of the temp  represents the size of a single pixel in bytes.*/
    pixel_t *temp = malloc(img->w * img->h * sizeof(pixel_t));
    int counter_row = 0;

    // here we swap the pixels from the position of the original picture to the temp array that we allocated
    while (counter_row < img->w)
    {
        int counter_column = 0;
        while (counter_column < img->h)
        {
            temp[counter_row * img->h + counter_column] = img->img[counter_column * img->w + (img->w - counter_row - 1)];
            ++counter_column;
        }
        ++counter_row;
    }

    free(img->img);  // free the malloc that where used to sotre the temp array
    img->img = temp; // assign the rotated image

    int updated_width = img->w;
    img->w = img->h;
    img->h = updated_width;
}

void rotate_clockwise(image_t *img)
{
    pixel_t *temp = malloc(img->w * img->h * sizeof(pixel_t));
    int counter_row = 0;
    // here we swap the pixels from the position of the original picture to the temp array that we allocated
    while (counter_row < img->w)
    {
        int counter_column = 0;
        while (counter_column < img->h)
        {
            temp[counter_row * img->h + counter_column] = img->img[(img->h - counter_column - 1) * img->w + counter_row];
            ++counter_column;
        }
        ++counter_row;
    }

    free(img->img);  // free the malloc that where used to sotre the temp array
    img->img = temp; // assign the rotated image

    int updated_width = img->w;
    img->w = img->h;
    img->h = updated_width;
}

void mirror_horizontal(image_t *img)
{
    pixel_t *temp = malloc(img->w * img->h * sizeof(pixel_t));
    int counter_row = 0;
    // here we swap the pixels from the position of the original picture to the temp array that we allocated
    while (counter_row < img->h)
    {
        int counter_column = 0;
        while (counter_column < img->w)
        {
            temp[counter_row * img->w + counter_column] = img->img[counter_row * img->w + (img->w - counter_column - 1)];
            ++counter_column;
        }
        ++counter_row;
    }

    free(img->img);  // free the malloc that where used to sotre the temp array
    img->img = temp; // assign the rotated image
}

void mirror_vertical(image_t *img)
{
    pixel_t *temp = malloc(img->w * img->h * sizeof(pixel_t));
    int counter_row = 0;
    // here we swap the pixels from the position of the original picture to the temp array that we allocated
    while (counter_row < img->h)
    {
        int counter_column = 0;
        while (counter_column < img->w)
        {
            temp[counter_row * img->w + counter_column] = img->img[(img->h - counter_row - 1) * img->w + counter_column];
            ++counter_column;
        }
        ++counter_row;
    }

    free(img->img);  // free the malloc that where used to sotre the temp array
    img->img = temp; // assign the rotated image
}

void resize(image_t *img, int new_width, int new_height)
{
    pixel_t *temp_array = malloc(new_width * new_height * sizeof(pixel_t));
    pixel_t resized_img;
    int counter_row = 0;
    //The outer loop controls the rows, and the inner loop controls the columns
    //This loop iterates over each row of the resized image. It continues until it  reaches the new height.
    while (counter_row < new_height)
    {
        int counter_column = 0;
        while (counter_column < new_width)
        {
            if ((counter_row >= img->h) || (counter_column >= img->w))
            {
                //we need to pad the resized image with zeros because we're outside the original image boundaries.
                resized_img.r = 0;
                resized_img.g = 0;
                resized_img.b = 0;
                //assigns the black pixel 
                temp_array[(counter_row * new_width) + counter_column] = resized_img;
            }
            else
            {
                //we can directly copy the corresponding pixel value from the original image to the resized image.
                temp_array[(counter_row * new_width) + counter_column] = img->img[(counter_row * img->w) + counter_column];
            }
            ++counter_column;
        }
        ++counter_row;
    }

    free(img->img);        // free the malloc that where used to sotre the temp array
    img->img = temp_array; // assign the rotated image
    img->w = new_width;
    img->h = new_height;
}
