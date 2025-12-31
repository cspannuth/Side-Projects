#include "imgops.h"
#include "loader.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv){

    char* input_filepath;
    char* output_filepath;

    // argv[1] to be the input filepath
    // argv[2] to be the integer width
    // argv[3] to be the integer height
    // argv[4] to be the operation, either "reflect" or "greenscreen"
    // argv[5] to be the output filepath.

    input_filepath = argv[1];
    int width = atoi(argv[2]);
    int height = atoi(argv[3]);
    char* operation = argv[4];
    output_filepath = argv[5];
    
    
    if(argc != 6) {
        printf("Incorrect number of arguments. Expected: ./build/image_calc <input_image> <width> <height> <img operation=reflect,greenscreen> <output_image_path>\n");
        return -1;
    }

    struct image* image = malloc(sizeof(struct image));
    image->height = height;
    image->width = width;
    //Allocates pixels inside the 2d array
    image->pixels = malloc(sizeof(struct pixel) * width * height);
    
    // call load image
    int err = loadimage(input_filepath, image);
    if(err == -1) {
        printf("ERROR loading image. Is your filepath correct?\n");
        return -1;
    }

    struct image* newImage;
    if(strcmp(operation, "reflect") == 0)
      //If reflect operation is called, reflect runs
      reflect(image);
    else if(strcmp(operation, "greenscreen") == 0) {
      //If greenscreen is called, a pink gradient is made and passed into greenscreen with the image
      struct image* newImage = make_pink_gradient(width, height);
      greenscreen(image, newImage);
    }
      // save result to file
    saveimage(output_filepath, image);
    
    //Frees both the malloc'd structs
    free(image);
    free(newImage);
}
