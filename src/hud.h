#ifndef HUD_H
#define HUD_H

#include "types.h"
#include <snes.h>

#define HUD_ROWS   5
#define HUD_COLS  32
#define HUD_WORDS (HUD_ROWS * HUD_COLS)

extern u16 gHudBuffer[HUD_WORDS];
extern u8  gHudDirty;

void hud_init(const char *name);
void hud_draw(void);
void hud_draw_hp(u8 hp, u8 maxHp);
void hud_draw_timer(u8 seconds);

extern u8 gTimer;
extern u8  gTimerTicks;

#endif
