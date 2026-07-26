#include "enemy.h"

#include <SDL2/SDL.h>
#include <stdlib.h>

#include "obelisk.h"
#include "player.h"
#include "room.h"
#include "renderer.h"

#define ENEMY_POOL_SIZE 5

#define ENEMY_HP 3

#define ENEMY_START_DELAY 550
#define ENEMY_MOVE_DELAY 450

#define ENEMY_FLASH_DURATION 120
#define ENEMY_FLASH_INTERVAL 40

typedef struct
{
    bool alive;
    bool flashing;
    bool mirrored;
    bool horizontalFirst;

    int x;
    int y;

    int hp;

    Uint32 lastHit;

} Enemy;

static Enemy gEnemies[ENEMY_POOL_SIZE];

static Uint32 gSpawnTime = 0;
static Uint32 gLastMoveTime = 0;

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

    if (Obelisk_IsAt(x, y))
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

static bool Enemy_CanMoveTo(
    int x,
    int y,
    int enemyIndex)
{
    if (!Room_IsWalkable(x, y))
    {
        return false;
    }

    if (Obelisk_IsAt(x, y))
    {
        return false;
    }

    if (x == Player_GetX() &&
        y == Player_GetY())
    {
        return false;
    }

    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        if (i == enemyIndex)
        {
            continue;
        }

        if (gEnemies[i].alive &&
            gEnemies[i].x == x &&
            gEnemies[i].y == y)
        {
            return false;
        }
    }

    return true;
}

static bool Enemy_TryMove(
    Enemy* enemy,
    int enemyIndex,
    int moveX,
    int moveY)
{
    int nextX = enemy->x + moveX;
    int nextY = enemy->y + moveY;

    if (!Enemy_CanMoveTo(
            nextX,
            nextY,
            enemyIndex))
    {
        return false;
    }

    enemy->x = nextX;
    enemy->y = nextY;
    enemy->mirrored = !enemy->mirrored;
    enemy->horizontalFirst =
        !enemy->horizontalFirst;

    return true;
}

static void Enemy_MoveTowardPlayer(
    Enemy* enemy,
    int enemyIndex)
{
    int deltaX = Player_GetX() - enemy->x;
    int deltaY = Player_GetY() - enemy->y;

    int distanceX = abs(deltaX);
    int distanceY = abs(deltaY);

    if (distanceX + distanceY <= 1)
    {
        return;
    }

    int moveX = (deltaX > 0) - (deltaX < 0);
    int moveY = (deltaY > 0) - (deltaY < 0);

    bool moveHorizontalFirst =
        moveX != 0 &&
        (moveY == 0 ||
         enemy->horizontalFirst);

    if (moveHorizontalFirst)
    {
        if (moveX != 0 &&
            Enemy_TryMove(
                enemy,
                enemyIndex,
                moveX,
                0))
        {
            return;
        }

        if (moveY != 0)
        {
            Enemy_TryMove(
                enemy,
                enemyIndex,
                0,
                moveY);
        }
    }
    else
    {
        if (moveY != 0 &&
            Enemy_TryMove(
                enemy,
                enemyIndex,
                0,
                moveY))
        {
            return;
        }

        if (moveX != 0)
        {
            Enemy_TryMove(
                enemy,
                enemyIndex,
                moveX,
                0);
        }
    }
}

static bool Enemy_IsNextToPlayer(
    const Enemy* enemy)
{
    int distanceX =
        abs(Player_GetX() - enemy->x);
    int distanceY =
        abs(Player_GetY() - enemy->y);

    return distanceX + distanceY == 1;
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
        enemy->mirrored = false;
        enemy->horizontalFirst = (i % 2 == 0);
    }

    gSpawnTime = SDL_GetTicks();
    gLastMoveTime = gSpawnTime;
}

void Enemy_Clear(void)
{
    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        gEnemies[i].alive = false;
        gEnemies[i].flashing = false;
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

    if (now - gSpawnTime < ENEMY_START_DELAY ||
        now - gLastMoveTime < ENEMY_MOVE_DELAY)
    {
        return;
    }

    gLastMoveTime = now;

    for (int i = 0; i < ENEMY_POOL_SIZE; i++)
    {
        Enemy* enemy = &gEnemies[i];

        if (!enemy->alive ||
            enemy->hp <= 0)
        {
            continue;
        }

        Enemy_MoveTowardPlayer(enemy, i);

        if (Enemy_IsNextToPlayer(enemy))
        {
            Player_Damage();
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

        Renderer_DrawAssetSpriteMirrored(
            enemy->x,
            enemy->y,
            1,      // sprite del enemigo
            0,
            enemy->mirrored);
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

bool Enemy_Damage(int index)
{
    if (index < 0 ||
        index >= ENEMY_POOL_SIZE)
    {
        return false;
    }

    Enemy* enemy = &gEnemies[index];

    if (!enemy->alive ||
        enemy->hp <= 0)
    {
        return false;
    }

    enemy->hp--;
    enemy->flashing = true;
    enemy->lastHit = SDL_GetTicks();

    return enemy->hp == 0;
}

int Enemy_GetX(void)
{
    return gEnemies[0].x;
}

int Enemy_GetY(void)
{
    return gEnemies[0].y;
}
