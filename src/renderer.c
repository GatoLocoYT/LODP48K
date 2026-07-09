#include "renderer.h"

#include <SDL2/SDL.h>
#define SPRITE_SIZE 8
#define SPRITESHEET_WIDTH 64
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define PIXEL_SCALE 8

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;

bool Renderer_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return false;
    }

    gWindow = SDL_CreateWindow(
        "LODP48K",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_SHOWN);

    if (gWindow == NULL)
    {
        SDL_Quit();
        return false;
    }

    gRenderer = SDL_CreateRenderer(
        gWindow,
        -1,
        SDL_RENDERER_ACCELERATED);

    if (gRenderer == NULL)
    {
        SDL_DestroyWindow(gWindow);
        SDL_Quit();

        return false;
    }

    return true;
}

void Renderer_Clear(void)
{
    SDL_SetRenderDrawColor(
        gRenderer,
        0,
        0,
        0,
        255);

    SDL_RenderClear(gRenderer);
}

void Renderer_DrawPixel(
    int x,
    int y,
    uint32_t color)
{
    uint8_t a = (color >> 24) & 0xFF;
    uint8_t r = (color >> 16) & 0xFF;
    uint8_t g = (color >> 8) & 0xFF;
    uint8_t b = color & 0xFF;

    if (a == 0)
    {
        return;
    }

    SDL_Rect pixel =
        {
            x * PIXEL_SCALE,
            y * PIXEL_SCALE,
            PIXEL_SCALE,
            PIXEL_SCALE};

    SDL_SetRenderDrawColor(
        gRenderer,
        r,
        g,
        b,
        a);

    SDL_RenderFillRect(
        gRenderer,
        &pixel);
}
void Renderer_DrawSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    const uint32_t *spriteSheet)
{
    int startX = spriteX * SPRITE_SIZE;
    int startY = spriteY * SPRITE_SIZE;

    for (int y = 0; y < SPRITE_SIZE; y++)
    {
        for (int x = 0; x < SPRITE_SIZE; x++)
        {
            int sheetIndex =
                (startY + y) * SPRITESHEET_WIDTH +
                (startX + x);

            uint32_t color = spriteSheet[sheetIndex];

            // if (color != 0)
            // {
            //     printf(
            //         "(%d,%d) -> %08X\n",
            //         x,
            //         y,
            //         color);
            // }

            Renderer_DrawPixel(
                screenX + x,
                screenY + y,
                color);
        }
    }
}
void Renderer_Present(void)
{
    SDL_RenderPresent(gRenderer);
}

void Renderer_Quit(void)
{
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);

    SDL_Quit();
}