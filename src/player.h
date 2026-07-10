#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>

bool Player_Init(void);

void Player_Update(void);

void Player_Draw(void);

int Player_GetX(void);
int Player_GetY(void);

#endif