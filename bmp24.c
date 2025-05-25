#include <stdio.h>
#include <stdlib.h>

#include "bmp24.h"


// Dynamically allocate a pixel matrix of size height x width
t_pixel **bmp24_allocateDataPixels(int width, int height) {
    t_pixel **pixels = malloc(height * sizeof(t_pixel *));
    if (!pixels) {
        fprintf(stderr, "Error: malloc failed for rows\n");
        return NULL;
    }

    for (int i = 0; i < height; i++) {
        pixels[i] = malloc(width * sizeof(t_pixel));
        if (!pixels[i]) {
            fprintf(stderr, "Error: malloc failed for row %d\n", i);
            // Free already allocated rows
            for (int j = 0; j < i; j++) free(pixels[j]);
            free(pixels);
            return NULL;
        }
    }
    return pixels;
}

// Free the pixel matrix memory
void bmp24_freeDataPixels(t_pixel **pixels, int height) {
    if (!pixels) return;
    for (int i = 0; i < height; i++) {
        free(pixels[i]);
    }
    free(pixels);
}

// Allocate a t_bmp24 structure and its pixel matrix
t_bmp24 *bmp24_allocate(int width, int height, int colorDepth) {
    t_bmp24 *img = malloc(sizeof(t_bmp24));
    if (!img) {
        fprintf(stderr, "Error: malloc failed for bmp24\n");
        return NULL;
    }

    img->pixels = bmp24_allocateDataPixels(width, height);
    if (!img->pixels) {
        free(img);
        return NULL;
    }

    img->width = width;
    img->height = height;
    img->colorDepth = colorDepth;

    return img;
}

// Free all memory allocated for the image
void bmp24_free(t_bmp24 *img) {
    if (!img) return;
    bmp24_freeDataPixels(img->pixels, img->height);
    free(img);
}

 void file_rawRead (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
     fseek(file, position, SEEK_SET);
     fread(buffer, size, n, file);
 }

 void file_rawWrite (uint32_t position, void * buffer, uint32_t size, size_t n, FILE * file) {
     fseek(file, position, SEEK_SET);
     fwrite(buffer, size, n, file);
 }


