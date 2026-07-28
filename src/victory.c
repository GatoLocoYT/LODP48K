#include "victory.h"

#include <SDL2/SDL.h>
#include <stdint.h>

#include "renderer.h"

#define VICTORY_PIXEL_SCALE 2
#define VICTORY_GLYPH_SIZE 16

#define VICTORY_TITLE_SCALE 3

#define VICTORY_MAX_TIME 999
#define VICTORY_MAX_SCORE 999999

#define VICTORY_SPACE 255

#define VICTORY_SELECTOR_VISIBLE_TIME 700
#define VICTORY_SELECTOR_BLINK_TIME 1000

typedef struct
{
    uint8_t x;
    uint8_t y;
} VictoryGlyph;

static const VictoryGlyph gTitle[] =
{
    {4, 4}, {5, 1}, {2, 4},
    {VICTORY_SPACE, VICTORY_SPACE},
    {3, 4}, {7, 1}, {1, 4}, {5, 4}
};

static const VictoryGlyph gScore[] =
{
    {2, 2}, {3, 2}, {5, 1}, {4, 2}, {1, 2}
};

static const VictoryGlyph gTime[] =
{
    {6, 1}, {7, 1}, {0, 2}, {1, 2}
};

static const VictoryGlyph gRetry[] =
{
    {4, 2}, {1, 2}, {6, 1}, {4, 2}, {4, 4}
};

static const VictoryGlyph gExit[] =
{
    {1, 2}, {0, 4}, {7, 1}, {6, 1}
};

static void Victory_DrawRow(
    int x,
    int y,
    int scale,
    const VictoryGlyph* glyphs,
    int glyphCount)
{
    int glyphSize = 8 * scale;

    for (int i = 0; i < glyphCount; i++)
    {
        if (glyphs[i].x == VICTORY_SPACE)
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

static void Victory_DrawDigit(
    int x,
    int y,
    int digit)
{
    int spriteX;
    int spriteY;

    if (digit == 0)
    {
        spriteX = 5;
        spriteY = 1;
    }
    else
    {
        int spriteIndex = digit + 3;

        spriteX = spriteIndex % 8;
        spriteY = spriteIndex / 8;
    }

    Renderer_DrawScreenSprite(
        x,
        y,
        spriteX,
        spriteY,
        VICTORY_PIXEL_SCALE);
}

static void Victory_DrawNumber(
    int x,
    int y,
    int value,
    int digits)
{
    int divisor = 1;

    for (int i = 1; i < digits; i++)
    {
        divisor *= 10;
    }

    for (int i = 0; i < digits; i++)
    {
        Victory_DrawDigit(
            x + i * VICTORY_GLYPH_SIZE,
            y,
            (value / divisor) % 10);

        divisor /= 10;
    }
}

static void Victory_DrawSelector(
    VictoryOption selectedOption)
{
    if (SDL_GetTicks() %
            VICTORY_SELECTOR_BLINK_TIME >=
        VICTORY_SELECTOR_VISIBLE_TIME)
    {
        return;
    }

    int x = selectedOption ==
                VICTORY_OPTION_RETRY
                ? 248
                : 256;
    int y = selectedOption ==
                VICTORY_OPTION_RETRY
                ? 320
                : 360;

    Renderer_DrawScreenSprite(
        x,
        y,
        7,
        3,
        VICTORY_PIXEL_SCALE);
}

void Victory_Draw(
    int elapsedTime,
    int score,
    VictoryOption selectedOption)
{
    if (elapsedTime < 0)
    {
        elapsedTime = 0;
    }
    else if (elapsedTime > VICTORY_MAX_TIME)
    {
        elapsedTime = VICTORY_MAX_TIME;
    }

    if (score < 0)
    {
        score = 0;
    }
    else if (score > VICTORY_MAX_SCORE)
    {
        score = VICTORY_MAX_SCORE;
    }

    Victory_DrawRow(
        224,
        112,
        VICTORY_TITLE_SCALE,
        gTitle,
        8);

    Victory_DrawRow(
        224,
        208,
        VICTORY_PIXEL_SCALE,
        gScore,
        5);
    Victory_DrawNumber(
        320,
        208,
        score,
        6);

    Victory_DrawRow(
        256,
        248,
        VICTORY_PIXEL_SCALE,
        gTime,
        4);
    Victory_DrawNumber(
        336,
        248,
        elapsedTime,
        3);

    Victory_DrawRow(
        280,
        320,
        VICTORY_PIXEL_SCALE,
        gRetry,
        5);

    Victory_DrawRow(
        288,
        360,
        VICTORY_PIXEL_SCALE,
        gExit,
        4);

    Victory_DrawSelector(selectedOption);
}
