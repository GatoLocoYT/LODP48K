#ifndef WRITER_H
#define WRITER_H

#include <stdbool.h>

#include "bmp.h"

bool Writer_SaveSpriteSheet(
    const char* filename,
    const Bitmap* bmp
);

#endif