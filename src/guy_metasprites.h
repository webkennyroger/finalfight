#ifndef GUY_METASPRITES_H
#define GUY_METASPRITES_H

#include <snes.h>

#define _G (OBJ_PAL(0) | OBJ_PRIO(2))

// ============================================================================
// METASPRITE NORMAL (OLHANDO PARA A DIREITA) - STRIDE 4 (BASE 64px)
// ============================================================================
static const t_metasprite guyMetasprite[] = {
    // Linha 1 (Y = 0)
    METASPR_ITEM(0, 0, 0, _G), METASPR_ITEM(16, 0, 4, _G),
    METASPR_ITEM(32, 0, 8, _G), METASPR_ITEM(48, 0, 12, _G),

    // Linha 2 (Y = 16)
    METASPR_ITEM(0, 16, 16, _G), METASPR_ITEM(16, 16, 20, _G),
    METASPR_ITEM(32, 16, 24, _G), METASPR_ITEM(48, 16, 28, _G),

    // Linha 3 (Y = 32)
    METASPR_ITEM(0, 32, 32, _G), METASPR_ITEM(16, 32, 36, _G),
    METASPR_ITEM(32, 32, 40, _G), METASPR_ITEM(48, 32, 44, _G),

    // Linha 4 (Y = 48)
    METASPR_ITEM(0, 48, 48, _G), METASPR_ITEM(16, 48, 52, _G),
    METASPR_ITEM(32, 48, 56, _G), METASPR_ITEM(48, 48, 60, _G),

    // Linha 5 (Y = 64)
    METASPR_ITEM(0, 64, 64, _G), METASPR_ITEM(16, 64, 68, _G),
    METASPR_ITEM(32, 64, 72, _G), METASPR_ITEM(48, 64, 76, _G),

    // Linha 6 (Y = 80)
    METASPR_ITEM(0, 80, 80, _G), METASPR_ITEM(16, 80, 84, _G),
    METASPR_ITEM(32, 80, 88, _G), METASPR_ITEM(48, 80, 92, _G),

    METASPR_TERM};

// ============================================================================
// METASPRITE ESPELHADO (OLHANDO PARA A ESQUERDA) - STRIDE 4 (BASE 64px)
// ============================================================================
static const t_metasprite guyMetaspriteFlip[] = {
    // Linha 1 (Y = 0)
    METASPR_ITEM(48, 0, 0, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 0, 4, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 0, 8, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 0, 12, _G | OBJ_FLIPX),

    // Linha 2 (Y = 16)
    METASPR_ITEM(48, 16, 16, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 16, 20, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 16, 24, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 16, 28, _G | OBJ_FLIPX),

    // Linha 3 (Y = 32)
    METASPR_ITEM(48, 32, 32, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 32, 36, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 32, 40, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 32, 44, _G | OBJ_FLIPX),

    // Linha 4 (Y = 48)
    METASPR_ITEM(48, 48, 48, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 48, 52, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 48, 56, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 48, 60, _G | OBJ_FLIPX),

    // Linha 5 (Y = 64)
    METASPR_ITEM(48, 64, 64, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 64, 68, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 64, 72, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 64, 76, _G | OBJ_FLIPX),

    // Linha 6 (Y = 80)
    METASPR_ITEM(48, 80, 80, _G | OBJ_FLIPX),
    METASPR_ITEM(32, 80, 84, _G | OBJ_FLIPX),
    METASPR_ITEM(16, 80, 88, _G | OBJ_FLIPX),
    METASPR_ITEM(0, 80, 92, _G | OBJ_FLIPX),

    METASPR_TERM};

#endif // GUY_METASPRITES_H