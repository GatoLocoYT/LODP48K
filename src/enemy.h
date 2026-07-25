#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>

bool Enemy_Init(void);

void Enemy_Spawn(void);

void Enemy_Update(void);

void Enemy_Draw(void);

bool Enemy_IsAlive(void);

int Enemy_FindAt(
    int x,
    int y
);

void Enemy_Damage(int index);

int Enemy_GetX(void);
int Enemy_GetY(void);

#endif
