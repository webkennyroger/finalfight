; ============================================================
;  data.asm — Assets do Final Fight SNES
;
;  Sprites do Guy: frames normalizados 64×96 px cada.
;  Padronizados via tools/restructure_sprites.py para
;  empilhamento vertical (frames empilhados).
;
;  Convertidos com:
;    gfx2snes -s 16 -o 0 -u 16 -p -t png -i <arquivo.png>
;
;  Produz: <arquivo>.pic  (tiles 4bpp SNES, 128px wide)
;          <arquivo>.pal  (paleta 16 cores, 32 bytes)
;
;  Com -s 16, entrada de 64×N produz saída de 128px de largura,
;  rearranjando blocos 16×16. Cada frame de 64×96 ocupa
;  3 linhas × 8 blocos = 24 blocos = 96 tiles = 3072 bytes.
; ============================================================

.include "hdr.asm"

; ============================================================
;  BANCO 1: Guy idle + walk
; ============================================================
.BANK 1 SLOT 0
.ORG 0
.SECTION "GUY_IDLE_WALK" SUPERFREE

; Paleta compartilhada por TODAS as animações do Guy
guy_palette:
    .incbin "assets/build/sprites/guy/idle/guy_idle.pal"
guy_palette_end:

; Idle: 1 frame  (64×96 → 3072 bytes)
guy_idle_tiles:
    .incbin "assets/build/sprites/guy/idle/guy_idle.pic"
guy_idle_tiles_end:

; Walk: 8 frames (64×768 → 24576 bytes)
guy_walk_tiles:
    .incbin "assets/build/sprites/guy/walk/guy_walk.pic"
guy_walk_tiles_end:

.ENDS

; ============================================================
;  BANCO 2: Punch + Kick
; ============================================================
.BANK 2 SLOT 0
.ORG 0
.SECTION "GUY_ATTACK" SUPERFREE

; Punch: 4 frames (64×384 → 12288 bytes)
guy_punch_tiles:
    .incbin "assets/build/sprites/guy/punch/guy_punch.pic"
guy_punch_tiles_end:

; Kick: 4 frames (64×384 → 12288 bytes)
guy_kick_tiles:
    .incbin "assets/build/sprites/guy/kick/guy_kick.pic"
guy_kick_tiles_end:

.ENDS

; ============================================================
;  BANCO 3: Damage + Grab
; ============================================================
.BANK 3 SLOT 0
.ORG 0
.SECTION "GUY_DAMAGE_GRAB" SUPERFREE

; Damage/Hurt: 3 frames (64×288 → 9216 bytes)
guy_damage_tiles:
    .incbin "assets/build/sprites/guy/damage/guy_damage.pic"
guy_damage_tiles_end:

; Grab: 3 frames (64×288 → 9216 bytes)
guy_grab_tiles:
    .incbin "assets/build/sprites/guy/grab/guy_grab.pic"
guy_grab_tiles_end:

.ENDS

; ============================================================
;  BANCO 4: Jump + Jump Kick
; ============================================================
.BANK 4 SLOT 0
.ORG 0
.SECTION "GUY_JUMP" SUPERFREE

; Jump: 4 frames (64×384 → 12288 bytes)
guy_jump_tiles:
    .incbin "assets/build/sprites/guy/jump/guy_jump.pic"
guy_jump_tiles_end:

; Jump Kick: 4 frames (64×384 → 12288 bytes)
guy_jump_kick_tiles:
    .incbin "assets/build/sprites/guy/jump_kick/guy_jump_kick.pic"
guy_jump_kick_tiles_end:

.ENDS

; ============================================================
;  BANCO 5: Backgrounds
; ============================================================
.BANK 5 SLOT 0
.ORG 0
.SECTION "BGDATA" SUPERFREE

BG1_tiles:
    .incbin "assets/backgrounds/stage1/stage1_bg1.pic"
BG1_tiles_end:
BG1_pal:
    .incbin "assets/backgrounds/stage1/stage1_bg1.pal"
BG1_pal_end:
BG1_map:
    .incbin "assets/backgrounds/stage1/stage1_bg1.map"
BG1_map_end:

BG2_tiles:
    .incbin "assets/backgrounds/stage1/stage1_bg2.pic"
BG2_tiles_end:
BG2_pal:
    .incbin "assets/backgrounds/stage1/stage1_bg2.pal"
BG2_pal_end:
BG2_map:
    .incbin "assets/backgrounds/stage1/stage1_bg2.map"
BG2_map_end:

BG3_tiles:
    .incbin "assets/backgrounds/stage1/stage1_bg3.pic"
BG3_tiles_end:
BG3_pal:
    .incbin "assets/backgrounds/stage1/stage1_bg3.pal"
BG3_pal_end:
BG3_map:
    .incbin "assets/backgrounds/stage1/stage1_bg3.map"
BG3_map_end:

.ENDS
