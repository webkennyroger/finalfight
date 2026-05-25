#ifndef HUD_H
#define HUD_H

#include <snes.h>

// ============================================================
//  hud.h
//  Exporta os buffers de timer usados pelo VBlank handler
// ============================================================

// Tile entries do timer (dezena e unidade) – escritos no VBlank
extern u16 gTimerTiles[2];

// Flag: VBlank handler copia gTimerTiles para VRAM quando = 1
extern u8  gTimerDirty;

// API pública
void hud_init(void);
void hud_set_timer(u16 value);
u16  hud_tick_timer(void);
void hud_update(void);

#endif // HUD_H