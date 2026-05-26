#include "physics.h"
#include "character.h"

void physics_start_jump(Character *c, s8 vel) {
    c->velY = vel;
}

void physics_apply(Character *c) {
    if (c->velY != 0 || c->y < c->groundY) {
        c->velY += GRAVITY;
        c->y += c->velY;
    }
    if (c->y >= c->groundY && c->velY >= 0) {
        c->y = c->groundY;
        c->velY = 0;
    }
}
