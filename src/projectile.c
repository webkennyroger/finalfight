#include "projectile.h"
#include "character.h"
#include "vblank.h"

// ============================================================
//  projectile.c — Bushin Goraisenpujin
//
//  O projétil usa 2×2 tiles de 16×16 (32×32 px total).
//  Tile SNES chars (base VRAM 0x7E00, arranjo 16 wide):
//    Char 0 = tile (col0,row0), Char 2 = tile (col1,row0)
//    Char 32= tile (col0,row1), Char 34= tile (col1,row1)
//
//  OAM reservado: slots PROJ_OAM_BASE até +4 por projétil.
// ============================================================

Projectile gProjectiles[MAX_PROJECTILES];

// ----------------------------------------------------------------
void projectile_init(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        gProjectiles[i].active = false;
        gProjectiles[i].timer  = 0;
    }

    // Carrega tiles do projétil no VRAM
    WaitForVBlank();
    dmaCopyVram(&gora_tiles,   PROJ_VRAM, PROJ_TILE_SIZE);
    dmaCopyCGram(&gora_palette, 128 + 5*32, 32); // OBJ palette slot 5
}

// ----------------------------------------------------------------
void projectile_fire(Character *c) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        if (!gProjectiles[i].active) {
            gProjectiles[i].active    = true;
            gProjectiles[i].timer     = PROJ_LIFETIME;
            gProjectiles[i].direction = c->hflip;
            gProjectiles[i].velX      = c->hflip ? -PROJ_SPEED : +PROJ_SPEED;
            // Aparece na frente do personagem, na altura do tronco
            gProjectiles[i].x = c->x + (c->hflip ? -24 : 48);
            gProjectiles[i].y = c->y - 32;
            gProjectiles[i].oamAddress = (u8)(PROJ_OAM_BASE + i * 4);

            // Configura OAM (sprite 32×32 = 2 OAM_LARGE de 16×16)
            oamSet(gProjectiles[i].oamAddress,
                   gProjectiles[i].x,
                   gProjectiles[i].y,
                   3, 0, 0,
                   PROJ_VRAM >> 4,   // vramOffset
                   5);               // palette slot 5
            oamSetEx(gProjectiles[i].oamAddress, OBJ_SMALL, OBJ_SHOW);
            return;
        }
    }
}

// ----------------------------------------------------------------
void projectile_update_all(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;

        p->x += p->velX;
        p->timer--;

        // Expirou ou saiu da tela
        if (p->timer == 0 || p->x < -32 || p->x > 280) {
            p->active = false;
            oamSetVisible(p->oamAddress, OBJ_HIDE);
            continue;
        }

        // Animação simples: alterna entre 2 frames de tile
        u8 frame = (p->timer >> 2) & 1;   // troca a cada 4 frames
        u16 vramOff = PROJ_VRAM + (u16)(frame * PROJ_TILE_SIZE);
        vblank_queue_sprite(&gora_tiles + frame * PROJ_TILE_SIZE,
                            vramOff, PROJ_TILE_SIZE);

        oamSetXY(p->oamAddress, p->x, p->y);
    }
}

// ----------------------------------------------------------------
void projectile_check_hits(void) {
    int i, j;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;

        for (j = 1; j < MAX_CHARS; j++) {
            Character *t = &gCharacters[j];
            if (!t->alive || t->isInvincible) continue;
            if (!t->isEnemy) continue;

            // Hitbox do projétil: 32×32 centrada
            int dx = (p->x + 16) - (t->x + 16);
            int dy = (p->y + 16) - (t->y - 32);
            if (dx < 0) dx = -dx;
            if (dy < 0) dy = -dy;

            if (dx < 32 && dy < 32) {
                char_apply_damage(t, 3);  // projétil faz 3 de dano
                p->active = false;
                oamSetVisible(p->oamAddress, OBJ_HIDE);
                break;
            }
        }
    }
}
