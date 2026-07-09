#ifndef RENDERER_H
#define RENDERER_H

#include <stdbool.h>
#include <stdint.h>

bool Renderer_Init(void);

void Renderer_Clear(void);

void Renderer_DrawPixel(
    int x,
    int y,
    uint32_t color
);

void Renderer_DrawSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    const uint32_t* spriteSheet
);

void Renderer_Present(void);

void Renderer_Quit(void);

#endif