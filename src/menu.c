#include "menu.h"

#include "renderer.h"

#define MENU_PIXEL_SCALE 3
#define MENU_GLYPH_SIZE 24

#define MENU_TITLE_X 236
#define MENU_TITLE_Y 144

#define MENU_START_X 260
#define MENU_START_Y 288

static void Menu_DrawGlyph(
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
        MENU_PIXEL_SCALE);
}

static void Menu_DrawTitle(void)
{
    Menu_DrawGlyph(
        MENU_TITLE_X,
        MENU_TITLE_Y,
        4,
        3);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE,
        MENU_TITLE_Y,
        5,
        1);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE * 2,
        MENU_TITLE_Y,
        5,
        3);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE * 3,
        MENU_TITLE_Y,
        6,
        2);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE * 4,
        MENU_TITLE_Y,
        7,
        0);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE * 5,
        MENU_TITLE_Y,
        3,
        1);

    Menu_DrawGlyph(
        MENU_TITLE_X + MENU_GLYPH_SIZE * 6,
        MENU_TITLE_Y,
        6,
        3);
}

static void Menu_DrawStart(void)
{
    Menu_DrawGlyph(
        MENU_START_X,
        MENU_START_Y,
        2,
        2);

    Menu_DrawGlyph(
        MENU_START_X + MENU_GLYPH_SIZE,
        MENU_START_Y,
        6,
        1);

    Menu_DrawGlyph(
        MENU_START_X + MENU_GLYPH_SIZE * 2,
        MENU_START_Y,
        0,
        3);

    Menu_DrawGlyph(
        MENU_START_X + MENU_GLYPH_SIZE * 3,
        MENU_START_Y,
        4,
        2);

    Menu_DrawGlyph(
        MENU_START_X + MENU_GLYPH_SIZE * 4,
        MENU_START_Y,
        6,
        1);
}

void Menu_Draw(void)
{
    Menu_DrawTitle();
    Menu_DrawStart();
}
