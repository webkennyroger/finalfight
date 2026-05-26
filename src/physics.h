#ifndef PHYSICS_H
#define PHYSICS_H

#include "types.h"

// Aplica gravidade e detecta pouso no chão
void physics_apply(Character *c);

// Inicia pulo (define gravVel inicial e isAirborne)
void physics_start_jump(Character *c, s8 vel);

#endif
