#include "physics.h"
#include "character.h"

// ============================================================
//  physics.c — Gravidade e arco de pulo
//
//  O eixo Y do SNES cresce para baixo (0 = topo).
//  Pulo: gravVel começa negativo (sobe) e aumenta por GRAVITY
//  a cada frame até aterrissar em GROUND_Y.
//
//  Separado de velX/velY do Character para não conflitar com
//  o sistema de movimento horizontal do chão.
// ============================================================

void physics_start_jump(Character *c, s8 vel) {
    c->gravVel    = vel;         // negativo = sobe
    c->isAirborne = true;
}

void physics_apply(Character *c) {
    if (!c->isAirborne) return;

    // Aplica gravidade
    c->gravVel += GRAVITY;

    // Move verticalmente
    c->y += c->gravVel;

    // Pouso: chegou ao nível do chão e está descendo
    if (c->y >= GROUND_Y && c->gravVel > 0) {
        c->y          = GROUND_Y;
        c->gravVel    = 0;
        c->isAirborne = false;

        // Volta ao idle (ou walk se ainda está se movendo)
        if (c->velX != 0)
            char_set_state(c, STATE_WALK, GUY_WALK_1);
        else
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
    }
}
