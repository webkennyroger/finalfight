#ifndef COLLISION_H
#define COLLISION_H

#include "types.h"

// ============================================================
//  final_fight_snes - Detecção de Colisão / Hitbox
// ============================================================

// Verifica hitboxes de ataque: se algum personagem está atacando
// e acerta outro, aplica dano via char_apply_damage()
void collision_check_hitboxes(void);

// Evita sobreposição física entre personagens (empurra)
void collision_resolve_overlap(void);

#endif // COLLISION_H
