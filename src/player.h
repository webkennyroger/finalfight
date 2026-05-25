#ifndef PLAYER_H
#define PLAYER_H

#include "types.h"

// ============================================================
//  player.h - Jogador Guy
//
//  Cada animação é um strip PNG convertido por:
//    gfx2snes -s 16 -o 0 -u 16 -p -t png -i <strip.png>
//
//  Estrutura de arquivos:
//    assets/sprites/guy/idle/guy_idle.png          (1 frame,  64×96)
//    assets/sprites/guy/walk/guy_walk.png           (6 frames, 384×96)
//    assets/sprites/guy/punch/guy_punch.png         (4 frames, 256×96)
//    assets/sprites/guy/kick/guy_kick.png           (4 frames, 256×96)
//    assets/sprites/guy/damage/guy_damage.png       (3 frames, 192×96)
//    assets/sprites/guy/grab/guy_grab.png           (3 frames, 192×96)
//    assets/sprites/guy/jump/guy_jump.png           (4 frames, 256×96)
//    assets/sprites/guy/jump_kick/guy_jump_kick.png (4 frames, 256×96)
// ============================================================

// --- Paleta (compartilhada por todas animações) ---
extern char guy_palette,          guy_palette_end;

// --- Tiles de cada animação (definidos no data.asm) ---
extern char guy_idle_tiles,       guy_idle_tiles_end;
extern char guy_walk_tiles,       guy_walk_tiles_end;
extern char guy_punch_tiles,      guy_punch_tiles_end;
extern char guy_kick_tiles,       guy_kick_tiles_end;
extern char guy_damage_tiles,     guy_damage_tiles_end;
extern char guy_grab_tiles,       guy_grab_tiles_end;
extern char guy_jump_tiles,       guy_jump_tiles_end;
extern char guy_jump_kick_tiles,  guy_jump_kick_tiles_end;

// --- Array de frames (29 sub-estados) ---
extern SpriteFrame gGuyFrames[GUY_TOTAL_FRAMES];

void player_init(Character *c);
void player_update(Character *c, u16 pad);

#endif // PLAYER_H
