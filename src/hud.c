#include "hud.h"
#include <snes.h>
#include <string.h>

u16 gHudBuffer[HUD_WORDS];
u8  gHudDirty;

u16 gTimer = 99;
u8  gTimerTicks = 0;

// Map = 0x2000 base; BG3 tiles loaded at 0x4000
#define MAP_BASE    0x2000

// Layout: sor2-style — clear all, only write name + HP + timer
// Row 0: [....................[99]]    timer at cols 24-25
// Row 1: [GUY]...[████████████]       name cols 0-2, HP bar cols 8-23
// Row 2-4: all blank

#define NAME_POS     32  // row 1, col 0
#define NAME_LEN_MAX  8

#define HP_START     40  // row 1, col 8
#define HP_WIDTH     16
#define HP_FILLED    80
#define HP_EMPTY     64

#define TENS_POS     24  // row 0, col 24
#define ONES_POS     25  // row 0, col 25

#define DIGIT_TILES  160
#define LETTER_TILES 170
#define BLANK_TILE   2

static u8 sLastTimerSeconds = 0xFF;

void hud_init(const char *name) {
    u16 blank = MAP_BASE + BLANK_TILE;
    u16 *buf = gHudBuffer;
    u16 i;

    // Fill entire buffer with blank tile (sor2-style)
    for (i = 0; i < HUD_WORDS; i++)
        buf[i] = blank;

    // Write character name at NAME_POS
    for (i = 0; i < NAME_LEN_MAX; i++) {
        char c = name[i];
        if (c == '\0') break;
        if (c >= 'A' && c <= 'Z')
            buf[NAME_POS + i] = MAP_BASE + LETTER_TILES + (c - 'A');
    }

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
