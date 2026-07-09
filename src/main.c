#include "game.h"

int main(int argc, char *argv[])
{
    (void)argc;
    (void)argv;

    if (!Game_Init())
    {
        return 1;
    }
    Game_Run();

    Game_Quit();

    return 0;
}