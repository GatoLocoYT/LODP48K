#include "renderer.h"

#include <SDL2/SDL.h>

#include "../tools/generated/assets.h"

#define SPRITE_SIZE 8
#define SPRITESHEET_WIDTH 64

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

#define ROOM_OFFSET_X 128
#define ROOM_OFFSET_Y 48

#define PIXEL_SCALE 3

static SDL_Window *gWindow = NULL;
static SDL_Renderer *gRenderer = NULL;

static void Renderer_DrawSpriteInternal(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    Direction direction,
    bool mirrored)
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
                case DIR_RIGHT: break;
                case DIR_LEFT:
                    sourceX = SPRITE_SIZE - 1 - x;
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

            if (mirrored)
            {
                sourceX = SPRITE_SIZE - 1 - sourceX;
            }

            uint32_t color =
                gSpriteSheet[
                    (startY + sourceY) *
                        SPRITESHEET_WIDTH +
                    startX + sourceX];

            Renderer_DrawPixel(
                screenX * SPRITE_SIZE + x,
                screenY * SPRITE_SIZE + y,
                color);
        }
    }
}

bool Renderer_Init(void)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        return false;
    }

    gWindow = SDL_CreateWindow(
        "La Orden del Plan 48K",
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

    uint32_t iconPixels[SPRITE_SIZE * SPRITE_SIZE];

    for (int y = 0; y < SPRITE_SIZE; y++)
    {
        for (int x = 0; x < SPRITE_SIZE; x++)
        {
            iconPixels[y * SPRITE_SIZE + x] =
                gSpriteSheet[
                    y * SPRITESHEET_WIDTH + x];
        }
    }

    SDL_Surface *icon =
        SDL_CreateRGBSurfaceFrom(
            iconPixels,
            SPRITE_SIZE,
            SPRITE_SIZE,
            32,
            SPRITE_SIZE * sizeof(uint32_t),
            0x00FF0000,
            0x0000FF00,
            0x000000FF,
            0xFF000000);

    if (icon != NULL)
    {
        SDL_SetWindowIcon(
            gWindow,
            icon);

        SDL_FreeSurface(icon);
    }

    return true;
}

void Renderer_Clear(void)
{
    SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
    SDL_RenderClear(gRenderer);
}

void Renderer_DrawPixel(int x, int y, uint32_t color)
{
    uint8_t a =
        (uint8_t)((color >> 24) & 0xFF);
    uint8_t r =
        (uint8_t)((color >> 16) & 0xFF);
    uint8_t g =
        (uint8_t)((color >> 8) & 0xFF);
    uint8_t b =
        (uint8_t)(color & 0xFF);

    if (a == 0)
    {
        return;
    }

    SDL_Rect pixel =
    {
        ROOM_OFFSET_X + x * PIXEL_SCALE,
        ROOM_OFFSET_Y + y * PIXEL_SCALE,
        PIXEL_SCALE,
        PIXEL_SCALE
    };

    SDL_SetRenderDrawColor(gRenderer, r, g, b, a);
    SDL_RenderFillRect(gRenderer, &pixel);
}

void Renderer_DrawAssetSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY)
{
    Renderer_DrawSpriteInternal(
        screenX,
        screenY,
        spriteX,
        spriteY,
        DIR_RIGHT,
        false);
}

void Renderer_DrawAssetSpriteEx(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    Direction direction)
{
    Renderer_DrawSpriteInternal(
        screenX,
        screenY,
        spriteX,
        spriteY,
        direction,
        false);
}

void Renderer_DrawAssetSpriteMirrored(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    bool mirrored)
{
    Renderer_DrawSpriteInternal(
        screenX,
        screenY,
        spriteX,
        spriteY,
        DIR_RIGHT,
        mirrored);
}

void Renderer_DrawScreenSprite(
    int screenX,
    int screenY,
    int spriteX,
    int spriteY,
    int scale)
{
    if (scale <= 0)
    {
        return;
    }

    int startX = spriteX * SPRITE_SIZE;
    int startY = spriteY * SPRITE_SIZE;

    for (int y = 0; y < SPRITE_SIZE; y++)
    {
        for (int x = 0; x < SPRITE_SIZE; x++)
        {
            uint32_t color =
                gSpriteSheet[
                    (startY + y) *
                        SPRITESHEET_WIDTH +
                    startX + x];

            uint8_t a =
                (uint8_t)((color >> 24) & 0xFF);

            if (a == 0)
            {
                continue;
            }

            uint8_t r =
                (uint8_t)((color >> 16) & 0xFF);
            uint8_t g =
                (uint8_t)((color >> 8) & 0xFF);
            uint8_t b =
                (uint8_t)(color & 0xFF);

            SDL_Rect pixel =
            {
                screenX + x * scale,
                screenY + y * scale,
                scale,
                scale
            };

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
