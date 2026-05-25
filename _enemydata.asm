.include "hdr.asm"

; -------------------------------------------------------
;  Hunter tiles & palette (single frame for now)
; -------------------------------------------------------
.section ".rodata_hunter" superfree

hunter_idle_tiles:
    .incbin "assets/sprites/hunter/hunter.pic" READ 3072
hunter_idle_tiles_end:

hunter_palette:
    .incbin "assets/sprites/hunter/hunter.pal"
hunter_palette_end:

.ends

; -------------------------------------------------------
;  May tiles & palette (single frame for now)
; -------------------------------------------------------
.section ".rodata_may" superfree

may_idle_tiles:
    .incbin "assets/sprites/may/may.pic" READ 3072
may_idle_tiles_end:

may_palette:
    .incbin "assets/sprites/may/may.pal"
may_palette_end:

.ends

; -------------------------------------------------------
;  Andore tiles & palette (single frame for now)
; -------------------------------------------------------
.section ".rodata_andore" superfree

andore_idle_tiles:
    .incbin "assets/sprites/andore/andore.pic" READ 3072
andore_idle_tiles_end:

andore_palette:
    .incbin "assets/sprites/andore/andore.pal"
andore_palette_end:

.ends
