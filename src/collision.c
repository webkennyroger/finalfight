#include "collision.h"
#include "character.h"

// ============================================================
//  final_fight_snes - Colisão, Hitboxes e Projéteis
// ============================================================

#define SAME_PLANE_Y  6

Projectile gProjectiles[MAX_PROJECTILES];

// ----------------------------------------------------------------
static bool _rects_overlap(int ax1, int ax2, int ay1, int ay2,
                            int bx1, int bx2, int by1, int by2) {
    return (ax1 <= bx2 && ax2 >= bx1 &&
            ay1 <= by2 && ay2 >= by1);
}

// ----------------------------------------------------------------
void collision_check_hitboxes(void) {
    int i, j;

    for (i = 0; i < MAX_CHARS; i++) {
        Character *attacker = &gCharacters[i];
        if (!attacker->alive) continue;

        SpriteFrame *af = &attacker->frames[attacker->subState];
        if (!af->hasHitbox) continue;

        int ax1 = attacker->x + (attacker->hflip ? (63 - af->hitboxX2) : af->hitboxX1);
        int ax2 = attacker->x + (attacker->hflip ? (63 - af->hitboxX1) : af->hitboxX2);
        int ay1 = attacker->y - af->hitboxY2;
        int ay2 = attacker->y - af->hitboxY1;

        for (j = 0; j < MAX_CHARS; j++) {
            if (i == j) continue;

            Character *target = &gCharacters[j];
            if (!target->alive)             continue;
            if (target->isInvincible)       continue;
            if (attacker->isEnemy && target->isEnemy) continue;

            int dy = attacker->y - target->y;
            if (dy < 0) dy = -dy;
            if (dy > SAME_PLANE_Y) continue;

            // Use target's hitbox if available, else full body
            SpriteFrame *tf = &target->frames[target->subState];
            int tx1, tx2, ty1, ty2;
            if (tf->hasHitbox) {
                tx1 = target->x + (target->hflip ? (63 - tf->hitboxX2) : tf->hitboxX1);
                tx2 = target->x + (target->hflip ? (63 - tf->hitboxX1) : tf->hitboxX2);
                ty1 = target->y - tf->hitboxY2;
                ty2 = target->y - tf->hitboxY1;
            } else {
                tx1 = target->x;
                tx2 = target->x + 63;
                ty1 = target->y - 63;
                ty2 = target->y;
            }

            if (_rects_overlap(ax1, ax2, ay1, ay2, tx1, tx2, ty1, ty2)) {
                char_apply_damage(target, 1);
            }
        }
    }

    // Also check projectiles vs characters
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;

        for (j = 0; j < MAX_CHARS; j++) {
            Character *target = &gCharacters[j];
            if (!target->alive) continue;
            if (target->isInvincible) continue;
            if (p->friendly && !target->isEnemy) continue;
            if (!p->friendly && target->isEnemy) continue;

            int px1 = p->x, px2 = p->x + 16;
            int py1 = p->y, py2 = p->y + 16;

            int tx1 = target->x, tx2 = target->x + 63;
            int ty1 = target->y - 63, ty2 = target->y;

            if (_rects_overlap(px1, px2, py1, py2, tx1, tx2, ty1, ty2)) {
                char_apply_damage(target, p->damage);
                p->active = false;
                p->life = 0;
                oamSetVisible(p->oamId, OBJ_HIDE);
                break;
            }
        }
    }
}

// ----------------------------------------------------------------
void collision_resolve_overlap(void) {
    int i, j;

    for (i = 0; i < MAX_CHARS; i++) {
        if (!gCharacters[i].alive) continue;

        for (j = i + 1; j < MAX_CHARS; j++) {
            if (!gCharacters[j].alive) continue;

            Character *p1 = &gCharacters[i];
            Character *p2 = &gCharacters[j];

            int dx = p1->x - p2->x;
            if (dx < 0) dx = -dx;
            int dy = p1->y - p2->y;
            if (dy < 0) dy = -dy;

            if (dx < 20 && dy < 10) {
                if (p1->x < p2->x)  { p1->x--; p2->x++; }
                else                 { p1->x++; p2->x--; }

                if (p1->y < p2->y) {
                    if (p1->oamAddress < p2->oamAddress)
                        char_swap_oam(p1, p2);
                } else {
                    if (p2->oamAddress < p1->oamAddress)
                        char_swap_oam(p1, p2);
                }
            }
        }
    }
}

// ----------------------------------------------------------------
void collision_update_projectiles(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;

        p->x += p->velX;
        p->life--;

        // Move OAM sprite
        oamSetXY(p->oamId, p->x, p->y);

        // Destroy if off-screen or expired
        if (p->life == 0 || p->x < -32 || p->x > 256) {
            p->active = false;
            oamSetVisible(p->oamId, OBJ_HIDE);
        }
    }
}

// ----------------------------------------------------------------
void collision_spawn_projectile(int x, int y, int velX, u8 damage, bool friendly) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (p->active) continue;

        p->x = x;
        p->y = y;
        p->velX = velX;
        p->damage = damage;
        p->life = 60;
        p->active = true;
        p->friendly = friendly;
        p->hflip = (velX < 0);

        // Use OAM entries 120-127 (projectile area)
        p->oamId = 120 + i * 4;
        oamSet(p->oamId, x, y, 3, p->hflip, 0, 0, 0);
        oamSetEx(p->oamId, OBJ_SMALL, OBJ_SHOW);
        return;
    }
}

// ----------------------------------------------------------------
void collision_init_projectiles(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        gProjectiles[i].active = false;
        gProjectiles[i].life = 0;
    }
}