#include <stdio.h>
#include "../common/writer.h"
#include "../common/bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage:\n");
        printf("sprite2c <input.bmp>\n");

        return 1;
    }

    Bitmap bmp;

    if (!BMP_Load(argv[1], &bmp))
    {
        return 1;
    }

    printf("BMP Info\n");
    printf("Width : %d\n", bmp.width);
    printf("Height: %d\n", bmp.height);
    printf("Bits  : %d\n", bmp.bitsPerPixel);

    printf("\n");

    if (!Writer_SaveSpriteSheet(
            "assets.h",
            &bmp))
    {
        BMP_Free(&bmp);
        return 1;
    }

    printf("\nassets.h generated successfully.\n");

    printf("\n");

    size_t pixelCount = (size_t)bmp.width * bmp.height;

    int found = 0;

    for (size_t i = 0; i < pixelCount; i++)
    {
        if (bmp.pixels[i] != 0)
        {
            printf(
                "Pixel %zu = %08X\n",
                i,
                bmp.pixels[i]);

            found++;

            if (found == 20)
                break;
        }
    }

    printf("Found %d non-zero pixels.\n", found);

    return 0;
}