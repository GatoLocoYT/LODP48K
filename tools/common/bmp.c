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

    // 0: Signature
    uint16_t signature = ReadU16(file);
    if (signature != 0x4D42) // "BM"
    {
        printf("Error: invalid BMP file.\n");
        fclose(file);
        return false;
    }

    // Leer campos del File Header secuencialmente
    fseek(file, 10, SEEK_SET);
    uint32_t pixelOffset = ReadU32(file);

    // Leer campos del Info Header secuencialmente
    fseek(file, 18, SEEK_SET);
    bmp->width = (int)ReadU32(file);
    
    // El height puede ser negativo (Top-Down) o positivo (Bottom-Up)
    int32_t rawHeight = (int32_t)ReadU32(file); 
    bool isBottomUp = true;

    if (rawHeight < 0)
    {
        isBottomUp = false;
        bmp->height = -rawHeight; // Convertir a positivo para la estructura
    }
    else
    {
        bmp->height = rawHeight;
    }

    fseek(file, 28, SEEK_SET);
    bmp->bitsPerPixel = (int)ReadU16(file);
    uint32_t compression = ReadU32(file);

    printf("Width       : %d\n", bmp->width);
    printf("Height      : %d\n", bmp->height);
    printf("Bits        : %d\n", bmp->bitsPerPixel);
    printf("Compression : %u\n", compression);
    printf("Format      : %s\n", isBottomUp ? "Bottom-Up (Inverted)" : "Top-Down (Normal)");

    if (bmp->bitsPerPixel != 32)
    {
        printf("Error: only 32-bit BMP files are supported.\n");
        fclose(file);
        return false;
    }

    if (compression != 0 && compression != 3)
    {
        printf("Error: unsupported BMP compression mode (%u).\n", compression);
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

    // Ir al inicio de los píxeles (salta automáticamente BI_BITFIELDS si existieran)
    fseek(file, pixelOffset, SEEK_SET);

    if (isBottomUp)
    {
        // Si es Bottom-Up, leemos el archivo fila por fila, 
        // pero las acomodamos en memoria desde la última fila hacia la primera.
        for (int y = bmp->height - 1; y >= 0; y--)
        {
            size_t rowOffset = (size_t)y * bmp->width;
            size_t pixelsRead = fread(&bmp->pixels[rowOffset], sizeof(uint32_t), bmp->width, file);
            
            if (pixelsRead != (size_t)bmp->width)
            {
                printf("Error: could not read pixel data at row %d.\n", y);
                BMP_Free(bmp);
                fclose(file);
                return false;
            }
        }
    }
    else
    {
        // Si ya es Top-Down, lo leemos de golpe de forma normal
        size_t pixelsRead = fread(bmp->pixels, sizeof(uint32_t), pixelCount, file);
        if (pixelsRead != pixelCount)
        {
            printf("Error: could not read pixel data.\n");
            BMP_Free(bmp);
            fclose(file);
            return false;
        }
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