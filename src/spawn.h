#ifndef SPAWN_H
#define SPAWN_H

#include "types.h"

#define MAX_WAVES   8

typedef struct {
    u8   triggerX;
    u8   types[3];
    u8   startY[3];
    u8   count;
    bool triggered;
} Wave;

#define ETYPE_HUNTER 0
#define ETYPE_MAY    1
#define ETYPE_ANDORE 2

extern Wave gWaves[MAX_WAVES];
extern u8   gWaveCount;
extern u8   gEnemiesAlive;
extern bool gStageClear;

void spawn_init(void);
void spawn_update(u16 scrollX);
void spawn_enemy(u8 slotIndex, u8 enemyType, int startX, int startY);

#endif
