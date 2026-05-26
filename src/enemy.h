#ifndef ENEMY_H
#define ENEMY_H

#include "types.h"

// ============================================================
//  final_fight_snes - Inimigos
//  Hunter (bat), May (knife), Andore (big)
// ============================================================

// --- Tiles e paletas de cada inimigo (definidos no data.asm) ---
extern char hunter_idle_tiles,   hunter_idle_tiles_end;
extern char hunter_palette,      hunter_palette_end;

extern char may_idle_tiles,   may_idle_tiles_end;
extern char may_palette,      may_palette_end;

extern char andore_idle_tiles,   andore_idle_tiles_end;
extern char andore_palette,      andore_palette_end;

// --- Arrays de frames de cada inimigo ---
extern SpriteFrame gHunterFrames[ENEMY_TOTAL_FRAMES];
extern SpriteFrame gMayFrames[ENEMY_TOTAL_FRAMES];
extern SpriteFrame gAndoreFrames[ENEMY_TOTAL_FRAMES];

// Inicialização de cada inimigo
void enemy_init_hunter(Character *c);
void enemy_init_may(Character *c);
void enemy_init_andore(Character *c);

// IA: atualiza lógica de um inimigo em relação ao jogador
void enemy_update(Character *c, Character *player);

// Configuração de uma onda de inimigos
typedef struct {
    u8   type;        // 0=Hunter, 1=May, 2=Andore
    u8   count;
    int  spawnX;
    int  spawnY;
    u8   oamStart;
    u16  vramAddr;
    u16  vramOffset;
} WaveDef;

// Inicializa ondas de inimigos
void enemy_init_waves(void);

// Tenta spawnar inimigos com base nos pontos de gatilho
void enemy_update_waves(Character *player);

#endif // ENEMY_H
