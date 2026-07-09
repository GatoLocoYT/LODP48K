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

void Renderer_Present(void);

void Renderer_Quit(void);

#endif