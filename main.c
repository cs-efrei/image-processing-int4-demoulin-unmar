#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "bmp8.h"
#include "bmp8.c"
#include "bmp24.h"
#include "bmp24.c"



void printMainMenu() {
    printf("\nPlease choose an option:\n");
    printf("  1. Open an image\n");
    printf("  2. Save an image\n");
    printf("  3. Apply a filter\n");
    printf("  4. Display image information\n");
    printf("  5. Quit\n");
    printf(">>> Your choice: ");
}

void printFilterMenu() {
    printf("\nPlease choose a filter:\n");
    printf("  1. Box Blur\n");
    printf("  2. Gaussian Blur\n");
    printf("  3. Outline\n");
    printf("  4. Emboss\n");
    printf("  5. Sharpen\n");
    printf("  6. Return to main menu\n");
    printf(">>> Your choice: ");
}

int main() {
    t_bmp8 *image = NULL;
    int choice;
    char filename[256];

    do {
        printMainMenu();
        scanf("%d", &choice);
        getchar(); 

        if (choice == 1) {
            printf("Enter image file path: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0'; // remove newline

            // Load image
            image = bmp8_loadImage(filename);
            if (image != NULL) {
                printf("Image loaded successfully!\n");
            } else {
                printf("Error loading image.\n");
            }

        } else if (choice == 2) {
            if (image == NULL) {
                printf("No image loaded.\n");
                continue;
            }

            printf("Enter filename to save as: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0';

            // Save image
            bmp8_saveImage(filename, image);
            printf("Image saved successfully!\n");

        } else if (choice == 3) {
            if (image == NULL) {
                printf("No image loaded.\n");
                continue;
            }

            int filterChoice;
            printFilterMenu();
            scanf("%d", &filterChoice);
            getchar();

            if (filterChoice == 1) {
                bmp8_boxBlur(image);
                printf("Box Blur applied.\n");
            } else if (filterChoice == 2) {
                bmp8_gaussianBlur(image);
                printf("Gaussian Blur applied.\n");
            } else if (filterChoice == 3) {
                bmp8_outline(image);
                printf("Outline filter applied.\n");
            } else if (filterChoice == 4) {
                bmp8_emboss(image);
                printf("Emboss filter applied.\n");
            } else if (filterChoice == 5) {
                bmp8_sharpen(image);
                printf("Sharpen filter applied.\n");
            } else if (filterChoice == 6) {
                continue; // return to main menu
            } else {
                printf("Invalid filter choice.\n");
            }

        } else if (choice == 4) {
            if (image == NULL) {
                printf("No image loaded.\n");
            } else {
                bmp8_printInfo(image);
            }

        } else if (choice == 5) {
            printf("Goodbye!\n");

        } else {
            printf("Invalid option. Try again.\n");
        }

    } while (choice != 5);

    // Free memory
    if (image != NULL) {
        bmp8_free(image);
    }

    return 0;
}

