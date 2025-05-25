#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "data_types.h"
#include "prototypes.h"


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

    if (image->dataSize == 0) {
        image->dataSize = image->width * image->height; // pour 8-bit
    }

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


void bmp8_negative(t_bmp8 * img){

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = 255 - img->data[i];
    }
}

void bmp8_brightness(t_bmp8 * img, int value){

    for (unsigned int i = 0; i < img->dataSize; i++) {
        img->data[i] = img->data[i] + value;
    }
}



int main() {
    const char *inputFilename = "barbara_gray.bmp";

    const char *outputFilename = "modified_gray.bmp";

    t_bmp8 *image = bmp8_loadImage(inputFilename);
    if (image == NULL) {
        printf("Failed to load image\n");
        return 1;
    }

    //Shows infos of image
    bmp8_printInfo(image);

    //Apply the negative filter on the image
    bmp8_negative(image);

    if (bmp8_saveImage(outputFilename, image) != 0) {
        return 1;
    }

    printf("Successfully saved image under the name: %s\n", outputFilename);

    bmp8_free(image);

    return 0;
}
