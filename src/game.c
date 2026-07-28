#include "game.h"

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <time.h>

#include "audio.h"
#include "bullet.h"
#include "defeat.h"
#include "enemy.h"
#include "hud.h"
#include "input.h"
#include "menu.h"
#include "obelisk.h"
#include "player.h"
#include "renderer.h"
#include "room.h"
#include "victory.h"

#define GAME_TIME_SECONDS 300
#define SCORE_PER_ENEMY 10
#define MAX_SCORE 999999

static int gTimeRemaining = GAME_TIME_SECONDS;
static int gTimeElapsed = 0;
static int gScore = 0;
static int gRoomsPassed = 0;

static Uint32 gLastTimerTick = 0;

typedef enum
{
    GAME_STATE_MENU,
    GAME_STATE_PLAYING,
    GAME_STATE_VICTORY,
    GAME_STATE_DEFEAT
} GameState;

static GameState gGameState = GAME_STATE_MENU;
static VictoryOption gVictoryOption =
    VICTORY_OPTION_RETRY;

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

    int secondsToCount =
        (int)elapsedSeconds;

    if (secondsToCount > gTimeRemaining)
    {
        secondsToCount = gTimeRemaining;
    }

    gTimeRemaining -= secondsToCount;
    gTimeElapsed += secondsToCount;
}

static bool Game_CheckDefeat(void)
{
    int playerHP = Player_GetHP();

    if (gTimeRemaining > 0 &&
        playerHP > 0)
    {
        return false;
    }

    Enemy_Clear();
    Obelisk_Clear();

    if (playerHP <= 0)
    {
        Audio_PlayPlayerDeath();
    }
    else
    {
        Audio_StopAll();
    }

    gGameState = GAME_STATE_DEFEAT;

    return true;
}

static void Game_Start(void)
{
    Room_LoadRandom();

    Player_Init();
    Bullet_Init();

    gRoomsPassed = 0;
    Obelisk_Spawn(gRoomsPassed);

    Enemy_Spawn();

    gTimeRemaining = GAME_TIME_SECONDS;
    gTimeElapsed = 0;
    gScore = 0;
    gLastTimerTick = SDL_GetTicks();
    Audio_StartGame();
    gGameState = GAME_STATE_PLAYING;
}

static void Game_UpdatePlaying(void)
{
    Game_UpdateTimer();

    if (Game_CheckDefeat())
    {
        return;
    }

    Enemy_Update();

    if (Game_CheckDefeat())
    {
        return;
    }

    if (Player_Update())
    {
        gRoomsPassed++;
        Obelisk_Clear();
        Room_LoadRandom();
        Player_EnterRoom();
        Obelisk_Spawn(gRoomsPassed);
        Enemy_Spawn();
    }

    gScore +=
        Bullet_Update() * SCORE_PER_ENEMY;

    if (gScore > MAX_SCORE)
    {
        gScore = MAX_SCORE;
    }

    if (Obelisk_IsAt(
            Player_GetX(),
            Player_GetY()))
    {
        Enemy_Clear();
        Obelisk_Clear();
        Audio_PlayVictory();
        gVictoryOption =
            VICTORY_OPTION_RETRY;
        gGameState = GAME_STATE_VICTORY;
        return;
    }
}

static void Game_DrawPlaying(void)
{
    Renderer_Clear();

    Room_Draw();

    Obelisk_Draw();

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

static void Game_DrawDefeat(void)
{
    Renderer_Clear();
    Defeat_Draw();
    Renderer_Present();
}

static void Game_DrawVictory(void)
{
    Renderer_Clear();
    Victory_Draw(
        gTimeElapsed,
        gScore,
        gVictoryOption);
    Renderer_Present();
}

bool Game_Init(void)
{
    if (!Renderer_Init())
    {
        return false;
    }

    srand((unsigned)time(NULL));

    Audio_Init();

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
            if (Input_Start())
            {
                Game_Start();
            }

            if (gGameState == GAME_STATE_MENU)
            {
                Game_DrawMenu();
                continue;
            }
        }

        if (gGameState == GAME_STATE_VICTORY)
        {
            if (Input_NavigateUp() ||
                Input_NavigateDown())
            {
                gVictoryOption =
                    gVictoryOption ==
                            VICTORY_OPTION_RETRY
                        ? VICTORY_OPTION_EXIT
                        : VICTORY_OPTION_RETRY;
            }

            if (Input_Start())
            {
                if (gVictoryOption ==
                    VICTORY_OPTION_RETRY)
                {
                    Audio_StopAll();
                    gGameState =
                        GAME_STATE_MENU;
                    Game_DrawMenu();
                    continue;
                }

                running = false;
                continue;
            }

            Game_DrawVictory();
            continue;
        }

        if (gGameState == GAME_STATE_DEFEAT)
        {
            if (Input_Start())
            {
                Audio_StopAll();
                gGameState = GAME_STATE_MENU;
                Game_DrawMenu();
                continue;
            }

            Game_DrawDefeat();
            continue;
        }

        Game_UpdatePlaying();

        if (gGameState == GAME_STATE_VICTORY)
        {
            Game_DrawVictory();
        }
        else if (gGameState ==
                 GAME_STATE_DEFEAT)
        {
            Game_DrawDefeat();
        }
        else
        {
            Game_DrawPlaying();
        }
    }
}

void Game_Quit(void)
{
    Audio_Quit();
    Renderer_Quit();
}
