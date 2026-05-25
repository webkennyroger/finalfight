#include "player.h"
#include "character.h"

SpriteFrame gGuyFrames[GUY_TOTAL_FRAMES];

static char sGuyName[] = "GUY";

static void set_frame(u8 idx, u8 *tiles, u8 *palette,
                      u16 state, u16 nextState, u16 nextSubState, u16 duration,
                      u8 hx1, u8 hx2, u8 hy1, u8 hy2, bool hb) {
    gGuyFrames[idx].tiles       = tiles;
    gGuyFrames[idx].palette     = palette;
    gGuyFrames[idx].state       = state;
    gGuyFrames[idx].nextState   = nextState;
    gGuyFrames[idx].nextSubState= nextSubState;
    gGuyFrames[idx].duration    = duration;
    gGuyFrames[idx].hitboxX1   = hx1;
    gGuyFrames[idx].hitboxX2   = hx2;
    gGuyFrames[idx].hitboxY1   = hy1;
    gGuyFrames[idx].hitboxY2   = hy2;
    gGuyFrames[idx].hasHitbox  = hb;
}

void player_init(Character *c) {
    // ----------------------------------------------------------
    //  IDLE (1 frame, loop)
    // ----------------------------------------------------------
    set_frame(GUY_IDLE_1,
        (u8*)(&guy_idle_tiles) + 0 * GUY_TILE_SIZE, &guy_palette,
        STATE_IDLE, STATE_IDLE, GUY_IDLE_1, 60,
        0,0,0,0, false);

    // ----------------------------------------------------------
    //  WALK (6 frames, ciclo)
    // ----------------------------------------------------------
    set_frame(GUY_WALK_1, (u8*)(&guy_walk_tiles) + 0 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_2, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_2, (u8*)(&guy_walk_tiles) + 1 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_3, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_3, (u8*)(&guy_walk_tiles) + 2 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_4, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_4, (u8*)(&guy_walk_tiles) + 3 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_5, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_5, (u8*)(&guy_walk_tiles) + 4 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_6, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_6, (u8*)(&guy_walk_tiles) + 5 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_1, 5, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  PUNCH (4 frames, frame 2 tem hitbox)
    // ----------------------------------------------------------
    set_frame(GUY_PUNCH_1, (u8*)(&guy_punch_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_PUNCH, STATE_PUNCH, GUY_PUNCH_2, 4, 0,0,0,0,   false);
    set_frame(GUY_PUNCH_2, (u8*)(&guy_punch_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_PUNCH, STATE_PUNCH, GUY_PUNCH_3, 6, 40,63,8,56, true);
    set_frame(GUY_PUNCH_3, (u8*)(&guy_punch_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_PUNCH, STATE_PUNCH, GUY_PUNCH_4, 4, 0,0,0,0,   false);
    set_frame(GUY_PUNCH_4, (u8*)(&guy_punch_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_PUNCH, STATE_IDLE,  GUY_IDLE_1,   4, 0,0,0,0,   false);

    // ----------------------------------------------------------
    //  KICK (4 frames, frame 2 tem hitbox)
    // ----------------------------------------------------------
    set_frame(GUY_KICK_1, (u8*)(&guy_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_KICK, STATE_KICK, GUY_KICK_2, 4, 0,0,0,0,    false);
    set_frame(GUY_KICK_2, (u8*)(&guy_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_KICK, STATE_KICK, GUY_KICK_3, 6, 38,63,10,58, true);
    set_frame(GUY_KICK_3, (u8*)(&guy_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_KICK, STATE_KICK, GUY_KICK_4, 4, 0,0,0,0,    false);
    set_frame(GUY_KICK_4, (u8*)(&guy_kick_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_KICK, STATE_IDLE, GUY_IDLE_1,  4, 0,0,0,0,    false);

    // ----------------------------------------------------------
    //  HURT (3 frames de dano)
    // ----------------------------------------------------------
    set_frame(GUY_HURT_1, (u8*)(&guy_damage_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_HURT, STATE_HURT, GUY_HURT_1, 999, 0,0,0,0, false);
    set_frame(GUY_HURT_2, (u8*)(&guy_damage_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_HURT, STATE_HURT, GUY_HURT_2, 999, 0,0,0,0, false);
    set_frame(GUY_DEAD_1, (u8*)(&guy_damage_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_DEAD, STATE_DEAD, GUY_DEAD_1, 999, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  GRAB (3 frames)
    // ----------------------------------------------------------
    set_frame(GUY_GRAB_1, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB, STATE_GRAB, GUY_GRAB_2, 6, 20,60,0,40, true);
    set_frame(GUY_GRAB_2, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB, STATE_GRAB, GUY_GRAB_3, 6, 20,60,0,40, true);
    set_frame(GUY_GRAB_3, (u8*)(&guy_grab_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB, STATE_IDLE, GUY_IDLE_1, 6, 0,0,0,0,    false);

    // ----------------------------------------------------------
    //  JUMP (4 frames)
    // ----------------------------------------------------------
    set_frame(GUY_JUMP_1, (u8*)(&guy_jump_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP, STATE_JUMP, GUY_JUMP_2, 6, 0,0,0,0, false);
    set_frame(GUY_JUMP_2, (u8*)(&guy_jump_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP, STATE_JUMP, GUY_JUMP_3, 6, 0,0,0,0, false);
    set_frame(GUY_JUMP_3, (u8*)(&guy_jump_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP, STATE_JUMP, GUY_JUMP_4, 6, 0,0,0,0, false);
    set_frame(GUY_JUMP_4, (u8*)(&guy_jump_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP, STATE_IDLE, GUY_IDLE_1, 6, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  JUMP KICK (4 frames, frame 2 tem hitbox)
    // ----------------------------------------------------------
    set_frame(GUY_JUMP_KICK_1, (u8*)(&guy_jump_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_2, 4, 0,0,0,0,    false);
    set_frame(GUY_JUMP_KICK_2, (u8*)(&guy_jump_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_3, 6, 30,63,10,58, true);
    set_frame(GUY_JUMP_KICK_3, (u8*)(&guy_jump_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_4, 4, 0,0,0,0,    false);
    set_frame(GUY_JUMP_KICK_4, (u8*)(&guy_jump_kick_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_IDLE,      GUY_IDLE_1,      4, 0,0,0,0,    false);

    // Now set up the character
    c->frames       = gGuyFrames;
    c->totalFrames  = GUY_TOTAL_FRAMES;
    c->tileSize     = GUY_TILE_SIZE;
    c->hp           = 18;
    c->maxHp        = 18;
    c->isEnemy      = false;
    c->name         = sGuyName;
    c->speed        = 0;
    c->hurtDuration = 20;
}

// (player_update continues as before)
void player_update(Character *c, u16 pad) {
    if (!c->alive) return;

    if (c->state == STATE_PUNCH  ||
        c->state == STATE_KICK   ||
        c->state == STATE_GRAB   ||
        c->state == STATE_JUMP_KICK) return;

    if (c->state == STATE_HURT || c->state == STATE_DEAD) {
        char_update_hurt(c);
        return;
    }

    if (c->state == STATE_JUMP) {
        if (pad & KEY_B)
            char_set_state(c, STATE_JUMP_KICK, GUY_JUMP_KICK_1);
        return;
    }

    bool moving = false;

    if (pad & KEY_UP)    { char_set_velocity(c, c->velX, -PLAYER_SPEED_Y); moving = true; }
    else if (pad & KEY_DOWN) { char_set_velocity(c, c->velX, +PLAYER_SPEED_Y); moving = true; }
    else                   char_set_velocity(c, c->velX, 0);

    if (pad & KEY_LEFT)  { char_set_velocity(c, -PLAYER_SPEED_X, c->velY); moving = true; }
    else if (pad & KEY_RIGHT) { char_set_velocity(c, +PLAYER_SPEED_X, c->velY); moving = true; }
    else                   char_set_velocity(c, 0, c->velY);

    if (moving) {
        if (c->state != STATE_WALK)
            char_set_state(c, STATE_WALK, GUY_WALK_1);
    } else {
        char_set_velocity(c, 0, 0);
        if (c->state == STATE_WALK)
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
    }

    if (pad & KEY_A) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_PUNCH, GUY_PUNCH_1);
    }

    if (pad & KEY_B) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_KICK, GUY_KICK_1);
    }

    if (pad & KEY_Y) {
        char_set_velocity(c, c->velX, 0);
        char_set_state(c, STATE_JUMP, GUY_JUMP_1);
    }
}
