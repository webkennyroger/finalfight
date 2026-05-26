#ifndef CHARACTER_H
#define CHARACTER_H

#include "types.h"

// Altura do canvas do sprite em pixels (usada para alinhar OAM Y)
#define CANVAS_H   96

// Tamanho máximo de um tile de todos os personagens (Guy: 3072)
#define TILE_BUF_SIZE  3072

void char_init(Character *c, u16 tileSize, u16 paletteSize);
void char_load_gfx(Character *c, u8 oamAddr, u16 vramAddr, u16 vramOffset, u8 paletteSlot);
void char_set_state(Character *c, u8 state, u8 subState);
void char_update_gfx(Character *c);
void char_update_pos(Character *c);
void char_update_pos_npc(Character *c);
void char_set_velocity(Character *c, s8 vx, s8 vy);
void char_apply_damage(Character *c, u8 dmg);
void char_update_hurt(Character *c);
void char_sort_y_order(void);
void char_swap_oam(Character *p1, Character *p2);

#endif
