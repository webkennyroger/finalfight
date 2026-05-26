#include "projectile.h"
#include "character.h"
#include "vblank.h"

extern Projectile gProjectiles[MAX_PROJECTILES];

void projectile_init(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        gProjectiles[i].active = false;
        gProjectiles[i].life = 0;
    }
}

void projectile_fire(Character *c) {
    (void)c;
}

void projectile_update_all(void) {
    int i;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;
        p->x += p->velX;
        p->life--;
        if (p->life == 0 || p->x < -32 || p->x > 280) {
            p->active = false;
            oamSetVisible(p->oamId, OBJ_HIDE);
        }
    }
}

void projectile_check_hits(void) {
    int i, j;
    for (i = 0; i < MAX_PROJECTILES; i++) {
        Projectile *p = &gProjectiles[i];
        if (!p->active) continue;
        for (j = 1; j < MAX_CHARS; j++) {
            Character *t = &gCharacters[j];
            if (!t->alive || t->isInvincible || !t->isEnemy) continue;
            int dx = (p->x + 16) - (t->x + 16);
            if (dx < 0) dx = -dx;
            int dy = (p->y + 16) - (t->y - 32);
            if (dy < 0) dy = -dy;
            if (dx < 32 && dy < 32) {
                char_apply_damage(t, p->damage);
                p->active = false;
                oamSetVisible(p->oamId, OBJ_HIDE);
                break;
            }
        }
    }
}
