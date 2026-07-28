#ifndef PLAYER_H
#define PLAYER_H

#include <stdbool.h>
#include "direction.h"

void Player_Init(void);

bool Player_Update(void);

void Player_EnterRoom(void);

void Player_Draw(void);

void Player_Damage(void);

int Player_GetX(void);
int Player_GetY(void);
int Player_GetHP(void);

#endif
