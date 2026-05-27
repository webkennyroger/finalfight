#include "hud.h"
#include <snes.h>
#include <string.h>

u16 gHudBuffer[HUD_WORDS];
u8  gHudDirty;

u8 gTimer = 99;
u8  gTimerTicks = 0;

// Map = 0x2000 base; BG3 tiles loaded at 0x4000
#define MAP_BASE    0x0000

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

#define TENS_TL      24  // row 0, col 24 (TL)
#define TENS_TR      25  // row 0, col 25 (TR)
#define TENS_BL      56  // row 1, col 24 (BL)
#define TENS_BR      57  // row 1, col 25 (BR)
#define ONES_TL      26  // row 0, col 26 (TL)
#define ONES_TR      27  // row 0, col 27 (TR)
#define ONES_BL      58  // row 1, col 26 (BL)
#define ONES_BR      59  // row 1, col 27 (BR)

#define DIGIT_TILES  104
#define LETTER_TILES 78
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
    u16 bt = MAP_BASE + DIGIT_TILES + tens * 4;
    u16 bo = MAP_BASE + DIGIT_TILES + ones * 4;
    gHudBuffer[TENS_TL] = bt;
    gHudBuffer[TENS_TR] = bt + 1;
    gHudBuffer[TENS_BL] = bt + 2;
    gHudBuffer[TENS_BR] = bt + 3;
    gHudBuffer[ONES_TL] = bo;
    gHudBuffer[ONES_TR] = bo + 1;
    gHudBuffer[ONES_BL] = bo + 2;
    gHudBuffer[ONES_BR] = bo + 3;
    gHudDirty = 1;
    sLastTimerSeconds = seconds;
}

void hud_draw(void) {
    if (!gHudDirty) return;
    dmaCopyVram((u8*)gHudBuffer, 0x1000, HUD_WORDS * 2);
    gHudDirty = 0;
}
