#ifndef VICTORY_H
#define VICTORY_H

typedef enum
{
    VICTORY_OPTION_RETRY,
    VICTORY_OPTION_EXIT
} VictoryOption;

void Victory_Draw(
    int elapsedTime,
    int score,
    VictoryOption selectedOption
);

#endif
