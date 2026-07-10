#include "bullet.h"

#include <SDL2/SDL.h>

#include "renderer.h"
#include "room.h"

#define BULLET_POOL_SIZE 6

#define BULLET_MOVE_DELAY 16
#define BULLET_SPEED 2

typedef struct
{
    bool active;

    int pixelX;
    int pixelY;

    Direction direction;

    Uint32 lastMove;

} Bullet;

static Bullet gBullets[BULLET_POOL_SIZE];

bool Bullet_Init(void)
{
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        gBullets[i].active = false;
    }

    return true;
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

        bullet->pixelX = startX * 8 + 4;
        bullet->pixelY = startY * 8 + 4;

        switch (direction)
        {
            case DIR_RIGHT:
                bullet->pixelX += 8;
                break;

            case DIR_LEFT:
                bullet->pixelX -= 8;
                break;

            case DIR_UP:
                bullet->pixelY -= 8;
                break;

            case DIR_DOWN:
                bullet->pixelY += 8;
                break;
        }

        bullet->lastMove = SDL_GetTicks();

        return;
    }
}

void Bullet_Update(void)
{
    Uint32 now = SDL_GetTicks();

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

        int tileX = bullet->pixelX / 8;
        int tileY = bullet->pixelY / 8;

        if (tileX < 0 ||
            tileX >= ROOM_WIDTH ||
            tileY < 0 ||
            tileY >= ROOM_HEIGHT)
        {
            bullet->active = false;
            continue;
        }

        if (!Room_IsWalkable(tileX, tileY))
        {
            bullet->active = false;
            continue;
        }
    }
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

        switch (bullet->direction)
        {
            case DIR_RIGHT:
            case DIR_LEFT:

                Renderer_DrawPixel(
                    bullet->pixelX,
                    bullet->pixelY,
                    0xFFFFFFFF);

                Renderer_DrawPixel(
                    bullet->pixelX + 1,
                    bullet->pixelY,
                    0xFFFFFFFF);

                Renderer_DrawPixel(
                    bullet->pixelX + 2,
                    bullet->pixelY,
                    0xFFFFFFFF);

                break;

            case DIR_UP:
            case DIR_DOWN:

                Renderer_DrawPixel(
                    bullet->pixelX,
                    bullet->pixelY,
                    0xFFFFFFFF);

                Renderer_DrawPixel(
                    bullet->pixelX,
                    bullet->pixelY + 1,
                    0xFFFFFFFF);

                Renderer_DrawPixel(
                    bullet->pixelX,
                    bullet->pixelY + 2,
                    0xFFFFFFFF);

                break;
        }
    }
}

bool Bullet_IsActive(void)
{
    for (int i = 0; i < BULLET_POOL_SIZE; i++)
    {
        if (gBullets[i].active)
        {
            return true;
        }
    }

    return false;
}