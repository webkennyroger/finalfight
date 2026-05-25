#include "hud.h"
#include <snes.h>

// ============================================================
//  hud.c - Timer do Final Fight
//
//  Arquitetura:
//  - BG3_map carregado UMA VEZ em VRAM 0x1000 (no main.c)
//    → contém o HUD estático (barra HP, retrato, texto "TIME")
//
//  - O timer (2 dígitos) é atualizado DINAMICAMENTE:
//    - gTimerTiles[2] guarda os 2 tile-entries do tilemap
//    - gTimerDirty sinaliza ao VBlank que precisa enviar para VRAM
//
//  Posição dos dígitos no BG3 map (SC_32x32, 32 tiles/linha):
//    - Dezena: linha 0, coluna HUD_COL_TENS
//    - Unidade: linha 0, coluna HUD_COL_UNITS
//  Ajuste HUD_COL_TENS/UNITS para coincidir com seu stage1_bg3.bmp
//
//  Tile indices no tileset (stage1_bg3.pic, carregado em VRAM 0x4000):
//    Dígitos 0-9 devem estar sequencialmente no tileset.
//    HUD_DIGIT_BASE = índice do tile que representa o dígito "0".
//    Ajuste conforme o layout do seu bg3.bmp.
// ============================================================

// --- Posição do timer no BG3 map (colunas, linha 0) ---
// Ajuste para bater com onde você quer o timer no seu bg3.bmp
#define HUD_COL_TENS    14   // coluna da dezena (0-31)
#define HUD_COL_UNITS   15   // coluna da unidade

// --- Índice do tile "0" no tileset BG3 ---
// Ajuste conforme onde estão os dígitos no seu stage1_bg3.bmp
#define HUD_DIGIT_BASE  16   // tile 16 = dígito "0", 17 = "1", ..., 25 = "9"

// --- Atributo de paleta BG3 (paleta 0, prioridade 0) ---
#define HUD_TILE_ATTR   0x0000

// ============================================================

// VRAM address of each timer digit tile in BG3 map
// BG3 map base = 0x1000, each tile = 2 bytes, SC_32x32 = 32 tiles/row
#define VRAM_TIMER_TENS  (0x1000 + (0 * 32 + HUD_COL_TENS)  * 2)
#define VRAM_TIMER_UNITS (0x1000 + (0 * 32 + HUD_COL_UNITS) * 2)

// Tile entries para os 2 dígitos do timer
// [0] = dezena, [1] = unidade
u16 gTimerTiles[2];

// Flag: 1 = VBlank deve copiar gTimerTiles para VRAM
u8 gTimerDirty;

// Timer interno
static u16 sTimer     = 99;
static u16 sTimerTick = 0;

#define TIMER_TICK  60   // 60 frames = 1 segundo

// ----------------------------------------------------------------
void hud_init(void)
{
    sTimer     = 99;
    sTimerTick = 0;
    gTimerDirty = 0;

    // Força atualização inicial dos dígitos
    hud_update();
}

// ----------------------------------------------------------------
void hud_set_timer(u16 value)
{
    sTimer     = value;
    sTimerTick = 0;
    hud_update();
}

// ----------------------------------------------------------------
u16 hud_tick_timer(void)
{
    if (sTimer == 0) return 0;

    sTimerTick++;
    if (sTimerTick >= TIMER_TICK)
    {
        sTimerTick = 0;
        if (sTimer > 0) sTimer--;
        hud_update();
    }
    return sTimer;
}

// ----------------------------------------------------------------
void hud_update(void)
{
    u8 tens  = (u8)(sTimer / 10);
    u8 units = (u8)(sTimer % 10);

    // Monta as entradas do tilemap BG3:
    // bits 9-0  = índice do tile no tileset
    // bits 13-10 = paleta (0)
    // bit 14    = flip-H
    // bit 15    = flip-V
    gTimerTiles[0] = (HUD_DIGIT_BASE + tens)  | HUD_TILE_ATTR;
    gTimerTiles[1] = (HUD_DIGIT_BASE + units) | HUD_TILE_ATTR;

    gTimerDirty = 1;   // avisa o VBlank handler
}