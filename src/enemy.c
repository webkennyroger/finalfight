#include "enemy.h"
#include "character.h"

// ============================================================
//  final_fight_snes - Inimigos: Hunter, May, Andore
//
//  Cada inimigo tem 6 sub-estados:
//    IDLE, WALK_1, WALK_2, ATTACK, HURT, DEAD
//
//  Os offsets de tiles seguem a ordem do sprite sheet (0x1000 por frame).
// ============================================================

// ----------------------------------------------------------------
//  HUNTER - Capanga com bastão de baseball
//  Sprite sheet: SNES_-_Final_Fight_3_-_Enemies_-_Hunter.png
// ----------------------------------------------------------------
SpriteFrame gHunterFrames[ENEMY_TOTAL_FRAMES] = {
    /* IDLE_1  */ { &hunter_idle_tiles,   &hunter_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  60, 0,63,0,63, false },
    /* WALK_1  */ { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  10, 0,63,0,63, false },
    /* WALK_2  */ { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  10, 0,63,0,63, false },
    /* ATTACK  */ { &hunter_idle_tiles,   &hunter_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  12,35,63,20,55, true  },
    /* HURT    */ { &hunter_idle_tiles,   &hunter_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    /* DEAD    */ { &hunter_idle_tiles,   &hunter_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

// ----------------------------------------------------------------
//  MAY - Capanga feminina com faca
//  Sprite sheet: SNES_-_Final_Fight_3_-_Enemies_-_May.png
// ----------------------------------------------------------------
SpriteFrame gMayFrames[ENEMY_TOTAL_FRAMES] = {
    /* IDLE_1  */ { &may_idle_tiles,   &may_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  50, 0,63,0,63, false },
    /* WALK_1  */ { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,   8, 0,63,0,63, false },
    /* WALK_2  */ { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,   8, 0,63,0,63, false },
    /* ATTACK  */ { &may_idle_tiles,   &may_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  10,30,63,15,50, true  },
    /* HURT    */ { &may_idle_tiles,   &may_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    /* DEAD    */ { &may_idle_tiles,   &may_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

// ----------------------------------------------------------------
//  ANDORE - Inimigo gigante (sub-boss)
//  Sprite sheet: SNES_-_Final_Fight_3_-_Enemies_-_Andore.png
// ----------------------------------------------------------------
SpriteFrame gAndoreFrames[ENEMY_TOTAL_FRAMES] = {
    /* IDLE_1  */ { &andore_idle_tiles,   &andore_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  40, 0,63,0,63, false },
    /* WALK_1  */ { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  12, 0,63,0,63, false },
    /* WALK_2  */ { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  12, 0,63,0,63, false },
    /* ATTACK  */ { &andore_idle_tiles,   &andore_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  15,28,63,10,58, true  },
    /* HURT    */ { &andore_idle_tiles,   &andore_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    /* DEAD    */ { &andore_idle_tiles,   &andore_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

// --- Nomes ---
static char sHunterName[] = "HUNTER";
static char sMayName[]    = "MAY";
static char sAndoreName[] = "ANDORE";

// ----------------------------------------------------------------
void enemy_init_hunter(Character *c) {
    c->frames       = gHunterFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->hp           = 6;
    c->maxHp        = 6;
    c->isEnemy      = true;
    c->name         = sHunterName;
    c->speed        = 1;
    c->hurtDuration = 28;
}

void enemy_init_may(Character *c) {
    c->frames       = gMayFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->hp           = 5;
    c->maxHp        = 5;
    c->isEnemy      = true;
    c->name         = sMayName;
    c->speed        = 0;        // May é mais rápida
    c->hurtDuration = 22;
}

void enemy_init_andore(Character *c) {
    c->frames       = gAndoreFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->hp           = 14;
    c->maxHp        = 14;
    c->isEnemy      = true;
    c->name         = sAndoreName;
    c->speed        = 2;        // Andore é lento
    c->hurtDuration = 35;
}

// ----------------------------------------------------------------
//  IA simples: persegue o jogador; ataca quando próximo
// ----------------------------------------------------------------
#define ATTACK_RANGE_X  35
#define ATTACK_RANGE_Y   8

void enemy_update(Character *c, Character *player) {
    if (!c->alive || !player->alive) return;

    // Estado de hurt gerenciado pelo sistema de personagem
    if (c->state == STATE_HURT) {
        char_update_hurt(c);
        return;
    }

    if (c->state == STATE_DEAD) return;

    // Calcula distância ao jogador
    int dx = player->x - c->x;
    int dy = player->y - c->y;
    int absDx = (dx > 0) ? dx : -dx;
    int absDy = (dy > 0) ? dy : -dy;

    // --- Ataque se dentro do alcance ---
    if (absDx < ATTACK_RANGE_X && absDy < ATTACK_RANGE_Y) {
        if (c->state != STATE_PUNCH) {
            char_set_velocity(c, 0, 0);
            char_set_state(c, STATE_PUNCH, ENEMY_ATTACK_1);
        }
        return;
    }

    // --- Perseguição ---
    s8 vx = 0, vy = 0;

    if (dx > 0)      vx = +ENEMY_SPEED_X;
    else if (dx < 0) vx = -ENEMY_SPEED_X;

    if (dy > 0)      vy = +ENEMY_SPEED_Y;
    else if (dy < 0) vy = -ENEMY_SPEED_Y;

    char_set_velocity(c, vx, vy);

    if (c->state != STATE_WALK)
        char_set_state(c, STATE_WALK, ENEMY_WALK_1);
}
