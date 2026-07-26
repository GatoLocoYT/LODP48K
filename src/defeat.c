#include "defeat.h"

#include <SDL2/SDL.h>
#include <stdint.h>

#include "renderer.h"

#define DEFEAT_PIXEL_SCALE 2
#define DEFEAT_TITLE_SCALE 3

#define DEFEAT_SPACE 255

#define DEFEAT_SELECTOR_VISIBLE_TIME 700
#define DEFEAT_SELECTOR_BLINK_TIME 1000

typedef struct
{
    uint8_t x;
    uint8_t y;
} DefeatGlyph;

static const DefeatGlyph gTitle[] =
{
    {7, 2}, {0, 3}, {0, 2}, {1, 2},
    {DEFEAT_SPACE, DEFEAT_SPACE},
    {5, 1}, {1, 3}, {1, 2}, {4, 2}
};

static const DefeatGlyph gRetry[] =
{
    {4, 2}, {1, 2}, {6, 1}, {4, 2}, {4, 4}
};

static void Defeat_DrawRow(
    int x,
    int y,
    int scale,
    const DefeatGlyph* glyphs,
    int glyphCount)
{
    int glyphSize = 8 * scale;

    for (int i = 0; i < glyphCount; i++)
    {
        if (glyphs[i].x == DEFEAT_SPACE)
        {
            continue;
        }

        Renderer_DrawScreenSprite(
            x + i * glyphSize,
            y,
            glyphs[i].x,
            glyphs[i].y,
            scale);
    }
}

void Defeat_Draw(void)
{
    Defeat_DrawRow(
        212,
        144,
        DEFEAT_TITLE_SCALE,
        gTitle,
        9);

    Defeat_DrawRow(
        280,
        300,
        DEFEAT_PIXEL_SCALE,
        gRetry,
        5);

    if (SDL_GetTicks() %
            DEFEAT_SELECTOR_BLINK_TIME <
        DEFEAT_SELECTOR_VISIBLE_TIME)
    {
        Renderer_DrawScreenSprite(
            248,
            300,
            7,
            3,
            DEFEAT_PIXEL_SCALE);
    }
}
