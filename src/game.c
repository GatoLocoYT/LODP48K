#include "game.h"
#include <SDL2/SDL.h>
#include "bullet.h"
#include "enemy.h"
#include "hud.h"
#include "input.h"
#include "menu.h"
#include "player.h"
#include "renderer.h"
#include "room.h"
#include <stdlib.h>
#include <time.h>

#define GAME_TIME_SECONDS 300
#define SCORE_PER_ENEMY 10
#define MAX_SCORE 999999

static int gTimeRemaining = GAME_TIME_SECONDS;
static int gScore = 0;

static Uint32 gLastTimerTick = 0;

typedef enum
{
    GAME_STATE_MENU,
    GAME_STATE_PLAYING
} GameState;

static GameState gGameState = GAME_STATE_MENU;

static void Game_UpdateTimer(void)
{
    Uint32 now = SDL_GetTicks();
    Uint32 elapsedSeconds =
        (now - gLastTimerTick) / 1000;

    if (elapsedSeconds == 0)
    {
        return;
    }

    gLastTimerTick += elapsedSeconds * 1000;

    if (elapsedSeconds >=
        (Uint32)gTimeRemaining)
    {
        gTimeRemaining = 0;
    }
    else
    {
        gTimeRemaining -=
            (int)elapsedSeconds;
    }
}

static bool Game_Start(void)
{
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

    gTimeRemaining = GAME_TIME_SECONDS;
    gScore = 0;
    gLastTimerTick = SDL_GetTicks();
    gGameState = GAME_STATE_PLAYING;

    return true;
}

static void Game_UpdatePlaying(void)
{
    Game_UpdateTimer();

    Enemy_Update();

    if (Player_Update())
    {
        Room_LoadRandom();
        Player_EnterRoom();
        Enemy_Spawn();
    }

    gScore +=
        Bullet_Update() * SCORE_PER_ENEMY;

    if (gScore > MAX_SCORE)
    {
        gScore = MAX_SCORE;
    }
}

static void Game_DrawPlaying(void)
{
    Renderer_Clear();

    Room_Draw();

    Enemy_Draw();

    Bullet_Draw();

    Player_Draw();

    Hud_Draw(
        gTimeRemaining,
        gScore,
        Player_GetHP());

    Renderer_Present();
}

static void Game_DrawMenu(void)
{
    Renderer_Clear();
    Menu_Draw();
    Renderer_Present();
}

bool Game_Init(void)
{
    if (!Renderer_Init())
    {
        return false;
    }

    srand((unsigned)time(NULL));

    gGameState = GAME_STATE_MENU;

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
            continue;
        }

        if (gGameState == GAME_STATE_MENU)
        {
            if (Input_Start() &&
                !Game_Start())
            {
                running = false;
                continue;
            }

            if (gGameState == GAME_STATE_MENU)
            {
                Game_DrawMenu();
                continue;
            }
        }

        Game_UpdatePlaying();
        Game_DrawPlaying();
    }
}

void Game_Quit(void)
{
    Renderer_Quit();
}
