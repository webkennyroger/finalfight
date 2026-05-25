#include "collision.h"
#include "character.h"

// ============================================================
//  final_fight_snes - Colisão e Hitboxes
// ============================================================

// Tolerância de eixo Y para considerar "mesmo plano"
#define SAME_PLANE_Y  6

// ----------------------------------------------------------------
//  Testa se o retângulo A sobrepõe o retângulo B
//  (todos valores em coordenadas de tela)
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

        // Hitbox do atacante em coordenadas de tela
        int ax1 = attacker->x + (attacker->hflip ? (63 - af->hitboxX2) : af->hitboxX1);
        int ax2 = attacker->x + (attacker->hflip ? (63 - af->hitboxX1) : af->hitboxX2);
        int ay1 = attacker->y - af->hitboxY2;
        int ay2 = attacker->y - af->hitboxY1;

        for (j = 0; j < MAX_CHARS; j++) {
            if (i == j) continue;

            Character *target = &gCharacters[j];
            if (!target->alive)             continue;
            if (target->isInvincible)       continue;
            // Inimigos não batem em si mesmos
            if (attacker->isEnemy && target->isEnemy) continue;

            // Mesmo plano Y?
            int dy = attacker->y - target->y;
            if (dy < 0) dy = -dy;
            if (dy > SAME_PLANE_Y) continue;

            SpriteFrame *tf = &target->frames[target->subState];

            int tx1 = target->x + tf->hitboxX1;
            int tx2 = target->x + tf->hitboxX2;
            int ty1 = target->y - tf->hitboxY2;
            int ty2 = target->y - tf->hitboxY1;

            if (_rects_overlap(ax1, ax2, ay1, ay2, tx1, tx2, ty1, ty2)) {
                char_apply_damage(target, 1);
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

            // Se sobrepostos, empurra ligeiramente
            if (dx < 20 && dy < 10) {
                if (p1->x < p2->x)  { p1->x--; p2->x++; }
                else                 { p1->x++; p2->x--; }

                // Atualiza OAM order se necessário
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
