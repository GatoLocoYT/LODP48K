#include "writer.h"

#include <stdio.h>

bool Writer_SaveSpriteSheet(
    const char* filename,
    const Bitmap* bmp)
{
    FILE* file = fopen(filename, "w");

    if (file == NULL)
    {
        printf("Error: could not create %s\n", filename);
        return false;
    }

    fprintf(file, "#ifndef ASSETS_H\n");
    fprintf(file, "#define ASSETS_H\n\n");

    fprintf(file, "#include <stdint.h>\n\n");

    fprintf(file, "#define SPRITESHEET_WIDTH %d\n", bmp->width);
    fprintf(file, "#define SPRITESHEET_HEIGHT %d\n\n", bmp->height);

    fprintf(file,
        "static const uint32_t gSpriteSheet[] =\n{\n");

    int pixelCount = bmp->width * bmp->height;

    for (int y = 0; y < bmp->height; y++)
    {
        fprintf(file, "    // Row %d\n    ", y);

        for (int x = 0; x < bmp->width; x++)
        {
            int index = y * bmp->width + x;

            fprintf(file, "0x%08X", bmp->pixels[index]);

            if (index != pixelCount - 1)
                fprintf(file, ", ");

            if (x != bmp->width - 1)
                fprintf(file, " ");
        }

        fprintf(file, "\n\n");
    }

    fprintf(file, "};\n\n");

    fprintf(file, "#endif\n");

    fclose(file);

    return true;
}