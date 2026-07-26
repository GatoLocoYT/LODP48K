#include "hud.h"

#include "renderer.h"

#define HUD_PIXEL_SCALE 2
#define HUD_GLYPH_SIZE 16

#define HUD_TIME_LABEL_X 32
#define HUD_TIME_LABEL_Y 48
#define HUD_TIME_VALUE_X 40
#define HUD_TIME_VALUE_Y 68

#define HUD_SCORE_LABEL_X 24
#define HUD_SCORE_LABEL_Y 108
#define HUD_SCORE_VALUE_X 16
#define HUD_SCORE_VALUE_Y 128

#define HUD_HP_LABEL_X 48
#define HUD_HP_LABEL_Y 168
#define HUD_HP_VALUE_X 40
#define HUD_HP_VALUE_Y 188

#define HUD_MAX_TIME 999
#define HUD_MAX_SCORE 999999
#define HUD_MAX_HP 3

static void Hud_DrawGlyph(
    int x,
    int y,
    int spriteX,
    int spriteY)
{
    Renderer_DrawScreenSprite(
        x,
        y,
        spriteX,
        spriteY,
        HUD_PIXEL_SCALE);
}

static void Hud_DrawDigit(
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

    Hud_DrawGlyph(
        x,
        y,
        spriteX,
        spriteY);
}

static void Hud_DrawNumber(
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
        int digit = (value / divisor) % 10;

        Hud_DrawDigit(
            x + i * HUD_GLYPH_SIZE,
            y,
            digit);

        divisor /= 10;
    }
}

static void Hud_DrawTimeLabel(void)
{
    Hud_DrawGlyph(
        HUD_TIME_LABEL_X,
        HUD_TIME_LABEL_Y,
        6,
        1);

    Hud_DrawGlyph(
        HUD_TIME_LABEL_X + HUD_GLYPH_SIZE,
        HUD_TIME_LABEL_Y,
        7,
        1);

    Hud_DrawGlyph(
        HUD_TIME_LABEL_X + HUD_GLYPH_SIZE * 2,
        HUD_TIME_LABEL_Y,
        0,
        2);

    Hud_DrawGlyph(
        HUD_TIME_LABEL_X + HUD_GLYPH_SIZE * 3,
        HUD_TIME_LABEL_Y,
        1,
        2);
}

static void Hud_DrawScoreLabel(void)
{
    Hud_DrawGlyph(
        HUD_SCORE_LABEL_X,
        HUD_SCORE_LABEL_Y,
        2,
        2);

    Hud_DrawGlyph(
        HUD_SCORE_LABEL_X + HUD_GLYPH_SIZE,
        HUD_SCORE_LABEL_Y,
        3,
        2);

    Hud_DrawGlyph(
        HUD_SCORE_LABEL_X + HUD_GLYPH_SIZE * 2,
        HUD_SCORE_LABEL_Y,
        5,
        1);

    Hud_DrawGlyph(
        HUD_SCORE_LABEL_X + HUD_GLYPH_SIZE * 3,
        HUD_SCORE_LABEL_Y,
        4,
        2);

    Hud_DrawGlyph(
        HUD_SCORE_LABEL_X + HUD_GLYPH_SIZE * 4,
        HUD_SCORE_LABEL_Y,
        1,
        2);
}

static void Hud_DrawHPLabel(void)
{
    Hud_DrawGlyph(
        HUD_HP_LABEL_X,
        HUD_HP_LABEL_Y,
        5,
        2);

    Hud_DrawGlyph(
        HUD_HP_LABEL_X + HUD_GLYPH_SIZE,
        HUD_HP_LABEL_Y,
        6,
        2);
}

void Hud_Draw(
    int time,
    int score,
    int hp)
{
    if (time < 0)
    {
        time = 0;
    }
    else if (time > HUD_MAX_TIME)
    {
        time = HUD_MAX_TIME;
    }

    if (score < 0)
    {
        score = 0;
    }
    else if (score > HUD_MAX_SCORE)
    {
        score = HUD_MAX_SCORE;
    }

    if (hp < 0)
    {
        hp = 0;
    }
    else if (hp > HUD_MAX_HP)
    {
        hp = HUD_MAX_HP;
    }

    Hud_DrawTimeLabel();

    Hud_DrawNumber(
        HUD_TIME_VALUE_X,
        HUD_TIME_VALUE_Y,
        time,
        3);

    Hud_DrawScoreLabel();

    Hud_DrawNumber(
        HUD_SCORE_VALUE_X,
        HUD_SCORE_VALUE_Y,
        score,
        6);

    Hud_DrawHPLabel();

    for (int i = 0; i < hp; i++)
    {
        Hud_DrawGlyph(
            HUD_HP_VALUE_X +
                i * HUD_GLYPH_SIZE,
            HUD_HP_VALUE_Y,
            2,
            3);
    }
}
