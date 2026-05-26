#include "character.h"
#include "vblank.h"
#include <string.h>

// ============================================================
//  character.c
//
//  FIX GEMINI #2: char_update_gfx agora faz o tick de duração
//    e avança o subState ANTES de enfileirar o DMA.
//
//  FIX GEMINI #3: o frameCounter incrementa corretamente a cada
//    frame de vídeo; só reseta quando o sub-estado muda.
// ============================================================

Character  gCharacters[MAX_CHARS];
Character *gPlayer      = NULL;
Character *gYOrder[MAX_CHARS];
u16        gHudBuffer[HUD_SIZE];

// ----------------------------------------------------------------
void char_init(Character *c, u16 tileSize, u16 paletteSize) {
    c->tileSize     = tileSize;
    c->paletteSize  = paletteSize;
    c->state        = STATE_IDLE;
    c->subState     = 0;
    c->prevState    = 255;
    c->prevSubState = 255;
    c->frameCounter = 0;
    c->x = c->y = 0;
    c->y            = 150;
    c->velX = c->velY = 0;
    c->speed        = 0;
    c->speedTimer   = 0;
    c->hflip        = 0;
    c->alive        = true;
    c->visible      = true;
    c->priority     = 2;
    c->isInvincible = false;
    c->isHurt       = false;
    c->hurtTimer    = 0;
    c->hurtDuration = 20;
    c->hurtShake    = 0;

    int i;
    for (i = 0; i < MAX_CHARS; i++) {
        if (gYOrder[i] == NULL) { gYOrder[i] = c; break; }
    }
}

// ----------------------------------------------------------------
void char_load_gfx(Character *c, u8 oamAddr, u16 vramAddr,
                   u16 vramOffset, u8 paletteSlot) {
    c->oamAddress  = oamAddr;
    c->vramAddress = vramAddr;
    c->vramOffset  = vramOffset;
    c->paletteSlot = paletteSlot;

    WaitForVBlank();

    // gfx2snes -s 16 -o 16 already outputs OBJ 16x16 tile layout.
    dmaCopyVram(c->frames[0].tiles, vramAddr, c->tileSize);

    u8 palEntry = 128 + paletteSlot * 32;   // OBJ palettes start at CGRAM 128
    dmaCopyCGram(c->frames[0].palette, palEntry, c->paletteSize);

    oamSet(oamAddr, 0, 0, c->priority, 0, 0, vramOffset, paletteSlot);
    oamSetEx(oamAddr, OBJ_SMALL, OBJ_SHOW);
}

// ----------------------------------------------------------------
void char_set_state(Character *c, u8 state, u8 subState) {
    c->prevState    = c->state;
    c->prevSubState = c->subState;
    c->state        = state;
    c->subState     = subState;
    c->frameCounter = 0;   // reseta contador só na mudança de estado
}

// ----------------------------------------------------------------
//  FIX GEMINI #2 + #3: tick de animação correto
//
//  Fluxo por frame de vídeo:
//    1. Se mudou de subState → DMA novo tile, zera frameCounter
//    2. Incrementa frameCounter
//    3. Se frameCounter >= duration → avança para próximo subState
// ----------------------------------------------------------------
void char_update_gfx(Character *c) {
    if (!c->alive || !c->visible) return;

    // ── Passo 1: novo subState → DMA imediato ──────────────────
    if (c->subState != c->prevSubState) {
        c->prevSubState = c->subState;
        c->prevState    = c->state;
        c->frameCounter = 0;
        vblank_queue_sprite(
            c->frames[c->subState].tiles,
            c->vramAddress,
            c->tileSize
        );
        return;   // aguarda próximo frame para começar a contar
    }

    // ── Passo 2: incrementa o contador deste frame ──────────────
    c->frameCounter++;

    // ── Passo 3: duração esgotada → avança sub-estado ───────────
    SpriteFrame *f = &c->frames[c->subState];

    if (c->frameCounter >= f->duration) {
        // Só avança se não for um estado "preso" (duration=999)
        if (f->duration < 900) {
            char_set_state(c, f->nextState, f->nextSubState);
            // DMA do próximo frame já será feito no próximo ciclo
            // (subState != prevSubState na próxima chamada)
        }
    }
}

// ----------------------------------------------------------------
static void _oam_set_xy_ex(u8 id, int x, u8 y) {
    oamSetXY(id, x, y);

    #define OAM_HI_TABLE_START (128 * 4)
    u8  oam_id    = (u8)(id >> 2);
    int bit_shift = (oam_id & 3) << 1;
    int hi_pos    = (oam_id >> 4) + OAM_HI_TABLE_START;
    int mask      = 1 << bit_shift;
    oamMemory[hi_pos] &= ~mask;
    oamMemory[hi_pos] |= ((x >> 8) & 1) << bit_shift;
}

// ----------------------------------------------------------------
void char_update_pos(Character *c) {
    c->oldX = c->x;
    c->oldY = c->y;

    if (c->speedTimer == 0) {
        c->x += c->velX;
        c->y += c->velY;
    }

    if (c->velX > 0) c->hflip = 0;
    if (c->velX < 0) c->hflip = 1;

    if (c->speedTimer == 0) c->speedTimer = c->speed;
    else                    c->speedTimer--;

    if (c->x > SCREEN_MAX_X) c->x = SCREEN_MAX_X;
    if (c->x < SCREEN_MIN_X) c->x = SCREEN_MIN_X;
    if (c->y > SCREEN_MAX_Y) c->y = SCREEN_MAX_Y;
    if (c->y < SCREEN_MIN_Y) c->y = SCREEN_MIN_Y;

    oamFlip(c->oamAddress, c->hflip, 0);
    _oam_set_xy_ex(c->oamAddress, c->x, (u8)(c->y - (CANVAS_H - 1)));
}

// ----------------------------------------------------------------
void char_update_pos_npc(Character *c) {
    c->oldX = c->x;
    c->oldY = c->y;

    if (c->speedTimer == 0) {
        c->x += c->velX;
        c->y += c->velY;
    }

    if (c->velX > 0) c->hflip = 0;
    if (c->velX < 0) c->hflip = 1;

    if (c->speedTimer == 0) c->speedTimer = c->speed;
    else                    c->speedTimer--;

    oamFlip(c->oamAddress, c->hflip, 0);
    _oam_set_xy_ex(c->oamAddress, c->x, (u8)(c->y - (CANVAS_H - 1)));

    if (c->x < -80) oamSetVisible(c->oamAddress, OBJ_HIDE);
}

// ----------------------------------------------------------------
void char_set_velocity(Character *c, s8 vx, s8 vy) {
    c->velX = vx;
    c->velY = vy;
}

// ----------------------------------------------------------------
void char_apply_damage(Character *c, u8 dmg) {
    if (c->isInvincible || !c->alive) return;

    if (c->hp > dmg) c->hp -= dmg;
    else             c->hp  = 0;

    c->isHurt       = true;
    c->hurtTimer    = c->hurtDuration;
    c->hurtShake    = 0;
    c->isInvincible = true;

    if (c->hp == 0) {
        c->alive = false;
        char_set_state(c, STATE_DEAD, GUY_DEAD_1);
    } else {
        char_set_state(c, STATE_HURT, GUY_HURT_1);
    }
}

// ----------------------------------------------------------------
void char_update_hurt(Character *c) {
    if (!c->isHurt) return;

    if (c->hurtTimer > 0) {
        c->hurtTimer--;
        if ((c->hurtTimer & 4) == 0) {
            c->x += (c->hurtShake == 0) ? -2 : 2;
            c->hurtShake ^= 1;
        }
    } else {
        c->isHurt       = false;
        c->isInvincible = false;
        if (c->alive)
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
    }
}

// ----------------------------------------------------------------
void char_sort_y_order(void) {
    int i;
    for (i = 0; i < MAX_CHARS - 1; i++) {
        if (gYOrder[i] && gYOrder[i+1]) {
            if (gYOrder[i+1]->y > gYOrder[i]->y) {
                Character *tmp = gYOrder[i];
                gYOrder[i]     = gYOrder[i+1];
                gYOrder[i+1]   = tmp;
                char_swap_oam(gYOrder[i], gYOrder[i+1]);
            }
        } else if (!gYOrder[i] && gYOrder[i+1]) {
            gYOrder[i] = gYOrder[i+1];
        }
    }
}

// ----------------------------------------------------------------
void char_swap_oam(Character *p1, Character *p2) {
    u8 a1 = p1->oamAddress;
    u8 a2 = p2->oamAddress;
    u8 tmp[4];
    memcpy(tmp,            &oamMemory[a1], 4);
    memcpy(&oamMemory[a1], &oamMemory[a2], 4);
    memcpy(&oamMemory[a2], tmp,            4);
    p1->oamAddress = a2;
    p2->oamAddress = a1;
}
