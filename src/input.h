#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

void Input_Update(void);

bool Input_Left(void);
bool Input_Right(void);
bool Input_Up(void);
bool Input_Down(void);
bool Input_Shoot(void);
bool Input_Quit(void);

#endif