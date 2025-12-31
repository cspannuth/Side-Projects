#include "imgops.h"
#include <stdio.h>

_Bool close_to_green(struct pixel* p) {
    if( sqrt(pow(p->g - 255.0,2) + pow((double) p->r,2) + pow((double) p->b,2)) < 100) {
        return 1;
    }
    return 0;
}

void reflect(struct image* image) {
  //Creates variables for easy access to height and width
  int height = image->height;
  int width = image->width;
  //Allocates memory for a new image, and its pixels
  struct image* newImage = malloc(sizeof(struct image));
  newImage->height = height;
  newImage->width = width;

  newImage->pixels = malloc(sizeof(struct pixel) * width * height);
  //Copys the passed in image, over to the new image
  for(int i = 0; i < width * height; i++) {
    newImage->pixels[i] = image->pixels[i];
  }
  //Sets the pixels from the passed in image equal to the reversed column of the new image
  //This gives the reflected effect, by not manipulating rows, and switching the columns
  for(int row = 0; row < height; row++) {
    for(int col = 0; col < width; col++) {
	image->pixels[row * width + col] = newImage->pixels[row * width + (width - 1 - col)];
      }
  }
  //frees the new image after the operation is completed
  free(newImage);
}

struct image* make_pink_gradient(int width, int height) {
  //Allocates memory for the gradient image and its pixels
  struct image* newImage = malloc(sizeof(struct image));
  newImage->height = height;
  newImage->width = width;
  newImage->pixels = malloc(sizeof(struct pixel) * width * height);
  int i = 0;
  //Loops through the pixels by 2d, and increments i until it hits 255, then resets.
  //Sets g = 0, and b = 255 to get the pink gradient effect
  for(int row = 0; row < height; row++) {
    for(int col = 0; col < width; col++) {
    newImage->pixels[row * width + col].r = i;
    newImage->pixels[row * width + col].g = 0;
    newImage->pixels[row * width + col].b = 255;
    if(i == 255)
      i = 0;
    else
      i++;
    }
  }
  //returns the pointer to the background
  return newImage;
}

void greenscreen(struct image* image, struct image* background) {
  //Loops through the pixels by 1d in the memory space
  for(int i = 0; i < image->width * image->height; i++) {
    //Creates a new pixel pointer so that we can utilize close_to_green
    struct pixel* p = malloc(sizeof(struct pixel));
    p->r = image->pixels[i].r;
    p->g = image->pixels[i].g;
    p->b = image->pixels[i].b;
    //If the pixel is close to green, set the image pixel equal to the background pixel
    if(close_to_green(p) == 1)
      image->pixels[i] = background->pixels[i];
    //Free's the pixel before the next iteration
    free(p);
  }
  
}
