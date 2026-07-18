#include "enemy.h"

#include <stdlib.h>

#include "room.h"
#include "renderer.h"

#include "../tools/generated/assets.h"

static bool gEnemyAlive = false;

static int gEnemyX = 0;
static int gEnemyY = 0;

static int gEnemyHP = 3;

bool Enemy_Init(void)
{
    Enemy_Spawn();

    return true;
}

void Enemy_Spawn(void)
{
    do
    {
        gEnemyX = rand() % ROOM_WIDTH;
        gEnemyY = rand() % ROOM_HEIGHT;

    } while (!Room_IsWalkable(
        gEnemyX,
        gEnemyY));

    gEnemyHP = 3;
    gEnemyAlive = true;
}

void Enemy_Update(void)
{
    /* Todavía no hace nada */
}

void Enemy_Draw(void)
{
    if (!gEnemyAlive)
    {
        return;
    }

    Renderer_DrawSpriteEx(
        gEnemyX,
        gEnemyY,
        1,      // sprite del enemigo
        0,
        DIR_RIGHT,
        gSpriteSheet);
}

bool Enemy_IsAlive(void)
{
    return gEnemyAlive;
}

int Enemy_GetX(void)
{
    return gEnemyX;
}

int Enemy_GetY(void)
{
    return gEnemyY;
}