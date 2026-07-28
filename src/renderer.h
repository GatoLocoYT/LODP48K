#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <stdint.h>

#include "direction.h"

bool Renderer_Init(void);

void Renderer_Clear(void);

void Renderer_DrawPixel(
    int x,
    int y,
    uint32_t color
);

void Renderer_DrawAssetSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY
);

void Renderer_DrawAssetSpriteEx(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    Direction direction
);

void Renderer_DrawAssetSpriteMirrored(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    bool mirrored
);

void Renderer_DrawScreenSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    int scale
);

void Renderer_Present(void);

void Renderer_Quit(void);

#endif
