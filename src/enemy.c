#include "enemy.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "player.h"
#include "room.h"
#include "renderer.h"

#include "../tools/generated/assets.h"

#define ENEMY_POOL_SIZE 5

#define ENEMY_HP 3

#define ENEMY_FLASH_DURATION 160
#define ENEMY_FLASH_INTERVAL 40

typedef struct
{
    bool alive;
    bool flashing;

    int x;
    int y;

    int hp;

    Uint32 lastHit;

} Enemy;

static Enemy gEnemies[ENEMY_POOL_SIZE];

static int Enemy_GetSpawnCount(void)
{
    int roll = rand() % 100;

    if (roll < 25)
    {
        return 1;
    }

    if (roll < 55)
    {
        return 2;
    }

    if (roll < 80)
    {
        return 3;
    }

    if (roll < 95)
    {
        return 4;
    }

    return 5;
}

static bool Enemy_CanSpawnAt(
    int x,
    int y,
    int enemyIndex)
{
    if (!Room_IsWalkable(x, y))
    {
        return false;
    }

    if (x == Player_GetX() &&
        y == Player_GetY())
    {
        return false;
    }

    for (int i = 0; i < enemyIndex; i++)
    {
        if (gEnemies[i].x == x &&
            gEnemies[i].y == y)
        {
            return false;
        }
    }

    return true;
}

bool Enemy_Init(void)
{
    Enemy_Spawn();

    return true;
}

void Enemy_Spawn(void)
{
    int enemyCount = Enemy_GetSpawnCount();

    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        gEnemies[i].alive = false;
    }

    for (int i = 0; i < enemyCount; i++)
    {
        Enemy* enemy = &gEnemies[i];

        do
        {
            enemy->x = rand() % ROOM_WIDTH;
            enemy->y = rand() % ROOM_HEIGHT;

        } while (!Enemy_CanSpawnAt(
            enemy->x,
            enemy->y,
            i));

        enemy->hp = ENEMY_HP;
        enemy->alive = true;
        enemy->flashing = false;
    }
}

void Enemy_Update(void)
{
    Uint32 now = SDL_GetTicks();

    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* enemy = &gEnemies[i];

        if (!enemy->alive ||
            !enemy->flashing)
        {
            continue;
        }

        if (now - enemy->lastHit <
            ENEMY_FLASH_DURATION)
        {
            continue;
        }

        enemy->flashing = false;

        if (enemy->hp <= 0)
        {
            enemy->alive = false;
        }
    }
}

void Enemy_Draw(void)
{
    Uint32 now = SDL_GetTicks();

    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* enemy = &gEnemies[i];

        if (!enemy->alive)
        {
            continue;
        }

        if (enemy->flashing &&
            ((now - enemy->lastHit) /
             ENEMY_FLASH_INTERVAL) % 2 == 0)
        {
            continue;
        }

        Renderer_DrawSpriteEx(
            enemy->x,
            enemy->y,
            1,      // sprite del enemigo
            0,
            DIR_RIGHT,
            gSpriteSheet);
    }
}

bool Enemy_IsAlive(void)
{
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if (gEnemies[i].alive)
        {
            return true;
        }
    }

    return false;
}

int Enemy_FindAt(
    int x,
    int y)
{
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* enemy = &gEnemies[i];

        if (enemy->alive &&
            enemy->hp > 0 &&
            enemy->x == x &&
            enemy->y == y)
        {
            return i;
        }
    }

    return -1;
}

void Enemy_Damage(int index)
{
    if (index < 0 ||
        index >= ENEMY_POOL_SIZE)
    {
        return;
    }

    Enemy* enemy = &gEnemies[index];

    if (!enemy->alive ||
        enemy->hp <= 0)
    {
        return;
    }

    enemy->hp--;
    enemy->flashing = true;
    enemy->lastHit = SDL_GetTicks();
}

int Enemy_GetX(void)
{
    return gEnemies[0].x;
}

int Enemy_GetY(void)
{
    return gEnemies[0].y;
}
