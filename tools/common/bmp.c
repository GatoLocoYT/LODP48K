#include "bmp.h"

#include <stdio.h>
#include <stdlib.h>

static uint16_t ReadU16(FILE *file)
{
    uint8_t b[2];

    fread(b, 1, 2, file);

    return (uint16_t)(b[0] | (b[1] << 8));
}

static uint32_t ReadU32(FILE *file)
{
    uint8_t b[4];

    fread(b, 1, 4, file);

    return (uint32_t)(b[0] |
                      (b[1] << 8) |
                      (b[2] << 16) |
                      (b[3] << 24));
}

bool BMP_Load(const char *path, Bitmap *bmp)
{
    FILE *file = fopen(path, "rb");

    if (file == NULL)
    {
        printf("Error: could not open BMP file: %s\n", path);
        return false;
    }

    uint16_t signature = ReadU16(file);

    if (signature != 0x4D42)
    {
        printf("Error: invalid BMP file.\n");

        fclose(file);

        return false;
    }

    // Offset donde comienzan los píxeles
    fseek(file, 10, SEEK_SET);

    uint32_t pixelOffset = ReadU32(file);

    // Width
    fseek(file, 18, SEEK_SET);
    bmp->width = (int)ReadU32(file);

    // Height
    fseek(file, 22, SEEK_SET);
    bmp->height = (int)ReadU32(file);

    // Bits por píxel
    fseek(file, 28, SEEK_SET);
    bmp->bitsPerPixel = (int)ReadU16(file);

    // Compresión
    fseek(file, 30, SEEK_SET);
    uint32_t compression = ReadU32(file);

    printf("Width       : %d\n", bmp->width);
    printf("Height      : %d\n", bmp->height);
    printf("Bits        : %d\n", bmp->bitsPerPixel);
    printf("Compression : %u\n", compression);

    if (bmp->bitsPerPixel != 32)
    {
        printf("Error: only 32-bit BMP files are supported.\n");

        fclose(file);

        return false;
    }

    if (compression != 0 && compression != 3)
    {
        printf(
            "Error: unsupported BMP compression mode (%u).\n",
            compression);

        fclose(file);

        return false;
    }

    size_t pixelCount = (size_t)bmp->width * bmp->height;

    bmp->pixels = malloc(pixelCount * sizeof(uint32_t));

    if (bmp->pixels == NULL)
    {
        printf("Error: out of memory.\n");

        fclose(file);

        return false;
    }

    fseek(file, pixelOffset, SEEK_SET);

    size_t pixelsRead = fread(
        bmp->pixels,
        sizeof(uint32_t),
        pixelCount,
        file);

    if (pixelsRead != pixelCount)
    {
        printf("Error: could not read pixel data.\n");

        BMP_Free(bmp);

        fclose(file);

        return false;
    }

    fclose(file);

    return true;
}

void BMP_Free(Bitmap *bmp)
{
    if (bmp->pixels != NULL)
    {
        free(bmp->pixels);
        bmp->pixels = NULL;
    }
}