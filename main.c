/*
    Final Fight SNES — main.c
    Baseado no exemplo Mode1ContinuousScroll (odelot/sor2_snes)

    FIX GEMINI #1-3 aplicados:
      • Máquina de estados correta em player.c/character.c
      • Loop principal usa gPlayer (não mais player1 separado)
      • VBlank processa fila de DMA de tiles de sprite
      • Metasprite dinâmico com flip horizontal automático
*/
#include <snes.h>
#include "src/types.h"
#include "src/character.h"
#include "src/player.h"
#include "src/enemy.h"
#include "src/collision.h"
#include "src/hud.h"
#include "src/vblank.h"
#include "src/guy_metasprites.h"

// ============================================================
//  Assets (definidos em data.asm)
// ============================================================
extern char BG1_tiles, BG1_tiles_end;
extern char BG1_pal,   BG1_pal_end;
extern char BG1_map,   BG1_map_end;

extern char BG2_tiles, BG2_tiles_end;
extern char BG2_pal,   BG2_pal_end;
extern char BG2_map,   BG2_map_end;

extern char BG3_tiles, BG3_tiles_end;
extern char BG3_pal,   BG3_pal_end;
extern char BG3_map,   BG3_map_end;

// Tiles do Guy carregados via gPlayer (player_init define frames[])
extern char guy_idle_tiles,  guy_idle_tiles_end;
extern char guy_palette,     guy_palette_end;

// ============================================================
//  Scroll (estruturas do exemplo sor2_snes)
// ============================================================
typedef struct {
    u8   id;
    u16  vram;
    u16  scrX, scrY;
    u8  *bgMap;
    u8   mapPage;
    bool vRamFirstPage;
    u16  maxScrollX;
} BGScroll;

typedef struct {
    u8  *gfxoffset;
    u16  adrgfxvram;
    u16  size;
} BGPage;

typedef struct {
    BGPage bg1, bg2;
    u8 refreshBG1, refreshBG2;
} BGInfo;

static BGScroll bgMain, bgSub;
static BGInfo   bgInfo;

// ============================================================
//  VBlank — processa DMA de BG, HUD e fila de tiles de sprite
// ============================================================
void myconsoleVblank(void) {
    if (!vblank_flag) return;

    // 1) OAM → hardware (pvsneslib já faz isso via WaitForVBlank,
    //    mas incluímos explicitamente para garantir)
    dmaCopyOAram((unsigned char*)&oamMemory, 0, 0x220);

    // 2) Scroll BG1
    if (bgInfo.refreshBG1) {
        dmaCopyVram(bgInfo.bg1.gfxoffset, bgInfo.bg1.adrgfxvram, bgInfo.bg1.size);
        bgInfo.refreshBG1 = 0;
    }

    // 3) Scroll BG2
    if (bgInfo.refreshBG2) {
        dmaCopyVram(bgInfo.bg2.gfxoffset, bgInfo.bg2.adrgfxvram, bgInfo.bg2.size);
        bgInfo.refreshBG2 = 0;
    }

    // 4) Timer HUD (2 tiles do BG3)
    if (gTimerDirty) {
        dmaCopyVram((u8*)&gTimerTiles[0], 0x1000 + (0*32+14)*2, 2);
        dmaCopyVram((u8*)&gTimerTiles[1], 0x1000 + (0*32+15)*2, 2);
        gTimerDirty = 0;
    }

    // 5) Fila de tiles de sprite (DMA dos frames de animação)
    //    Cada entrada tem ponteiro ROM → VRAM destino + tamanho
    vblank_flush_sprite_queue();
}

// ============================================================
//  BG helpers
// ============================================================
static void updateBG1(u8 *data, u16 vram, int size) {
    bgInfo.bg1.gfxoffset  = data;
    bgInfo.bg1.adrgfxvram = vram;
    bgInfo.bg1.size       = size;
    bgInfo.refreshBG1     = 1;
}

static void updateBG2(u8 *data, u16 vram, int size) {
    bgInfo.bg2.gfxoffset  = data;
    bgInfo.bg2.adrgfxvram = vram;
    bgInfo.bg2.size       = size;
    bgInfo.refreshBG2     = 1;
}

static void handleScroll(BGScroll *s, int playerX) {
    if (playerX <= 94) return;
    if (s->scrX < s->maxScrollX) s->scrX++;

    if (s->scrX >= (u16)((s->mapPage + 1) * 256)) {
        s->mapPage++;
        s->vRamFirstPage = !s->vRamFirstPage;
        u16 vram = s->vram + (s->vRamFirstPage ? 0 : 1024);
        if (s->id == 0) updateBG1(&BG1_map + 2048 * s->mapPage, vram, 2048);
        else            updateBG2(&BG2_map + 2048 * s->mapPage, vram, 2048);
    }
    bgSetScroll(s->id, s->scrX - 255, s->scrY - 223);
}

// ============================================================
//  FIX GEMINI #2: desenha metasprite correto baseado no gPlayer
//
//  Como o DMA já trocou o conteúdo do VRAM para o frame atual,
//  o mesmo metasprite (guyMetasprite / guyMetaspriteFlip) serve
//  para todas as animações.
// ============================================================
static void drawPlayer(void) {
    const t_metasprite *meta = gPlayer->hflip
                               ? guyMetaspriteFlip
                               : guyMetasprite;

    oamMetaDraw16(
        gPlayer->oamAddress,
        gPlayer->x,
        gPlayer->y - (CANVAS_H - 1),   // âncora no pé do personagem
        (u8*)meta,
        OBJ_SMALL,
        0
    );
}

// ============================================================
//  MAIN
// ============================================================
int main(void) {

    // --- Tilemaps ---
    bgSetMapPtr(0, 0x0000,        SC_64x32);
    bgSetMapPtr(1, 0x0000 + 2048, SC_64x32);
    bgSetMapPtr(2, 0x1000,        SC_32x32);   // BG3 = HUD

    // --- Tilesets ---
    bgInitTileSet(0, &BG1_tiles, &BG1_pal, 2,
                  (&BG1_tiles_end - &BG1_tiles), 16*4, BG_16COLORS, 0x2000);
    bgInitTileSet(1, &BG2_tiles, &BG2_pal, 4,
                  (&BG2_tiles_end - &BG2_tiles), 16*4, BG_16COLORS, 0x3000);
    bgInitTileSet(2, &BG3_tiles, &BG3_pal, 0,
                  (&BG3_tiles_end - &BG3_tiles), 16*4, BG_4COLORS,  0x4000);

    // --- Primeiros mapas de scroll ---
    updateBG1(&BG1_map, 0x0000,        2048);
    updateBG2(&BG2_map, 0x0000 + 2048, 2048);

    // --- BG3 HUD estático (copiado uma vez) ---
    WaitForVBlank();
    dmaCopyVram(&BG3_map, 0x1000, 2048);
    setMode(BG_MODE1, BG3_MODE1_PRIORITY_HIGH);

    // --- Sprites: configura modo OBJ (small=16×16, large=32×32) ---
    // Tiles do Guy idle carregados no VRAM 0x6000 para inicialização
    oamInitGfxSet(
        &guy_idle_tiles,
        (&guy_idle_tiles_end - &guy_idle_tiles),
        &guy_palette,
        (&guy_palette_end    - &guy_palette),
        0,        // OBJ palette slot 0
        0x6000,   // base dos tiles no VRAM (pvsneslib word addr)
        OBJ_SIZE16_L32
    );

    // --- Inicializa sistema de personagens ---
    int i;
    for (i = 0; i < MAX_CHARS; i++) gYOrder[i] = NULL;

    gPlayer = &gCharacters[0];
    char_init(gPlayer, GUY_TILE_SIZE, 32);
    player_init(gPlayer);
    gPlayer->x = 48;
    gPlayer->y = 160;

    // char_load_gfx: faz upload do frame inicial, da paleta, chama
    // oamSet + oamSetEx(OBJ_SMALL, OBJ_SHOW) para tornar visível
    char_load_gfx(gPlayer, 0, 0x6000, 0, 0);

    // --- HUD ---
    hud_init();

    // --- VBlank handler customizado ---
    vblank_init();
    nmiSet(myconsoleVblank);

    setScreenOn();

    // --- Scroll setup ---
    bgMain.id           = 0;
    bgMain.scrX         = 255;
    bgMain.scrY         = 255;
    bgMain.vram         = 0x0000;
    bgMain.vRamFirstPage= true;
    bgMain.mapPage      = 0;
    bgMain.maxScrollX   = 768;

    bgSub.id            = 1;
    bgSub.scrX          = 255;
    bgSub.scrY          = 255;
    bgSub.vram          = 0x0000 + 2048;
    bgSub.vRamFirstPage = true;
    bgSub.mapPage       = 0;
    bgSub.maxScrollX    = 1024 + 128;

    bgSetScroll(0, 0, 32);
    bgSetScroll(1, 0, 32);

    // ============================================================
    //  GAME LOOP
    // ============================================================
    while (1) {

        // 1. Input
        u16 pad = padsCurrent(0);

        // 2. Lógica do jogador (estado + velocidade)
        player_update(gPlayer, pad);

        // 3. Física: aplica velocidade à posição
        char_update_pos(gPlayer);

        // 4. Animação: tick do frame + enfileira DMA se mudou frame
        char_update_gfx(gPlayer);

        // 5. Scroll (move BG junto com jogador)
        handleScroll(&bgMain, gPlayer->x);
        handleScroll(&bgSub,  gPlayer->x);

        // 6. HUD: decrementa timer 1×/segundo
        hud_tick_timer();

        // 7. Desenha metasprite na OAM (espelhado automaticamente)
        drawPlayer();

        // 8. Aguarda VBlank (DMA de OAM + tiles + BG acontece aqui)
        WaitForVBlank();
    }

    return 0;
}
