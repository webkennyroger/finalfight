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

; Jump: 4 frames (64x384 → 12288 bytes)
guy_jump_tiles:
    .incbin "assets/build/sprites/guy/jump/guy_jump.pic"
guy_jump_tiles_end:

; Jump Kick: 4 frames (64x384 → 12288 bytes)
guy_jump_kick_tiles:
    .incbin "assets/build/sprites/guy/jump_kick/guy_jump_kick.pic"
guy_jump_kick_tiles_end:

.ENDS

; ============================================================
;  BANCO 9: Low Kick + Super Preto (silhouette)
; ============================================================
.BANK 9 SLOT 0
.ORG 0
.SECTION "GUY_SPECIALS_A" SUPERFREE

; Low Kick: 3 frames (64x288 → 9216 bytes)
guy_low_kick_tiles:
    .incbin "assets/build/sprites/guy/low_kick/guy_low_kick.pic"
guy_low_kick_tiles_end:

; Super preto (silhouette): 4 frames (64x384 → 12288 bytes)
guy_superpreto_tiles:
    .incbin "assets/build/sprites/guy/super/guy_superpreto.pic"
guy_superpreto_tiles_end:

.ENDS

; ============================================================
;  BANCO 10: Super
; ============================================================
.BANK 10 SLOT 0
.ORG 0
.SECTION "GUY_SUPER" SUPERFREE

; Super: 8 frames (64x768 → 24576 bytes)
guy_super_tiles:
    .incbin "assets/build/sprites/guy/super/guy_super.pic"
guy_super_tiles_end:

.ENDS

; ============================================================
;  BANCO 11: Senpuukyaku
; ============================================================
.BANK 11 SLOT 0
.ORG 0
.SECTION "GUY_SENPUUKYAKU" SUPERFREE

; Senpuukyaku: 8 frames (64x768 → 24576 bytes)
guy_senpuukyaku_tiles:
    .incbin "assets/build/sprites/guy/senpuukyaku/guy_senpuukyaku.pic"
guy_senpuukyaku_tiles_end:

.ENDS

; ============================================================
;  BANCO 12: Dash Kick
; ============================================================
.BANK 12 SLOT 0
.ORG 0
.SECTION "GUY_DASH_KICK" SUPERFREE

; Dash Kick / Run Attack: 8 frames (64x768 → 24576 bytes)
guy_dash_kick_tiles:
    .incbin "assets/build/sprites/guy/dash/guy_dash_kick.pic"
guy_dash_kick_tiles_end:

.ENDS

; ============================================================
;  BANCO 13: Elbow Drop + Izuna Otoshi
; ============================================================
.BANK 13 SLOT 0
.ORG 0
.SECTION "GUY_ELBOW_DROP" SUPERFREE

; Elbow Drop: 4 frames (64x384 → 12288 bytes)
guy_elbow_drop_tiles:
    .incbin "assets/build/sprites/guy/elbow_drop/guy_elbow_drop.pic"
guy_elbow_drop_tiles_end:

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

; ============================================================
;  HUD
; ============================================================
HUD_tiles:
    .incbin "assets/hud/hud.pic"
HUD_tiles_end:
HUD_pal:
    .incbin "assets/hud/hud.pal"
HUD_pal_end:
HUD_map:
    .incbin "assets/hud/hud.map"
HUD_map_end:

.ENDS

; ============================================================
;  Inimigos (Hunter, May, Andore)
;
;  .pic files are full spritesheets (multiple 64x96 frames).
;  Only the first frame (96 tiles / 3072 bytes) is loaded per
;  enemy at VRAM, so all sub-states share the same tile data.
; ============================================================

; ------------------------------------------------------------
;  HUNTER (64x896, 896 tiles = 28672 bytes)
; ------------------------------------------------------------
.BANK 6 SLOT 0
.ORG 0
.SECTION "HUNTER" SUPERFREE

hunter_palette:
    .incbin "assets/sprites/enemies/hunter/hunter.pal"
hunter_palette_end:

hunter_idle_tiles:
    .incbin "assets/sprites/enemies/hunter/hunter.pic"
hunter_idle_tiles_end:

.ENDS

; ------------------------------------------------------------
;  MAY (64x704, 704 tiles = 22528 bytes)
; ------------------------------------------------------------
.BANK 7 SLOT 0
.ORG 0
.SECTION "MAY" SUPERFREE

may_palette:
    .incbin "assets/sprites/enemies/may/may.pal"
may_palette_end:

may_idle_tiles:
    .incbin "assets/sprites/enemies/may/may.pic"
may_idle_tiles_end:

.ENDS

; ------------------------------------------------------------
;  ANDORE (64x896, 896 tiles = 28672 bytes)
; ------------------------------------------------------------
.BANK 8 SLOT 0
.ORG 0
.SECTION "ANDORE" SUPERFREE

andore_palette:
    .incbin "assets/sprites/enemies/andore/andore.pal"
andore_palette_end:

andore_idle_tiles:
    .incbin "assets/sprites/enemies/andore/andore.pic"
andore_idle_tiles_end:

.ENDS
