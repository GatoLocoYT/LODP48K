#include "game.h"
#include "renderer.h"
#include "room.h"
#include "input.h"
#include <time.h>
#include <stdlib.h>
#include "../tools/generated/assets.h"

static int playerX = 8;
static int playerY = 8;

bool Game_Init(void)
{
    if (!Renderer_Init())
    {
        return false;
    }

    srand((unsigned)time(NULL));

    Room_LoadRandom();
    return true;
}

void Game_Run(void)
{
    bool running = true;

    while (running)
    {
        Input_Update();

        if (Input_Quit())
        {
            running = false;
        }

        if (Input_Left())
        {
            if (Room_IsWalkable(playerX - 1, playerY))
            {
                playerX--;
            }
        }
        else if (Input_Right())
        {
            if (Room_IsWalkable(playerX + 1, playerY))
            {
                playerX++;
            }
        }
        else if (Input_Up())
        {
            if (Room_IsWalkable(playerX, playerY - 1))
            {
                playerY--;
            }
        }
        else if (Input_Down())
        {
            if (Room_IsWalkable(playerX, playerY + 1))
            {
                playerY++;
            }
        }
        
        if (playerX < 0)
        {
            Room_LoadRandom();
            playerX = ROOM_WIDTH - 2;
        }

        if (playerX >= ROOM_WIDTH)
        {
            Room_LoadRandom();
            playerX = 1;
        }

        if (playerY < 0)
        {
            Room_LoadRandom();
            playerY = ROOM_HEIGHT - 2;
        }

        if (playerY >= ROOM_HEIGHT)
        {
            Room_LoadRandom();
            playerY = 1;
        }

        Renderer_Clear();

        Room_Draw();

        Renderer_DrawSprite(
            playerX,
            playerY,
            0,
            0,
            gSpriteSheet);

        Renderer_Present();
    }
}

void Game_Quit(void)
{
    Renderer_Quit();
}