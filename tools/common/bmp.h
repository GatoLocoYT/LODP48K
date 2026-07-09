#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    int width;
    int height;
    int bitsPerPixel;

    uint32_t* pixels;

} Bitmap;

bool BMP_Load(const char* path, Bitmap* bmp);

void BMP_Free(Bitmap* bmp);

#endif