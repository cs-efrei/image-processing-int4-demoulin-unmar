#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "bmp8.h"


t_bmp8* bmp8_loadImage(const char *filename) {
    // Open the file in binary read mode
    FILE *file = fopen(filename, "rb");
    if (!file) {
        fprintf(stderr, "Error: unable to open file %s\n", filename);
        return NULL;
    }

    // Allocate memory for the image structure
    t_bmp8 *image = (t_bmp8*) malloc(sizeof(t_bmp8));
    if (!image) {
        fprintf(stderr, "Error: memory allocation failed\n");
        fclose(file);
        return NULL;
    }

    // Read the 54-byte BMP header
    if (fread(image->header, 1, 54, file) != 54) {
        fprintf(stderr, "Error: reading header failed\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Extract image width, height, color depth, and data size from header
    image->width = *(unsigned int*)(&image->header[18]);
    image->height = *(unsigned int*)(&image->header[22]);
    image->colorDepth = (unsigned int)image->header[28];
    image->dataSize = *(unsigned int*)(&image->header[34]);

    // Check if image is 8-bit grayscale
    if (image->colorDepth != 8) {
        fprintf(stderr, "Error: image is not 8-bit (colorDepth=%u)\n", image->colorDepth);
        free(image);
        fclose(file);
        return NULL;
    }

    // Read the 1024-byte color table (palette)
    if (fread(image->colorTable, 1, 1024, file) != 1024) {
        fprintf(stderr, "Error: reading color table failed\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Allocate memory for the image pixel data
    image->data = (unsigned char*) malloc(image->dataSize);
    if (!image->data) {
        fprintf(stderr, "Error: memory allocation for image data failed\n");
        free(image);
        fclose(file);
        return NULL;
    }

    // Read the pixel data
    if (fread(image->data, 1, image->dataSize, file) != image->dataSize) {
        fprintf(stderr, "Error: reading image data failed\n");
        free(image->data);
        free(image);
        fclose(file);
        return NULL;
    }

    // Close the file
    fclose(file);

    // Return pointer to the loaded image structure
    return image;
    }

    int bmp8_saveImage(const char *filename, const t_bmp8 *image) {
        // Open the file in binary write mode
        FILE *file = fopen(filename, "wb");
        if (!file) {
            fprintf(stderr, "Error: unable to open file %s for writing\n", filename);
            return -1;
        }

        // Write the 54-byte header to the file
        if (fwrite(image->header, 1, 54, file) != 54) {
            fprintf(stderr, "Error: writing header failed\n");
            fclose(file);
            return -1;
        }

        // Write the 1024-byte color table (palette)
        if (fwrite(image->colorTable, 1, 1024, file) != 1024) {
            fprintf(stderr, "Error: writing color table failed\n");
            fclose(file);
            return -1;
        }

        // Write the pixel data (dataSize bytes)
        if (fwrite(image->data, 1, image->dataSize, file) != image->dataSize) {
            fprintf(stderr, "Error: writing image data failed\n");
            fclose(file);
            return -1;
        }

        fclose(file);

        return 0;
    }

    void bmp8_free(t_bmp8 *image) {
        if (image != NULL) {
            // Free the pixel data memory
            if (image->data != NULL) {
                free(image->data);
            }
            // Free the structure itself
            free(image);
        }
    }

    void bmp8_printInfo(const t_bmp8 *image) {
        if (image == NULL) {
            printf("No image data to display.\n");
            return;
        }

        printf("Image Info:\n");
        printf("  Width: %u\n", image->width);
        printf("  Height: %u\n", image->height);
        printf("  Color Depth: %u\n", image->colorDepth);
        printf("  Data Size: %u\n", image->dataSize);
    }

// Apply a convolution filter to an 8-bit grayscale image
void bmp8_applyFilter(t_bmp8 *img, float **kernel, int kernelSize) {
    if (img == NULL || img->data == NULL) {
        printf("Error: No image loaded.\n");
        return;
    }

    int width = img->width;
    int height = img->height;
    int n = kernelSize / 2; 

    // Allocate memory for a temporary copy of the image
    unsigned char *copy = malloc(img->dataSize);
    if (copy == NULL) {
        printf("Error: Failed to allocate memory for image copy.\n");
        return;
    }

    // Loop through each pixel, excluding the border pixels
    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {

            float sum = 0.0; // Result of convolution for current pixel

            // Loop through the kernel
            for (int i = -n; i <= n; i++) {
                for (int j = -n; j <= n; j++) {
                    int neighbor_x = x + j;
                    int neighbor_y = y + i;
                    int index = neighbor_y * width + neighbor_x;

                    unsigned char pixelValue = img->data[index];
                    float kernelWeight = kernel[i + n][j + n];

                    sum += pixelValue * kernelWeight;
                }
            }

            // Clamp the value to stay within 0–255
            if (sum < 0) sum = 0;
            if (sum > 255) sum = 255;

            // Store the result in the temporary copy
            int currentIndex = y * width + x;
            copy[currentIndex] = (unsigned char)round(sum);
        }
    }

    // Copy the filtered result back to the original image
    for (int y = n; y < height - n; y++) {
        for (int x = n; x < width - n; x++) {
            int index = y * width + x;
            img->data[index] = copy[index];
        }
    }

    // Free the temporary memory
    free(copy);

    printf("Filter applied successfully to the grayscale image.\n");
}

// Apply Box Blur filter
void bmp8_boxBlur(t_bmp8 *img) {
    // Allocate the  kernel
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0; i < 3; i++) {
        kernel[i] = malloc(3 * sizeof(float));
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = 1.0 / 9.0; 
        }
    }
    // Apply the filter
    bmp8_applyFilter(img, kernel, 3);

    // Free the kernel
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

void bmp8_gaussianBlur(t_bmp8 *img){
    //Alocate the kernel
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0 ; i < 3 ; i++){
        kernel[i] = malloc(3 * sizeof(float));

    }

    float gaussianBlurValues[3][3] = {
    {1.0/16, 2.0/16, 1.0/16},
    {2.0/16, 4.0/16, 2.0/16},
    {1.0/16, 2.0/16, 1.0/16}
    };

    // Copy the static matrix to dynamic kernel
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = gaussianBlurValues[i][j];
        }
    }

    // Apply the filter
    bmp8_applyFilter(img, kernel, 3);

    // Free the kernel
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

void bmp8_outline(t_bmp8 * img){
    //Alocate the kernel
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0 ; i < 3 ; i++){
        kernel[i] = malloc(3 * sizeof(float));

    }

    float outlineValues[3][3] = {
    {-1, -1, -1},
    {-1,  8, -1},
    {-1, -1, -1}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = outlineValues[i][j];
        }
    }
    
    // Apply the filter
    bmp8_applyFilter(img, kernel, 3);

    // Free the kernel
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}

void bmp8_emboss(t_bmp8 * img){
    //Alocate the kernel
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0 ; i < 3 ; i++){
        kernel[i] = malloc(3 * sizeof(float));

    }

    float embossValues[3][3] = {
    { -2, -1,  0 },
    { -1,  1,  1 },
    {  0,  1,  2 }
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = embossValues[i][j];
        }
    }
    
    // Apply the filter
    bmp8_applyFilter(img, kernel, 3);

    // Free the kernel
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}


void bmp8_sharpen(t_bmp8 * img){
    //Alocate the kernel
    float **kernel = malloc(3 * sizeof(float *));
    for (int i = 0 ; i < 3 ; i++){
        kernel[i] = malloc(3 * sizeof(float));

    }

    float sharpenValues[3][3] = {
    { 0, -1,  0},
    {-1,  5, -1},
    { 0, -1,  0}
    };

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            kernel[i][j] = sharpenValues[i][j];
        }
    }


    // Apply the filter
    bmp8_applyFilter(img, kernel, 3);

    // Free the kernel
    for (int i = 0; i < 3; i++) {
        free(kernel[i]);
    }
    free(kernel);
}
