#include "scroll.h"
#include "vblank.h"

// ============================================================
//  final_fight_snes - Scrolling de Background
//
//  BG1 = cenário principal (rua)   - scroll 1:1 com jogador
//  BG2 = fundo distante (prédios)  - scroll paralaxe 1:2
//
//  O mapa é maior que uma tela → usa "ping-pong" de páginas VRAM:
//  Quando o scroll cruza 256px, a página inativa é recarregada
//  com o próximo bloco de 32 tiles (2048 bytes).
// ============================================================

#define BG1_VRAM_PAGE_A  0x2000   // 1ª página VRAM do BG1
#define BG1_VRAM_PAGE_B  0x2400   // 2ª página VRAM do BG1
#define BG2_VRAM_PAGE_A  0x4000
#define BG2_VRAM_PAGE_B  0x4400

// Ponto de tela após o qual começa o scroll
#define SCROLL_TRIGGER_X  94

static ScrollBG sBG1;
static ScrollBG sBG2;

// ----------------------------------------------------------------
void scroll_init(void) {
    WaitForVBlank();

    // --- BG1: rua principal ---
    sBG1.bgLayer    = 0;
    sBG1.vramAddress= BG1_VRAM_PAGE_A;
    sBG1.scrollX    = 255;
    sBG1.scrollY    = 223;
    sBG1.mapData    = &bg1_map;
    sBG1.mapPage    = 0;
    sBG1.firstPage  = true;
    sBG1.maxScrollX = 1024;   // largura total do mapa - 256
    sBG1.scrollSpeed= 1;

    // --- BG2: fundo distante ---
    sBG2.bgLayer    = 1;
    sBG2.vramAddress= BG2_VRAM_PAGE_A;
    sBG2.scrollX    = 255;
    sBG2.scrollY    = 223;
    sBG2.mapData    = &bg2_map;
    sBG2.mapPage    = 0;
    sBG2.firstPage  = true;
    sBG2.maxScrollX = 512;
    sBG2.scrollSpeed= 2;   // paralaxe mais lento

    // Carrega tiles e mapas iniciais
    dmaCopyVram(&bg1_tiles,  0x0000, &bg1_tiles_end  - &bg1_tiles);
    dmaCopyVram(&bg2_tiles,  0x1000, &bg2_tiles_end  - &bg2_tiles);
    dmaCopyCGram(&bg1_palette, BG_16COLORS * 2, &bg1_palette_end - &bg1_palette);
    dmaCopyCGram(&bg2_palette, BG_16COLORS * 4, &bg2_palette_end - &bg2_palette);

    // Primeira página de mapa
    dmaCopyVram(sBG1.mapData, BG1_VRAM_PAGE_A, 2048);
    dmaCopyVram(sBG2.mapData, BG2_VRAM_PAGE_A, 2048);
}

// ----------------------------------------------------------------
//  Atualiza um scroll e enfileira nova página quando necessário
// ----------------------------------------------------------------
static void _update_bg(ScrollBG *s, bool isBG1) {
    if (s->scrollX >= s->maxScrollX) return;

    s->scrollX += s->scrollSpeed;

    // Cruzou limite de página → carrega próximo bloco do mapa
    if (s->scrollX >= (u16)((s->mapPage + 1) * 256)) {
        s->mapPage++;
        s->firstPage = !s->firstPage;

        u16 vramTarget = s->vramAddress;
        if (!s->firstPage) vramTarget += 1024;  // página B

        u8 *newMapData = s->mapData + 2048 * s->mapPage;

        if (isBG1)
            vblank_queue_bg1(newMapData, vramTarget, 2048);
        else
            vblank_queue_bg2(newMapData, vramTarget, 2048);
    }

    // Aplica scroll ao hardware (ajuste para SNES: subtraímos posição base)
    bgSetScroll(s->bgLayer, s->scrollX - 255, s->scrollY - 223);
}

// ----------------------------------------------------------------
void scroll_update(Character *player) {
    // Só scrolla quando jogador passa do ponto de trigger
    if (player->x <= SCROLL_TRIGGER_X) return;

    // Move todos os personagens junto com o scroll do BG1
    if (sBG1.scrollX < sBG1.maxScrollX) {
        int i;
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive) {
                gCharacters[i].x -= sBG1.scrollSpeed;
            }
        }
    }

    _update_bg(&sBG1, true);
    _update_bg(&sBG2, false);
}
