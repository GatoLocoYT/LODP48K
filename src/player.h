#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "direction.h"

bool Player_Init(void);

bool Player_Update(void);

void Player_EnterRoom(void);

void Player_Draw(void);

int Player_GetX(void);
int Player_GetY(void);

#endif
