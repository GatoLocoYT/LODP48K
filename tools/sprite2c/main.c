#include <stdio.h>
#include "../common/writer.h"
#include "../common/bmp.h"

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(
            stderr,
            "Usage:\n"
            "sprite2c <input.bmp>\n");

        return 1;
    }

    Bitmap bmp;

    if (!BMP_Load(argv[1], &bmp))
    {
        return 1;
    }

    if (!Writer_SaveSpriteSheet(
            "assets.h",
            &bmp))
    {
        BMP_Free(&bmp);
        return 1;
    }

    BMP_Free(&bmp);
    return 0;
}
