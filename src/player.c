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
    //  WALK (8 frames, ciclo)
    // ----------------------------------------------------------
    set_frame(GUY_WALK_1, (u8*)(&guy_walk_tiles) + 0 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_2, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_2, (u8*)(&guy_walk_tiles) + 1 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_3, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_3, (u8*)(&guy_walk_tiles) + 2 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_4, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_4, (u8*)(&guy_walk_tiles) + 3 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_5, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_5, (u8*)(&guy_walk_tiles) + 4 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_6, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_6, (u8*)(&guy_walk_tiles) + 5 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_7, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_7, (u8*)(&guy_walk_tiles) + 6 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_8, 5, 0,0,0,0, false);
    set_frame(GUY_WALK_8, (u8*)(&guy_walk_tiles) + 7 * GUY_TILE_SIZE,  &guy_palette, STATE_WALK, STATE_WALK, GUY_WALK_1, 5, 0,0,0,0, false);

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
    //  JUMP KICK (5 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_JUMP_KICK_1, (u8*)(&guy_jump_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_2, 4, 0,0,0,0,    false);
    set_frame(GUY_JUMP_KICK_2, (u8*)(&guy_jump_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_3, 6, 30,63,10,58, true);
    set_frame(GUY_JUMP_KICK_3, (u8*)(&guy_jump_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_JUMP_KICK, GUY_JUMP_KICK_4, 4, 0,0,0,0,    false);
    set_frame(GUY_JUMP_KICK_4, (u8*)(&guy_jump_kick_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_JUMP_KICK, STATE_IDLE, GUY_IDLE_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  DASH (6 frames, reusa walk tiles)
    // ----------------------------------------------------------
    set_frame(GUY_DASH_1, (u8*)(&guy_walk_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_2, 4, 0,0,0,0, false);
    set_frame(GUY_DASH_2, (u8*)(&guy_walk_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_3, 4, 0,0,0,0, false);
    set_frame(GUY_DASH_3, (u8*)(&guy_walk_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_4, 4, 0,0,0,0, false);
    set_frame(GUY_DASH_4, (u8*)(&guy_walk_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_5, 4, 0,0,0,0, false);
    set_frame(GUY_DASH_5, (u8*)(&guy_walk_tiles) + 4 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_6, 4, 0,0,0,0, false);
    set_frame(GUY_DASH_6, (u8*)(&guy_walk_tiles) + 5 * GUY_TILE_SIZE, &guy_palette, STATE_DASH, STATE_DASH, GUY_DASH_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  SENPUUKYAKU (8 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_SPECIAL1_1, (u8*)(&guy_senpuukyaku_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_2, 4, 0,0,0,0,    false);
    set_frame(GUY_SPECIAL1_2, (u8*)(&guy_senpuukyaku_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_3, 4, 35,63,5,58, true);
    set_frame(GUY_SPECIAL1_3, (u8*)(&guy_senpuukyaku_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_4, 4, 0,0,0,0,    false);
    set_frame(GUY_SPECIAL1_4, (u8*)(&guy_senpuukyaku_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_5, 4, 35,63,5,58, true);
    set_frame(GUY_SPECIAL1_5, (u8*)(&guy_senpuukyaku_tiles) + 4 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_6, 4, 0,0,0,0,    false);
    set_frame(GUY_SPECIAL1_6, (u8*)(&guy_senpuukyaku_tiles) + 5 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_7, 4, 35,63,5,58, true);
    set_frame(GUY_SPECIAL1_7, (u8*)(&guy_senpuukyaku_tiles) + 6 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_SPECIAL1, GUY_SPECIAL1_8, 4, 0,0,0,0,    false);
    set_frame(GUY_SPECIAL1_8, (u8*)(&guy_senpuukyaku_tiles) + 7 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL1, STATE_IDLE, GUY_IDLE_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  GORAISENPUJIN (3 frames, reusa punch tiles, spawns projectile)
    // ----------------------------------------------------------
    set_frame(GUY_SPECIAL2_1, (u8*)(&guy_punch_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL2, STATE_SPECIAL2, GUY_SPECIAL2_2, 5, 0,0,0,0,    false);
    set_frame(GUY_SPECIAL2_2, (u8*)(&guy_punch_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL2, STATE_SPECIAL2, GUY_SPECIAL2_3, 8, 40,63,8,56, false);
    set_frame(GUY_SPECIAL2_3, (u8*)(&guy_punch_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SPECIAL2, STATE_IDLE,     GUY_IDLE_1,     5, 0,0,0,0,    false);

    // ----------------------------------------------------------
    //  DASH KICK / RUN ATTACK (8 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_DASH_KICK_1, (u8*)(&guy_dash_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_2, 3, 0,0,0,0,    false);
    set_frame(GUY_DASH_KICK_2, (u8*)(&guy_dash_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_3, 3, 0,0,0,0,    false);
    set_frame(GUY_DASH_KICK_3, (u8*)(&guy_dash_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_4, 5, 30,63,8,56, true);
    set_frame(GUY_DASH_KICK_4, (u8*)(&guy_dash_kick_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_5, 5, 30,63,8,56, true);
    set_frame(GUY_DASH_KICK_5, (u8*)(&guy_dash_kick_tiles) + 4 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_6, 3, 0,0,0,0,    false);
    set_frame(GUY_DASH_KICK_6, (u8*)(&guy_dash_kick_tiles) + 5 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_7, 3, 0,0,0,0,    false);
    set_frame(GUY_DASH_KICK_7, (u8*)(&guy_dash_kick_tiles) + 6 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_DASH_ATTACK, GUY_DASH_KICK_8, 3, 0,0,0,0,    false);
    set_frame(GUY_DASH_KICK_8, (u8*)(&guy_dash_kick_tiles) + 7 * GUY_TILE_SIZE, &guy_palette, STATE_DASH_ATTACK, STATE_IDLE, GUY_IDLE_1, 3, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  LOW KICK (3 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_LOW_KICK_1, (u8*)(&guy_low_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_LOW_KICK, STATE_LOW_KICK, GUY_LOW_KICK_2, 4, 0,0,0,0,   false);
    set_frame(GUY_LOW_KICK_2, (u8*)(&guy_low_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_LOW_KICK, STATE_LOW_KICK, GUY_LOW_KICK_3, 6, 20,50,40,63, true);
    set_frame(GUY_LOW_KICK_3, (u8*)(&guy_low_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_LOW_KICK, STATE_IDLE, GUY_IDLE_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  ELBOW DROP (4 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_ELBOW_DROP_1, (u8*)(&guy_elbow_drop_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_ELBOW_DROP, STATE_ELBOW_DROP, GUY_ELBOW_DROP_2, 4, 0,0,0,0,    false);
    set_frame(GUY_ELBOW_DROP_2, (u8*)(&guy_elbow_drop_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_ELBOW_DROP, STATE_ELBOW_DROP, GUY_ELBOW_DROP_3, 4, 0,0,0,0,    false);
    set_frame(GUY_ELBOW_DROP_3, (u8*)(&guy_elbow_drop_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_ELBOW_DROP, STATE_ELBOW_DROP, GUY_ELBOW_DROP_4, 6, 20,50,30,60, true);
    set_frame(GUY_ELBOW_DROP_4, (u8*)(&guy_elbow_drop_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_ELBOW_DROP, STATE_IDLE, GUY_IDLE_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  SUPER MOVE (8 frames DEDICADOS)
    // ----------------------------------------------------------
    set_frame(GUY_SUPER_1, (u8*)(&guy_super_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_2, 5, 0,0,0,0,    false);
    set_frame(GUY_SUPER_2, (u8*)(&guy_super_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_3, 4, 30,63,8,56, true);
    set_frame(GUY_SUPER_3, (u8*)(&guy_super_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_4, 4, 30,63,8,56, true);
    set_frame(GUY_SUPER_4, (u8*)(&guy_super_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_5, 4, 30,63,8,56, true);
    set_frame(GUY_SUPER_5, (u8*)(&guy_super_tiles) + 4 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_6, 4, 30,63,8,56, true);
    set_frame(GUY_SUPER_6, (u8*)(&guy_super_tiles) + 5 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_7, 4, 30,63,8,56, true);
    set_frame(GUY_SUPER_7, (u8*)(&guy_super_tiles) + 6 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPER_8, 4, 0,0,0,0,    false);
    set_frame(GUY_SUPER_8, (u8*)(&guy_super_tiles) + 7 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_IDLE, GUY_IDLE_1, 5, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  SUPER PRETO (4 frames DEDICADOS, silhouette)
    // ----------------------------------------------------------
    set_frame(GUY_SUPERPRETO_1, (u8*)(&guy_superpreto_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPERPRETO_2, 5, 0,0,0,0,    false);
    set_frame(GUY_SUPERPRETO_2, (u8*)(&guy_superpreto_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPERPRETO_3, 5, 30,63,8,56, true);
    set_frame(GUY_SUPERPRETO_3, (u8*)(&guy_superpreto_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_SUPER, GUY_SUPERPRETO_4, 5, 0,0,0,0,    false);
    set_frame(GUY_SUPERPRETO_4, (u8*)(&guy_superpreto_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_SUPER, STATE_IDLE, GUY_IDLE_1, 5, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  WALL JUMP (3 frames, reusa jump tiles)
    // ----------------------------------------------------------
    set_frame(GUY_WALL_JUMP_1, (u8*)(&guy_jump_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_WALL_JUMP, STATE_WALL_JUMP, GUY_WALL_JUMP_2, 4, 0,0,0,0, false);
    set_frame(GUY_WALL_JUMP_2, (u8*)(&guy_jump_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_WALL_JUMP, STATE_WALL_JUMP, GUY_WALL_JUMP_3, 4, 0,0,0,0, false);
    set_frame(GUY_WALL_JUMP_3, (u8*)(&guy_jump_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_WALL_JUMP, STATE_JUMP, GUY_JUMP_2, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  TURN ATTACK (3 frames, reusa punch tiles)
    // ----------------------------------------------------------
    set_frame(GUY_TURN_ATTACK_1, (u8*)(&guy_punch_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_TURN_ATTACK, STATE_TURN_ATTACK, GUY_TURN_ATTACK_2, 3, 0,0,0,0,  false);
    set_frame(GUY_TURN_ATTACK_2, (u8*)(&guy_punch_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_TURN_ATTACK, STATE_TURN_ATTACK, GUY_TURN_ATTACK_3, 5, 20,50,8,56, true);
    set_frame(GUY_TURN_ATTACK_3, (u8*)(&guy_punch_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_TURN_ATTACK, STATE_IDLE, GUY_IDLE_1, 3, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  GRAB ATTACK (8 frames, reusa grab tiles)
    // ----------------------------------------------------------
    set_frame(GUY_GRAB_ATTACK_1, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_2, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_2, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_3, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_3, (u8*)(&guy_grab_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_4, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_4, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_5, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_5, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_6, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_6, (u8*)(&guy_grab_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_7, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_7, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_GRAB_PUNCH, GUY_GRAB_ATTACK_8, 5, 20,60,0,40, true);
    set_frame(GUY_GRAB_ATTACK_8, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_GRAB_PUNCH, STATE_IDLE, GUY_IDLE_1, 5, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  MEGA CRASH (7 frames, reusa kick tiles)
    // ----------------------------------------------------------
    set_frame(GUY_MEGA_CRASH_1, (u8*)(&guy_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_2, 4, 0,0,0,0,    false);
    set_frame(GUY_MEGA_CRASH_2, (u8*)(&guy_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_3, 5, 35,63,5,58, true);
    set_frame(GUY_MEGA_CRASH_3, (u8*)(&guy_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_4, 4, 0,0,0,0,    false);
    set_frame(GUY_MEGA_CRASH_4, (u8*)(&guy_kick_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_5, 4, 35,63,5,58, true);
    set_frame(GUY_MEGA_CRASH_5, (u8*)(&guy_kick_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_6, 4, 0,0,0,0,    false);
    set_frame(GUY_MEGA_CRASH_6, (u8*)(&guy_kick_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_MEGA_CRASH, GUY_MEGA_CRASH_7, 4, 35,63,5,58, true);
    set_frame(GUY_MEGA_CRASH_7, (u8*)(&guy_kick_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_MEGA_CRASH, STATE_IDLE, GUY_IDLE_1, 4, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  IZUNA OTOSHI (6 frames, reusa jump tiles)
    // ----------------------------------------------------------
    set_frame(GUY_IZUNA_OTOSHI_1, (u8*)(&guy_jump_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IZUNA_OTOSHI, GUY_IZUNA_OTOSHI_2, 5, 0,0,0,0, false);
    set_frame(GUY_IZUNA_OTOSHI_2, (u8*)(&guy_jump_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IZUNA_OTOSHI, GUY_IZUNA_OTOSHI_3, 5, 0,0,0,0, false);
    set_frame(GUY_IZUNA_OTOSHI_3, (u8*)(&guy_jump_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IZUNA_OTOSHI, GUY_IZUNA_OTOSHI_4, 8, 20,60,0,50, true);
    set_frame(GUY_IZUNA_OTOSHI_4, (u8*)(&guy_jump_tiles) + 3 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IZUNA_OTOSHI, GUY_IZUNA_OTOSHI_5, 5, 0,0,0,0, false);
    set_frame(GUY_IZUNA_OTOSHI_5, (u8*)(&guy_jump_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IZUNA_OTOSHI, GUY_IZUNA_OTOSHI_6, 5, 0,0,0,0, false);
    set_frame(GUY_IZUNA_OTOSHI_6, (u8*)(&guy_jump_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_IZUNA_OTOSHI, STATE_IDLE, GUY_IDLE_1, 5, 0,0,0,0, false);

    // ----------------------------------------------------------
    //  GRAPPLE FINISH / SUPLEX (8 frames, reusa grab tiles)
    // ----------------------------------------------------------
    set_frame(GUY_GRAPPLE_FINISH_1, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_2, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_2, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_3, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_3, (u8*)(&guy_grab_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_4, 5, 20,60,0,40, true);
    set_frame(GUY_GRAPPLE_FINISH_4, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_5, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_5, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_6, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_6, (u8*)(&guy_grab_tiles) + 2 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_7, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_7, (u8*)(&guy_grab_tiles) + 0 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_SHOULDER_THROW, GUY_GRAPPLE_FINISH_8, 5, 0,0,0,0, false);
    set_frame(GUY_GRAPPLE_FINISH_8, (u8*)(&guy_grab_tiles) + 1 * GUY_TILE_SIZE, &guy_palette, STATE_SHOULDER_THROW, STATE_IDLE, GUY_IDLE_1, 5, 0,0,0,0, false);

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

// ----------------------------------------------------------------
// Double-tap thresholds
#define DASH_TAP_FRAMES 12

void player_update(Character *c, u16 pad) {
    static u8  sDashDir = 0;    // 1=left, 2=right (current dash dir)
    static u8  sDirTap  = 0;    // 1=left, 2=right (last tap)
    static u8  sTapTimer = 0;
    static u16 sOldPad  = 0;

    if (!c->alive) return;

    // Block re-input during attack animations
    if (c->state == STATE_PUNCH || c->state == STATE_KICK ||
        c->state == STATE_GRAB  || c->state == STATE_JUMP_KICK ||
        c->state == STATE_SPECIAL1 || c->state == STATE_SPECIAL2)
        return;

    // Hurt / dead handled by char_update_hurt
    if (c->state == STATE_HURT || c->state == STATE_DEAD) {
        char_update_hurt(c);
        return;
    }

    // ── Jump state: can jump-kick ──
    if (c->state == STATE_JUMP) {
        if (pad & KEY_B)
            char_set_state(c, STATE_JUMP_KICK, GUY_JUMP_KICK_1);
        if (c->y >= c->groundY && c->velY >= 0)
            char_land(c);
        return;
    }
    if (c->state == STATE_JUMP_KICK) {
        if (c->y >= c->groundY && c->velY >= 0)
            char_land(c);
        return;
    }

    // ── Dash state: maintain speed, cancel into attacks ──
    if (c->state == STATE_DASH) {
        if (pad & KEY_A) { char_set_state(c, STATE_PUNCH, GUY_PUNCH_1); return; }
        if (pad & KEY_B) { char_set_state(c, STATE_KICK, GUY_KICK_1); return; }
        if (pad & KEY_Y && c->y >= c->groundY) {
            char_set_velocity(c, c->velX, JUMP_VEL);
            char_set_state(c, STATE_JUMP, GUY_JUMP_1);
            return;
        }
        // Exit dash when direction key is released
        bool release = (sDashDir == 1 && !(pad & KEY_LEFT)) ||
                       (sDashDir == 2 && !(pad & KEY_RIGHT));
        if (release) {
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
            char_set_velocity(c, 0, 0);
            sDashDir = 0;
        }
        return;
    }

    // ── Special moves ──────────────────────────────────────
    if ((pad & KEY_DOWN) && (pad & KEY_A)) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_SPECIAL2, GUY_SPECIAL2_1);
        return;
    }
    if ((pad & KEY_DOWN) && (pad & KEY_B)) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_SPECIAL1, GUY_SPECIAL1_1);
        return;
    }

    // ── Direction input: double-tap detection + walk ──────
    u16 newKeys = pad & ~sOldPad;
    sOldPad = pad;

    s8 vx = 0, vy = 0;

    if (newKeys & KEY_LEFT) {
        if (sDirTap == 1 && sTapTimer < DASH_TAP_FRAMES && c->state != STATE_DASH) {
            sDashDir = 1;
            char_set_state(c, STATE_DASH, GUY_DASH_1);
            char_set_velocity(c, -DASH_SPEED_X, 0);
            sTapTimer = 0;
            return;
        }
        sDirTap = 1;
        sTapTimer = 0;
        vx = -PLAYER_SPEED_X;
    } else if (newKeys & KEY_RIGHT) {
        if (sDirTap == 2 && sTapTimer < DASH_TAP_FRAMES && c->state != STATE_DASH) {
            sDashDir = 2;
            char_set_state(c, STATE_DASH, GUY_DASH_1);
            char_set_velocity(c, DASH_SPEED_X, 0);
            sTapTimer = 0;
            return;
        }
        sDirTap = 2;
        sTapTimer = 0;
        vx = +PLAYER_SPEED_X;
    } else {
        if (sTapTimer < 255) sTapTimer++;
        if (pad & KEY_LEFT)  vx = -PLAYER_SPEED_X;
        if (pad & KEY_RIGHT) vx = +PLAYER_SPEED_X;
    }

    if (pad & KEY_UP)    vy = -PLAYER_SPEED_Y;
    else if (pad & KEY_DOWN)  vy = +PLAYER_SPEED_Y;

    char_set_velocity(c, vx, vy);

    if (vx != 0 || vy != 0) {
        if (c->state != STATE_WALK)
            char_set_state(c, STATE_WALK, GUY_WALK_1);
    } else {
        if (c->state == STATE_WALK)
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
    }

    // ── Grounded actions ───────────────────────────────────
    if (pad & KEY_A) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_PUNCH, GUY_PUNCH_1);
        return;
    }
    if (pad & KEY_B) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_KICK, GUY_KICK_1);
        return;
    }
    if (pad & KEY_Y && c->y >= c->groundY) {
        char_set_velocity(c, c->velX, JUMP_VEL);
        char_set_state(c, STATE_JUMP, GUY_JUMP_1);
        return;
    }
}
