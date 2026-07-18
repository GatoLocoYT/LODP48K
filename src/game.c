#include "game.h"
#include "enemy.h"
#include "renderer.h"
#include "room.h"
#include "input.h"
#include "player.h"
#include "bullet.h"
#include <stdlib.h>
#include <time.h>

bool Game_Init(void)
{
    if (!Renderer_Init())
    {
        return false;
    }

    srand((unsigned)time(NULL));

    Room_LoadRandom();

    if (!Player_Init())
    {
        return false;
    }

    if (!Bullet_Init())
    {
        return false;
    }

    if (!Enemy_Init())
    {
        return false;
    }

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
        Enemy_Update();

        Player_Update();

        Bullet_Update();

        Renderer_Clear();

        Room_Draw();

        Enemy_Draw();

        Bullet_Draw();

        Player_Draw();

        Renderer_Present();
    }
}

void Game_Quit(void)
{
    Renderer_Quit();
}