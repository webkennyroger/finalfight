#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "types.h"

// Tiles do projétil (definidos no data.asm)
extern char gora_tiles,   gora_tiles_end;
extern char gora_palette, gora_palette_end;

#define PROJ_VRAM      0x7E00   // endereço VRAM dos tiles do projétil
#define PROJ_TILE_SIZE 0x200    // 32×32 px, 4bpp → 512 bytes (2×2 tiles de 16×16)
#define PROJ_OAM_BASE  96       // primeiro slot OAM reservado para projéteis
#define PROJ_SPEED     5        // pixels/frame
#define PROJ_LIFETIME  90       // frames antes de sumir

// Inicializa o sistema (carrega tiles na VRAM)
void projectile_init(void);

// Dispara um projétil a partir da posição do personagem
void projectile_fire(Character *c);

// Atualiza movimento e expiração de todos os projéteis
void projectile_update_all(void);

// Verifica colisão dos projéteis com inimigos
void projectile_check_hits(void);

#endif
