#ifndef ENEMY_H
#define ENEMY_H

#include <stdbool.h>

void Enemy_Spawn(void);

void Enemy_Clear(void);

void Enemy_Update(void);

void Enemy_Draw(void);

int Enemy_FindAt(
    int x,
    int y
);

bool Enemy_Damage(int index);

#endif
