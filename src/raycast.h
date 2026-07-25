#ifndef RAYCAST_H
#define RAYCAST_H

typedef enum
{
    RAYCAST_HIT_NONE,
    RAYCAST_HIT_WALL,
    RAYCAST_HIT_ENEMY
} RaycastHit;

typedef struct
{
    RaycastHit hit;
    int enemyIndex;
} RaycastResult;

RaycastResult Raycast_Cast(
    int pixelX,
    int pixelY);

#endif
