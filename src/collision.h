#ifndef COLLISION_H
#define COLLISION_H

#include "types.h"

// ============================================================
//  final_fight_snes - Detecção de Colisão / Hitbox / Projéteis
// ============================================================

// Verifica hitboxes de ataque + projéteis vs personagens
void collision_check_hitboxes(void);

// Evita sobreposição física entre personagens (empurra)
void collision_resolve_overlap(void);

// Atualiza movimento e tempo de vida dos projéteis
void collision_update_projectiles(void);

// Inicializa array de projéteis (OAM entries 120-127)
void collision_init_projectiles(void);

// Cria um novo projétil
void collision_spawn_projectile(int x, int y, int velX, u8 damage, bool friendly);

#endif // COLLISION_H
