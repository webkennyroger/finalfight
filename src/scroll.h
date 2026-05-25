#ifndef SCROLL_H
#define SCROLL_H

#include "types.h"

// ============================================================
//  final_fight_snes - Scrolling de Background
//  Camadas BG1 (principal) e BG2 (paralaxe)
// ============================================================

// Tiles e mapas do cenário (fase 1 - rua de Metro City)
extern char bg1_tiles,  bg1_tiles_end;
extern char bg1_palette,bg1_palette_end;
extern char bg1_map,    bg1_map_end;

extern char bg2_tiles,  bg2_tiles_end;
extern char bg2_palette,bg2_palette_end;
extern char bg2_map,    bg2_map_end;

// Inicializa os dois scrolls de background
void scroll_init(void);

// Atualiza scroll em função da posição do jogador
void scroll_update(Character *player);

#endif // SCROLL_H
