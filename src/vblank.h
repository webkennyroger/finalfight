#ifndef VBLANK_H
#define VBLANK_H

#include "types.h"

// ============================================================
//  vblank.h — Fila de DMA de tiles de sprite
// ============================================================

void vblank_init(void);

// Chamado pelo myconsoleVblank no MAIN para descarregar a fila
void vblank_flush_sprite_queue(void);

// Chamado por char_update_gfx para enfileirar um upload de tile
void vblank_queue_sprite(u8 *data, u16 vramDest, u16 size);

// Chamado pelo código de scroll para atualizar mapas de BG
void vblank_queue_bg1(u8 *data, u16 vramDest, u16 size);
void vblank_queue_bg2(u8 *data, u16 vramDest, u16 size);

#endif
