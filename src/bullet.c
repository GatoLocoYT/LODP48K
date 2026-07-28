#include "bullet.h"

#include <SDL2/SDL.h>
#include <stdbool.h>

#include "audio.h"
#include "enemy.h"
#include "renderer.h"
#include "raycast.h"

#define BULLET_POOL_SIZE 6

#define BULLET_MOVE_DELAY 16
#define BULLET_SPEED 2
#define BULLET_LENGTH 3

#define TILE_SIZE 8

typedef struct
{
    bool active;

    int pixelX;
    int pixelY;

    Direction direction;

    Uint32 lastMove;

} Bullet;

static Bullet gBullets[BULLET_POOL_SIZE];

void Bullet_Init(void)
{
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        gBullets[i].active = false;
    }
}

void Bullet_Shoot(
    int startX,
    int startY,
    Direction direction)
{
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* bullet = &gBullets[i];

        if (bullet->active)
        {
            continue;
        }

        bullet->active = true;

        bullet->direction = direction;

        bullet->pixelX = startX * TILE_SIZE + TILE_SIZE / 2;
        bullet->pixelY = startY * TILE_SIZE + TILE_SIZE / 2;

        switch (direction)
        {
            case DIR_RIGHT:
                bullet->pixelX =
                    (startX + 1) * TILE_SIZE;
                break;

            case DIR_LEFT:
                bullet->pixelX =
                    startX * TILE_SIZE - BULLET_LENGTH;
                break;

            case DIR_UP:
                bullet->pixelY =
                    startY * TILE_SIZE - BULLET_LENGTH;
                break;

            case DIR_DOWN:
                bullet->pixelY =
                    (startY + 1) * TILE_SIZE;
                break;
        }

        bullet->lastMove = SDL_GetTicks();

        Audio_PlayShot();

        return;
    }
}

int Bullet_Update(void)
{
    Uint32 now = SDL_GetTicks();
    int defeatedEnemies = 0;

    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* bullet = &gBullets[i];

        if (!bullet->active)
        {
            continue;
        }

        if (now - bullet->lastMove < BULLET_MOVE_DELAY)
        {
            continue;
        }

        bullet->lastMove = now;

        switch (bullet->direction)
        {
            case DIR_RIGHT:
                bullet->pixelX += BULLET_SPEED;
                break;

            case DIR_LEFT:
                bullet->pixelX -= BULLET_SPEED;
                break;

            case DIR_UP:
                bullet->pixelY -= BULLET_SPEED;
                break;

            case DIR_DOWN:
                bullet->pixelY += BULLET_SPEED;
                break;
        }

        int hitX = bullet->pixelX;
        int hitY = bullet->pixelY;

        if (bullet->direction == DIR_RIGHT)
        {
            hitX += BULLET_LENGTH - 1;
        }
        else if (bullet->direction == DIR_DOWN)
        {
            hitY += BULLET_LENGTH - 1;
        }

        RaycastResult result =
            Raycast_Cast(hitX, hitY);

        if (result.hit == RAYCAST_HIT_ENEMY)
        {
            if (Enemy_Damage(result.enemyIndex))
            {
                defeatedEnemies++;
            }
        }

        if (result.hit != RAYCAST_HIT_NONE)
        {
            bullet->active = false;
        }
    }

    return defeatedEnemies;
}

void Bullet_Draw(void)
{
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        Bullet* bullet = &gBullets[i];

        if (!bullet->active)
        {
            continue;
        }

        bool horizontal =
            bullet->direction == DIR_RIGHT ||
            bullet->direction == DIR_LEFT;

        for (int pixel = 0;
             pixel < BULLET_LENGTH;
             pixel++)
        {
            Renderer_DrawPixel(
                bullet->pixelX +
                    (horizontal ? pixel : 0),
                bullet->pixelY +
                    (horizontal ? 0 : pixel),
                0xFFFFFFFF);
        }
    }
}
