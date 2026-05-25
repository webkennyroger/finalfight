#include "vblank.h"

// ============================================================
//  vblank.c — Fila de DMA segura para tiles de sprite
//
//  Fluxo correto:
//    (código principal)  char_update_gfx → vblank_queue_sprite
//    (VBlank handler)    vblank_flush_sprite_queue → dmaCopyVram
//
//  O VBlank handler em main.c (myconsoleVblank) chama
//  vblank_flush_sprite_queue() explicitamente.
// ============================================================

typedef struct {
    u8  *data;
    u16  vramDest;
    u16  size;
} SprEntry;

#define SPR_QUEUE_SIZE  8

static SprEntry sSprQueue[SPR_QUEUE_SIZE];
static s8       sSprHead   = -1;
static u8       sSprMutex  = 0;

// ----------------------------------------------------------------
void vblank_init(void) {
    sSprHead  = -1;
    sSprMutex = 0;
}

// ----------------------------------------------------------------
//  Enfileira um upload de tile (chamado fora do VBlank)
// ----------------------------------------------------------------
void vblank_queue_sprite(u8 *data, u16 vramDest, u16 size) {
    if (sSprMutex) return;
    sSprMutex = 1;

    // Substitui a entrada mais antiga do mesmo destino (se houver)
    int i;
    for (i = 0; i <= sSprHead; i++) {
        if (sSprQueue[i].vramDest == vramDest) {
            sSprQueue[i].data = data;
            sSprQueue[i].size = size;
            sSprMutex = 0;
            return;
        }
    }

    // Adiciona no final
    if (sSprHead < SPR_QUEUE_SIZE - 1) {
        sSprHead++;
        sSprQueue[sSprHead].data     = data;
        sSprQueue[sSprHead].vramDest = vramDest;
        sSprQueue[sSprHead].size     = size;
    }

    sSprMutex = 0;
}

// ----------------------------------------------------------------
//  Descarrega a fila (chamado DENTRO do VBlank handler)
// ----------------------------------------------------------------
void vblank_flush_sprite_queue(void) {
    if (sSprMutex) return;

    while (sSprHead >= 0) {
        dmaCopyVram(
            sSprQueue[sSprHead].data,
            sSprQueue[sSprHead].vramDest,
            sSprQueue[sSprHead].size
        );
        sSprHead--;
    }
}

// ----------------------------------------------------------------
//  Stubs para scroll (scroll.c usa estas funções)
// ----------------------------------------------------------------
void vblank_queue_bg1(u8 *data, u16 vramDest, u16 size) {
    // No contexto do main.c atual, o scroll chama updateBG1/BG2
    // diretamente. Estas funções existem para compatibilidade com scroll.c.
    dmaCopyVram(data, vramDest, size);
}

void vblank_queue_bg2(u8 *data, u16 vramDest, u16 size) {
    dmaCopyVram(data, vramDest, size);
}
