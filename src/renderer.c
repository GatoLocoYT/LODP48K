#include "renderer.h"

#include <SDL2/SDL.h>

#define SPRITE_SIZE 8
#define SPRITESHEET_WIDTH 64

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define ROOM_OFFSET_X 128
#define ROOM_OFFSET_Y 48

#define PIXEL_SCALE 3

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
            ROOM_OFFSET_X + x * PIXEL_SCALE,
            ROOM_OFFSET_Y + y * PIXEL_SCALE,
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
    Renderer_DrawSpriteEx(
        screenX,
        screenY,
        spriteX,
        spriteY,
        DIR_RIGHT,
        spriteSheet);
}

void Renderer_DrawSpriteEx(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    Direction direction,
    const uint32_t *spriteSheet)
{
    int startX = spriteX * SPRITE_SIZE;
    int startY = spriteY * SPRITE_SIZE;

    for (int y = 0; y < SPRITE_SIZE; y++)
    {
        for (int x = 0; x < SPRITE_SIZE; x++)
        {
            int sourceX = x;
            int sourceY = y;

            switch (direction)
            {
            case DIR_RIGHT:
                sourceX = x;
                sourceY = y;
                break;

            case DIR_LEFT:
                sourceX = SPRITE_SIZE - 1 - x;
                sourceY = y;
                break;

            case DIR_UP:
                sourceX = SPRITE_SIZE - 1 - y;
                sourceY = x;
                break;

            case DIR_DOWN:
                sourceX = y;
                sourceY = SPRITE_SIZE - 1 - x;
                break;
            }

            int sheetIndex =
                (startY + sourceY) * SPRITESHEET_WIDTH +
                (startX + sourceX);

            uint32_t color = spriteSheet[sheetIndex];

            Renderer_DrawPixel(
                screenX * SPRITE_SIZE + x,
                screenY * SPRITE_SIZE + y,
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