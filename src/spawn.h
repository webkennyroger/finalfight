#ifndef SPAWN_H
#define SPAWN_H

#include "types.h"

extern Wave gWaves[MAX_WAVES];
extern u8   gWaveCount;
extern u8   gEnemiesAlive;    // inimigos vivos na tela
extern bool gStageClear;      // todos os inimigos derrotados

// Inicializa as ondas de inimigos da fase 1
void spawn_init(void);

// Verifica e ativa ondas conforme o scroll avança
// Chamado a cada frame com o scrollX atual
void spawn_update(u16 scrollX);

// Spawna um inimigo individual
void spawn_enemy(u8 slotIndex, u8 enemyType, int startX, int startY);

#endif
