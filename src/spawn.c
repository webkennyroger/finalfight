#include "spawn.h"
#include "character.h"
#include "enemy.h"

// ============================================================
//  spawn.c — Ondas de inimigos da Fase 1 (Rua de Metro City)
//
//  Cada onda é ativada quando o scroll passa de um certo X.
//  Os inimigos aparecem pela direita da tela (x ≈ 256).
//
//  Onda  X     Inimigos
//  ─────────────────────────────────────
//   0    100   2× Hunter
//   1    300   1× Hunter + 1× May
//   2    500   2× May
//   3    700   1× May + 1× Andore
//   4    900   2× Hunter + 1× May
//   5   1100   1× Andore (sub-boss)
//   6   1300   2× Andore (boss)
// ============================================================

Wave gWaves[MAX_WAVES];
u8   gWaveCount   = 0;
u8   gEnemiesAlive = 0;
bool gStageClear  = false;

// Slot VRAM/OAM por inimigo (slots 1-3 do array gCharacters)
#define ENEMY_OAM_BASE   24   // Guy usa 0-23
#define ENEMY_OAM_STEP    4   // cada inimigo usa 4 slots OAM

// VRAM separado por inimigo
#define ENEMY_VRAM_BASE  0x7000
#define ENEMY_VRAM_STEP  0x0400

// ----------------------------------------------------------------
static void _define_wave(u8 trigX,
                         u8 t0, u8 y0,
                         u8 t1, u8 y1,
                         u8 t2, u8 y2,
                         u8 count) {
    if (gWaveCount >= MAX_WAVES) return;
    Wave *w      = &gWaves[gWaveCount++];
    w->triggerX  = trigX;
    w->types[0]  = t0; w->startY[0] = y0;
    w->types[1]  = t1; w->startY[1] = y1;
    w->types[2]  = t2; w->startY[2] = y2;
    w->count     = count;
    w->triggered = false;
}

// ----------------------------------------------------------------
void spawn_init(void) {
    gWaveCount    = 0;
    gEnemiesAlive = 0;
    gStageClear   = false;

    // Define ondas (trigX, tipo0,y0, tipo1,y1, tipo2,y2, count)
    _define_wave(100,  ETYPE_HUNTER,150, ETYPE_HUNTER,170, 0,0,          2);
    _define_wave(300,  ETYPE_HUNTER,160, ETYPE_MAY,   145, 0,0,          2);
    _define_wave(500,  ETYPE_MAY,   150, ETYPE_MAY,   170, 0,0,          2);
    _define_wave(700,  ETYPE_MAY,   155, ETYPE_ANDORE,165, 0,0,          2);
    _define_wave(900,  ETYPE_HUNTER,145, ETYPE_HUNTER,170, ETYPE_MAY,155, 3);
    _define_wave(1100, ETYPE_ANDORE,160, 0,0,          0,0,              1);
    _define_wave(1300, ETYPE_ANDORE,150, ETYPE_ANDORE,170, 0,0,          2);
}

// ----------------------------------------------------------------
void spawn_enemy(u8 slotIdx, u8 type, int startX, int startY) {
    if (slotIdx >= MAX_CHARS - 1) return;   // slot 0 = player

    Character *c = &gCharacters[1 + slotIdx];

    // Reinicia o personagem
    char_init(c, 0x200, 32);

    switch (type) {
        case ETYPE_HUNTER: enemy_init_hunter(c); break;
        case ETYPE_MAY:    enemy_init_may(c);    break;
        case ETYPE_ANDORE: enemy_init_andore(c); break;
    }

    c->x          = startX;
    c->y          = startY;
    c->oamAddress = (u8)(ENEMY_OAM_BASE + slotIdx * ENEMY_OAM_STEP);

    u16 vram = (u16)(ENEMY_VRAM_BASE + slotIdx * ENEMY_VRAM_STEP);
    char_load_gfx(c, c->oamAddress, vram, vram >> 4, (u8)(1 + slotIdx));

    gEnemiesAlive++;
}

// ----------------------------------------------------------------
void spawn_update(u16 scrollX) {
    int i;

    // Conta inimigos vivos
    gEnemiesAlive = 0;
    for (i = 1; i < MAX_CHARS; i++) {
        if (gCharacters[i].alive) gEnemiesAlive++;
    }

    // Verifica se há onda pronta para ativar
    // (só spawna se não houver inimigos na tela)
    if (gEnemiesAlive > 0) return;

    for (i = 0; i < gWaveCount; i++) {
        Wave *w = &gWaves[i];
        if (w->triggered) continue;
        if (scrollX < w->triggerX) continue;

        // Ativa esta onda
        w->triggered = true;
        u8 slot = 0;
        u8 j;
        for (j = 0; j < w->count && j < 3; j++) {
            spawn_enemy(slot, w->types[j],
                        250 + j * 30,   // entra pela direita
                        (int)w->startY[j]);
            slot++;
        }
        break;   // apenas uma onda por frame
    }

    // Verifica se a fase acabou
    bool allDone = true;
    for (i = 0; i < gWaveCount; i++) {
        if (!gWaves[i].triggered) { allDone = false; break; }
    }
    if (allDone && gEnemiesAlive == 0) gStageClear = true;
}
