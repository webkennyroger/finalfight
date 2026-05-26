#include "enemy.h"
#include "character.h"
#include "hud.h"

// ============================================================
//  final_fight_snes - Inimigos + Ondas
//
//  Cada inimigo tem 6 sub-estados:
//    IDLE, WALK_1, WALK_2, ATTACK, HURT, DEAD
//
//  OAM allocation:
//    Player:  0-23  (24 entries)
//    Enemy 1: 24-47 (24 entries)
//    Enemy 2: 48-71 (24 entries)
//    Enemy 3: 72-95 (24 entries)
//    Enemy 4: 96-119 (24 entries)
// ============================================================

// ----------------------------------------------------------------
//  HUNTER
// ----------------------------------------------------------------
SpriteFrame gHunterFrames[ENEMY_TOTAL_FRAMES] = {
    { &hunter_idle_tiles,   &hunter_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  60, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  10, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  10, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  12,35,63,20,55, true  },
    { &hunter_idle_tiles,   &hunter_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &hunter_idle_tiles,   &hunter_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

// ----------------------------------------------------------------
//  MAY
// ----------------------------------------------------------------
SpriteFrame gMayFrames[ENEMY_TOTAL_FRAMES] = {
    { &may_idle_tiles,   &may_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  50, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,   8, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,   8, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  10,30,63,15,50, true  },
    { &may_idle_tiles,   &may_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &may_idle_tiles,   &may_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

// ----------------------------------------------------------------
//  ANDORE
// ----------------------------------------------------------------
SpriteFrame gAndoreFrames[ENEMY_TOTAL_FRAMES] = {
    { &andore_idle_tiles,   &andore_palette, STATE_IDLE,  STATE_WALK,  ENEMY_WALK_1,  40, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_2,  12, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_WALK,  STATE_WALK,  ENEMY_WALK_1,  12, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_PUNCH, STATE_IDLE,  ENEMY_IDLE_1,  15,28,63,10,58, true  },
    { &andore_idle_tiles,   &andore_palette, STATE_HURT,  STATE_HURT,  ENEMY_HURT_1, 999, 0,63,0,63, false },
    { &andore_idle_tiles,   &andore_palette, STATE_DEAD,  STATE_DEAD,  ENEMY_DEAD_1, 999, 0,63,0,63, false },
};

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
    c->speed        = 0;
    c->hurtDuration = 22;
}

void enemy_init_andore(Character *c) {
    c->frames       = gAndoreFrames;
    c->totalFrames  = ENEMY_TOTAL_FRAMES;
    c->hp           = 14;
    c->maxHp        = 14;
    c->isEnemy      = true;
    c->name         = sAndoreName;
    c->speed        = 2;
    c->hurtDuration = 35;
}

// ----------------------------------------------------------------
#define ATTACK_RANGE_X  35
#define ATTACK_RANGE_Y   8

void enemy_update(Character *c, Character *player) {
    if (!c->alive || !player->alive) return;

    if (c->state == STATE_HURT) {
        char_update_hurt(c);
        return;
    }

    if (c->state == STATE_DEAD) return;

    int dx = player->x - c->x;
    int dy = player->y - c->y;
    int absDx = (dx > 0) ? dx : -dx;
    int absDy = (dy > 0) ? dy : -dy;

    if (absDx < ATTACK_RANGE_X && absDy < ATTACK_RANGE_Y) {
        if (c->state != STATE_PUNCH) {
            char_set_velocity(c, 0, 0);
            char_set_state(c, STATE_PUNCH, ENEMY_ATTACK_1);
        }
        return;
    }

    s8 vx = 0, vy = 0;

    if (dx > 0)      vx = +ENEMY_SPEED_X;
    else if (dx < 0) vx = -ENEMY_SPEED_X;

    if (dy > 0)      vy = +ENEMY_SPEED_Y;
    else if (dy < 0) vy = -ENEMY_SPEED_Y;

    char_set_velocity(c, vx, vy);

    if (c->state != STATE_WALK)
        char_set_state(c, STATE_WALK, ENEMY_WALK_1);
}

// ============================================================
//  SISTEMA DE ONDAS
// ============================================================

typedef struct {
    bool  used;
    bool  spawned;
    u8    enemyType;
    u8    count;
    int   spawnX;
    int   spawnY;
    int   triggerX;
} WaveSlot;

#define MAX_WAVES 6
static WaveSlot sWaves[MAX_WAVES];

// ----------------------------------------------------------------
void enemy_init_waves(void) {
    int i;
    for (i = 0; i < MAX_WAVES; i++)
        sWaves[i].used = false;

    // Wave 0: 2 Hunters, spawn at triggerX=100
    sWaves[0].used = true; sWaves[0].spawned = false;
    sWaves[0].enemyType = 0; sWaves[0].count = 2;
    sWaves[0].spawnX = 220; sWaves[0].spawnY = 160;
    sWaves[0].triggerX = 100;

    // Wave 1: 1 May + 1 Hunter, triggerX=200
    sWaves[1].used = true; sWaves[1].spawned = false;
    sWaves[1].enemyType = 1; sWaves[1].count = 1;
    sWaves[1].spawnX = 230; sWaves[1].spawnY = 155;
    sWaves[1].triggerX = 200;

    sWaves[2].used = true; sWaves[2].spawned = false;
    sWaves[2].enemyType = 0; sWaves[2].count = 2;
    sWaves[2].spawnX = 240; sWaves[2].spawnY = 160;
    sWaves[2].triggerX = 280;

    // Wave 2: 3 May + 1 Hunter, triggerX=350
    sWaves[3].used = true; sWaves[3].spawned = false;
    sWaves[3].enemyType = 1; sWaves[3].count = 2;
    sWaves[3].spawnX = 250; sWaves[3].spawnY = 155;
    sWaves[3].triggerX = 400;

    sWaves[4].used = true; sWaves[4].spawned = false;
    sWaves[4].enemyType = 0; sWaves[4].count = 2;
    sWaves[4].spawnX = 260; sWaves[4].spawnY = 160;
    sWaves[4].triggerX = 400;

    // Wave 3: Andore (mini-boss), triggerX=500
    sWaves[5].used = true; sWaves[5].spawned = false;
    sWaves[5].enemyType = 2; sWaves[5].count = 1;
    sWaves[5].spawnX = 230; sWaves[5].spawnY = 150;
    sWaves[5].triggerX = 550;
}

// ----------------------------------------------------------------
static void _spawn_enemy(u8 type, int spawnX, int spawnY) {
    int i;
    Character *slot = NULL;

    for (i = 1; i < MAX_CHARS; i++) {
        if (!gCharacters[i].alive) { slot = &gCharacters[i]; break; }
    }
    if (!slot) return;

    char_init(slot, GUY_TILE_SIZE, 32);
    slot->x = spawnX;
    slot->y = spawnY;

    // OAM: enemy i starts at entry i * 24 (0 reserved for player)
    u8 oamAddr = (u8)(i * 24);
    // VRAM offset from OBJ base (0x6000). Each frame = 3072 bytes = 24 16x16 tiles.
    // One 16x16 tile = 128 bytes. 3072/128 = 24. So slot i: vramOffset = i * 24.
    u16 vramAddr = 0x6000 + i * GUY_TILE_SIZE;
    u16 tileOffs = (u16)(i * 24);

    switch (type) {
        case 0: enemy_init_hunter(slot); break;
        case 1: enemy_init_may(slot);    break;
        case 2: enemy_init_andore(slot); break;
    }

    char_load_gfx(slot, oamAddr, vramAddr, tileOffs, (u8)i);
}

// ----------------------------------------------------------------
void enemy_update_waves(Character *player) {
    static bool sScored[MAX_CHARS];
    int i;

    for (i = 0; i < MAX_WAVES; i++) {
        WaveSlot *w = &sWaves[i];
        if (!w->used || w->spawned) continue;
        if (player->x >= w->triggerX) {
            w->spawned = true;
            int j;
            for (j = 0; j < w->count; j++) {
                _spawn_enemy(w->enemyType,
                    w->spawnX + j * 40,
                    w->spawnY);
            }
        }
    }

    // Award score once per enemy death
    for (i = 1; i < MAX_CHARS; i++) {
        Character *e = &gCharacters[i];
        if (!e->alive) {
            if (!sScored[i]) {
                sScored[i] = true;
                // score/kill tracking removed (no digit tiles in HUD)
            }
        } else {
            sScored[i] = false;
        }
    }
}