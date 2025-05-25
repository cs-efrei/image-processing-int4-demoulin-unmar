#ifndef MAIN_H
#define MAIN_H
#include "data_types.h"

t_bmp8* bmp8_loadImage(const char *filename);
int bmp8_saveImage(const char *filename, const t_bmp8 *image);
void bmp8_free(t_bmp8 *image);
void bmp8_printInfo(const t_bmp8 *image);
void bmp8_negative(t_bmp8 * img);

#endif //MAIN_H
