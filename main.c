#include <snes.h>
#include "src/types.h"
#include "src/character.h"
#include "src/player.h"
#include "src/enemy.h"
#include "src/collision.h"
#include "src/hud.h"
#include "src/vblank.h"

// scanPads is declared in <snes/pad.h> but that conflicts with <snes/input.h>
// (both define KEY_* enums). We forward-declare it here instead.
void scanPads(void);

// ============================================================
//  Assets (data.asm)
// ============================================================
extern char BG1_tiles, BG1_tiles_end;
extern char BG1_pal,   BG1_pal_end;
extern char BG1_map,   BG1_map_end;

extern char BG2_tiles, BG2_tiles_end;
extern char BG2_pal,   BG2_pal_end;
extern char BG2_map,   BG2_map_end;

extern char HUD_tiles, HUD_tiles_end;
extern char HUD_pal,   HUD_pal_end;
extern char HUD_map,   HUD_map_end;

extern char guy_idle_tiles,  guy_idle_tiles_end;
extern char guy_palette,     guy_palette_end;

// ============================================================
//  Scroll (structs do exemplo sor2_snes)
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
//  VBlank
// ============================================================
void myconsoleVblank(void) {
    // 1) Read input
    scanPads();

    // 2) OAM
    dmaCopyOAram((unsigned char*)&oamMemory, 0, 0x220);

    // 3) Scroll BG1
    if (bgInfo.refreshBG1) {
        dmaCopyVram(bgInfo.bg1.gfxoffset, bgInfo.bg1.adrgfxvram, bgInfo.bg1.size);
        bgInfo.refreshBG1 = 0;
    }

    // 4) Scroll BG2
    if (bgInfo.refreshBG2) {
        dmaCopyVram(bgInfo.bg2.gfxoffset, bgInfo.bg2.adrgfxvram, bgInfo.bg2.size);
        bgInfo.refreshBG2 = 0;
    }

    // 5) Flush queued sprite tile DMAs
    vblank_flush_sprite_queue();

    // 6) HUD (full buffer to BG3 map at 0x1000)
    hud_draw();

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

static void handleScroll(BGScroll *s, int playerX, s8 playerVelX) {
    if (playerVelX <= 0) return;
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
//  MAIN
// ============================================================
int main(void) {
    int i;

    // --- Tilemaps ---
    bgSetMapPtr(0, 0x0000,        SC_64x32);
    bgSetMapPtr(1, 0x0000 + 2048, SC_64x32);
    bgSetMapPtr(2, 0x1000,        SC_32x32);

    // --- Tilesets ---
    bgInitTileSet(0, &BG1_tiles, &BG1_pal, 2,
                  (&BG1_tiles_end - &BG1_tiles), 16*4, BG_16COLORS, 0x2000);
    bgInitTileSet(1, &BG2_tiles, &BG2_pal, 4,
                  (&BG2_tiles_end - &BG2_tiles), 16*4, BG_16COLORS, 0x3000);
    bgInitTileSet(2, &HUD_tiles, &HUD_pal, 0,
                  (&HUD_tiles_end - &HUD_tiles), 16*4*8, BG_16COLORS, 0x4000);

    // --- Primeiros mapas de scroll ---
    updateBG1(&BG1_map, 0x0000,        2048);
    updateBG2(&BG2_map, 0x0000 + 2048, 2048);

    // --- BG3 HUD ---
    setMode(BG_MODE1, BG3_MODE1_PRIORITY_HIGH);

    // --- Sprites: OBJ (small=16x16) ---
    oamInitGfxSet(
        &guy_idle_tiles,
        (&guy_idle_tiles_end - &guy_idle_tiles),
        &guy_palette,
        (&guy_palette_end  - &guy_palette),
        0, 0x6000, OBJ_SIZE16_L32
    );

    // --- Inicializa sistema de personagens ---
    for (i = 0; i < MAX_CHARS; i++) gYOrder[i] = NULL;

    gPlayer = &gCharacters[0];
    char_init(gPlayer, GUY_TILE_SIZE, 32);
    player_init(gPlayer);
    gPlayer->x = 48;
    gPlayer->y = 160;
    gPlayer->groundY = GROUND_Y;
    char_load_gfx(gPlayer, 0, 0x6000, 0, 0);

    // --- Inimigos: marcar como mortos ---
    for (i = 1; i < MAX_CHARS; i++)
        gCharacters[i].alive = false;

    // --- Projéteis ---
    collision_init_projectiles();

    // --- HUD ---
    hud_init();
    hud_draw_hp(gPlayer->hp, gPlayer->maxHp);
    hud_draw_timer((u8)gTimer);
    WaitForVBlank();
    hud_draw();

    // --- Ondas de inimigos ---
    enemy_init_waves();

    // --- VBlank ---
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
        u16 pad = padsCurrent(0);

        // 1) Player logic
        player_update(gPlayer, pad);

        // 2) Update enemies
        for (i = 1; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                enemy_update(&gCharacters[i], gPlayer);
        }

        // 3) Spawn waves based on player position
        enemy_update_waves(gPlayer);

        // 3b) Timer countdown
        if (gTimer > 0) {
            gTimerTicks++;
            if (gTimerTicks >= 60) {
                gTimerTicks = 0;
                gTimer--;
                hud_draw_timer((u8)gTimer);
            }
        }

        // 4) Physics: apply velocity + gravity to all
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_update_pos(&gCharacters[i]);
        }

        // 5) Animation: tick frames
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_update_gfx(&gCharacters[i]);
        }

        // 6) Check if SPECIAL2 should spawn projectile
        {
            static u8 sLastSub = 255;
            u8 curSub = gPlayer->subState;
            if (gPlayer->state == STATE_SPECIAL2 && curSub == GUY_SPECIAL2_2 && sLastSub != curSub) {
                int dir = gPlayer->hflip ? -1 : 1;
                collision_spawn_projectile(
                    gPlayer->x + (dir > 0 ? 50 : -16),
                    gPlayer->y - 30,
                    dir * 4, 2, true
                );
            }
            sLastSub = curSub;
            if (gPlayer->state != STATE_SPECIAL2) sLastSub = 255;
        }

        // 7) Collision: hitboxes + projectiles
        collision_check_hitboxes();

        // 8) Projectile movement
        collision_update_projectiles();

        // 9) Physical overlap resolution
        collision_resolve_overlap();

        // 10) Z-depth sort (Y axis)
        char_sort_y_order();

        // 11) Draw all characters
        for (i = 0; i < MAX_CHARS; i++) {
            if (gCharacters[i].alive)
                char_draw(&gCharacters[i]);
        }

        // 12) HUD
        hud_draw_hp(gPlayer->hp, gPlayer->maxHp);

        // 13) Scroll
        handleScroll(&bgMain, gPlayer->x, gPlayer->velX);
        handleScroll(&bgSub,  gPlayer->x, gPlayer->velX);

        // 14) VBlank (DMA OAM + tiles + BG + HUD)
        WaitForVBlank();
    }

    return 0;
}