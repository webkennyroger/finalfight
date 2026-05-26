#include "hud.h"
#include <snes.h>

extern char HUD_map, HUD_map_end;

u16 gHudBuffer[HUD_WORDS];
u8  gHudDirty;

u16 gTimer = 99;
u8  gTimerTicks = 0;

#define MAP_BASE    0x2000

#define HP_START    80
#define HP_WIDTH    16
#define HP_FILLED   80
#define HP_EMPTY    64

// Timer: row 0, cols 24-25 (buffer indices 24-25)
#define TENS_POS    24
#define ONES_POS    25
#define DIGIT_TILES 160

static u8 sLastTimerSeconds = 0xFF;

void hud_init(void) {
    u16 *src = (u16*)&HUD_map;
    int i;
    for (i = 0; i < HUD_WORDS; i++)
        gHudBuffer[i] = src[i];
    gHudDirty = 1;
    sLastTimerSeconds = 0xFF;
}

void hud_draw_hp(u8 hp, u8 maxHp) {
    u8 i;
    u8 filled = (maxHp > 0) ? (u8)((u16)hp * HP_WIDTH / maxHp) : 0;
    for (i = 0; i < HP_WIDTH; i++)
        gHudBuffer[HP_START + i] = MAP_BASE + (i < filled ? HP_FILLED : HP_EMPTY);
    gHudDirty = 1;
}

void hud_draw_timer(u8 seconds) {
    if (seconds == sLastTimerSeconds) return;
    u8 tens = seconds / 10;
    u8 ones = seconds % 10;
    gHudBuffer[TENS_POS] = MAP_BASE + DIGIT_TILES + tens;
    gHudBuffer[ONES_POS] = MAP_BASE + DIGIT_TILES + ones;
    gHudDirty = 1;
    sLastTimerSeconds = seconds;
}

void hud_draw(void) {
    if (!gHudDirty) return;
    dmaCopyVram((u8*)gHudBuffer, 0x1000, HUD_WORDS * 2);
    gHudDirty = 0;
}
