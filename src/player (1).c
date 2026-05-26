#include "player.h"
#include "character.h"
#include "physics.h"
#include "projectile.h"

// ============================================================
//  player.c — Todas as mecânicas do Guy
//
//  Botões:
//    D-Pad       Mover / Esquivar no ar
//    A           Soco  (ou Grab se perto de inimigo)
//    B           Chute (ou Jump Kick se no ar)
//    Y           Pulo
//    X           Bushin Goraisenpujin (↓↓ + X)
//    Duplo ←/→   Dash → Corrida → Senpukyaku (dash + B)
// ============================================================

#define F(base, n) ((u8*)(&base) + (u32)(n) * GUY_TILE_SIZE)

SpriteFrame gGuyFrames[GUY_TOTAL_FRAMES] = {
    [GUY_IDLE_1]={F(guy_idle_tiles,0),&guy_palette,STATE_IDLE,STATE_IDLE,GUY_IDLE_1,60,0,0,0,0,false},

    [GUY_WALK_1]={F(guy_walk_tiles,0),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_2,5,0,0,0,0,false},
    [GUY_WALK_2]={F(guy_walk_tiles,1),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_3,5,0,0,0,0,false},
    [GUY_WALK_3]={F(guy_walk_tiles,2),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_4,5,0,0,0,0,false},
    [GUY_WALK_4]={F(guy_walk_tiles,3),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_5,5,0,0,0,0,false},
    [GUY_WALK_5]={F(guy_walk_tiles,4),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_6,5,0,0,0,0,false},
    [GUY_WALK_6]={F(guy_walk_tiles,5),&guy_palette,STATE_WALK,STATE_WALK,GUY_WALK_1,5,0,0,0,0,false},

    [GUY_PUNCH_1]={F(guy_punch_tiles,0),&guy_palette,STATE_PUNCH,STATE_PUNCH,GUY_PUNCH_2,4,0,0,0,0,false},
    [GUY_PUNCH_2]={F(guy_punch_tiles,1),&guy_palette,STATE_PUNCH,STATE_PUNCH,GUY_PUNCH_3,6,40,63,8,56,true},
    [GUY_PUNCH_3]={F(guy_punch_tiles,2),&guy_palette,STATE_PUNCH,STATE_PUNCH,GUY_PUNCH_4,4,0,0,0,0,false},
    [GUY_PUNCH_4]={F(guy_punch_tiles,3),&guy_palette,STATE_PUNCH,STATE_IDLE,GUY_IDLE_1,4,0,0,0,0,false},

    [GUY_KICK_1]={F(guy_kick_tiles,0),&guy_palette,STATE_KICK,STATE_KICK,GUY_KICK_2,4,0,0,0,0,false},
    [GUY_KICK_2]={F(guy_kick_tiles,1),&guy_palette,STATE_KICK,STATE_KICK,GUY_KICK_3,6,38,63,10,58,true},
    [GUY_KICK_3]={F(guy_kick_tiles,2),&guy_palette,STATE_KICK,STATE_KICK,GUY_KICK_4,4,0,0,0,0,false},
    [GUY_KICK_4]={F(guy_kick_tiles,3),&guy_palette,STATE_KICK,STATE_IDLE,GUY_IDLE_1,4,0,0,0,0,false},

    [GUY_HURT_1]={F(guy_damage_tiles,0),&guy_palette,STATE_HURT,STATE_HURT,GUY_HURT_1,999,0,0,0,0,false},
    [GUY_HURT_2]={F(guy_damage_tiles,1),&guy_palette,STATE_HURT,STATE_HURT,GUY_HURT_2,999,0,0,0,0,false},
    [GUY_DEAD_1]={F(guy_damage_tiles,2),&guy_palette,STATE_DEAD,STATE_DEAD,GUY_DEAD_1,999,0,0,0,0,false},

    [GUY_GRAB_1]={F(guy_grab_tiles,0),&guy_palette,STATE_GRAB,STATE_GRAB,GUY_GRAB_2,8,20,60,0,40,true},
    [GUY_GRAB_2]={F(guy_grab_tiles,1),&guy_palette,STATE_GRAB,STATE_GRAB,GUY_GRAB_3,8,20,60,0,40,true},
    [GUY_GRAB_3]={F(guy_grab_tiles,2),&guy_palette,STATE_GRAB,STATE_IDLE,GUY_IDLE_1,6,0,0,0,0,false},

    [GUY_JUMP_1]={F(guy_jump_tiles,0),&guy_palette,STATE_JUMP,STATE_JUMP,GUY_JUMP_2,6,0,0,0,0,false},
    [GUY_JUMP_2]={F(guy_jump_tiles,1),&guy_palette,STATE_JUMP,STATE_JUMP,GUY_JUMP_3,6,0,0,0,0,false},
    [GUY_JUMP_3]={F(guy_jump_tiles,2),&guy_palette,STATE_JUMP,STATE_JUMP,GUY_JUMP_4,6,0,0,0,0,false},
    [GUY_JUMP_4]={F(guy_jump_tiles,3),&guy_palette,STATE_JUMP,STATE_JUMP,GUY_JUMP_4,6,0,0,0,0,false},

    [GUY_JUMP_KICK_1]={F(guy_jump_kick_tiles,0),&guy_palette,STATE_JUMP_KICK,STATE_JUMP_KICK,GUY_JUMP_KICK_2,4,0,0,0,0,false},
    [GUY_JUMP_KICK_2]={F(guy_jump_kick_tiles,1),&guy_palette,STATE_JUMP_KICK,STATE_JUMP_KICK,GUY_JUMP_KICK_3,5,30,63,10,58,true},
    [GUY_JUMP_KICK_3]={F(guy_jump_kick_tiles,2),&guy_palette,STATE_JUMP_KICK,STATE_JUMP_KICK,GUY_JUMP_KICK_4,4,0,0,0,0,false},
    [GUY_JUMP_KICK_4]={F(guy_jump_kick_tiles,3),&guy_palette,STATE_JUMP_KICK,STATE_JUMP_KICK,GUY_JUMP_KICK_1,4,30,63,10,58,true},
};

static char sGuyName[] = "GUY";

void player_init(Character *c) {
    c->frames      = gGuyFrames;
    c->totalFrames = GUY_TOTAL_FRAMES;
    c->tileSize    = GUY_TILE_SIZE;
    c->hp = c->maxHp = 18;
    c->isEnemy     = false;
    c->name        = sGuyName;
    c->speed       = 0;
    c->hurtDuration= 20;
    c->grabTarget  = NULL;
    c->isGrabbed   = false;
    c->isDashing   = false;
    c->dashTimer   = 0;
    c->lastDir     = 0;
    c->specialHits = 0;
    c->goraCharge  = 0;
    c->isAirborne  = false;
    c->gravVel     = 0;
}

// ----------------------------------------------------------------
//  Verifica grab: procura inimigo no alcance de agarre
// ----------------------------------------------------------------
static Character *_find_grab_target(Character *c) {
    int i;
    for (i = 1; i < MAX_CHARS; i++) {
        Character *e = &gCharacters[i];
        if (!e->alive || e->isGrabbed || e->isEnemy == false) continue;
        int dx = c->x - e->x; if (dx < 0) dx = -dx;
        int dy = c->y - e->y; if (dy < 0) dy = -dy;
        if (dx < 36 && dy < 10) return e;
    }
    return NULL;
}

// ----------------------------------------------------------------
//  Goraisenpujin: só dispara se goraCharge >= 20
// ----------------------------------------------------------------
#define GORA_CHARGE_NEEDED  20

// ----------------------------------------------------------------
void player_update(Character *c, u16 pad) {
    if (!c->alive) return;

    // ── MORTO ─────────────────────────────────────────────────
    if (c->state == STATE_DEAD) return;

    // ── HURT ──────────────────────────────────────────────────
    if (c->state == STATE_HURT) {
        char_update_hurt(c);
        return;
    }

    // ── NO AR (pulo / jump kick) ───────────────────────────────
    if (c->isAirborne) {
        // Direcional no ar: leve controle horizontal
        if (pad & KEY_LEFT)  char_set_velocity(c, -2, c->velY);
        if (pad & KEY_RIGHT) char_set_velocity(c,  2, c->velY);
        // Jump kick
        if ((pad & KEY_B) && c->state != STATE_JUMP_KICK) {
            c->specialHits = 3;
            char_set_state(c, STATE_JUMP_KICK, GUY_JUMP_KICK_1);
        }
        // physics_apply cuida da aterrissagem (chamado no main.c)
        return;
    }

    // ── SOCO / CHUTE / GRAB em andamento ──────────────────────
    if (c->state == STATE_PUNCH ||
        c->state == STATE_KICK  ||
        c->state == STATE_SPECIAL) return;

    // ── GRAB em andamento ─────────────────────────────────────
    if (c->state == STATE_GRAB) {
        // A durante grab: punch no inimigo agarrado (1 hit)
        if ((pad & KEY_A) && c->grabTarget && c->grabTarget->alive) {
            char_apply_damage(c->grabTarget, 2);
            char_set_state(c, STATE_GRAB, GUY_GRAB_1);
        }
        // B durante grab: arremessa o inimigo
        if (pad & KEY_B) {
            if (c->grabTarget) {
                c->grabTarget->isGrabbed = false;
                // Dá velocidade de arremesso
                c->grabTarget->velX = c->hflip ? -5 : 5;
                c->grabTarget->velY = -4;
                char_apply_damage(c->grabTarget, 3);
                c->grabTarget = NULL;
            }
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
        }
        return;
    }

    // ── GORAISENPUJIN: ↓ acumula carga ────────────────────────
    if (pad & KEY_DOWN) {
        if (c->goraCharge < 60) c->goraCharge++;
    } else {
        // Se soltou baixo com carga suficiente e pressiona X → dispara
        if ((pad & KEY_X) && c->goraCharge >= GORA_CHARGE_NEEDED) {
            c->goraCharge = 0;
            projectile_fire(c);
            char_set_state(c, STATE_PROJECTIL, GUY_GORA_1);
            return;
        }
        c->goraCharge = 0;
    }

    // ── PULO ──────────────────────────────────────────────────
    if ((pad & KEY_Y) && !c->isAirborne) {
        physics_start_jump(c, JUMP_VEL);
        char_set_velocity(c, c->velX, 0);
        char_set_state(c, STATE_JUMP, GUY_JUMP_1);
        return;
    }

    // ── DETECÇÃO DE DASH (duplo-toque direcional) ─────────────
    u8 dir = 0;
    if (pad & KEY_LEFT)  dir = 1;
    if (pad & KEY_RIGHT) dir = 2;

    if (c->dashTimer > 0) c->dashTimer--;

    if (dir != 0 && dir == c->lastDir && c->dashTimer > 0 && !c->isDashing) {
        // Duplo-toque detectado → inicia dash
        c->isDashing  = true;
        c->dashFrames = 18;
        char_set_velocity(c, (dir == 2) ? DASH_SPEED : -DASH_SPEED, 0);
    } else if (dir != 0 && dir != c->lastDir) {
        c->lastDir   = dir;
        c->dashTimer = 12;   // janela de 12 frames para 2° toque
    }

    // ── DASH / CORRIDA ────────────────────────────────────────
    if (c->isDashing) {
        c->dashFrames--;
        if (c->state != STATE_RUN)
            char_set_state(c, STATE_RUN, GUY_WALK_1);

        // Bushin Senpukyaku: dash + B
        if (pad & KEY_B) {
            c->isDashing   = false;
            c->specialHits = 4;   // multi-hit
            char_set_state(c, STATE_SPECIAL, GUY_SENPU_1);
            return;
        }

        if (c->dashFrames == 0) {
            c->isDashing = false;
            char_set_velocity(c, 0, 0);
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
        }
        return;
    }

    // ── SENPUKYAKU em andamento ───────────────────────────────
    if (c->state == STATE_SPECIAL) {
        // Avança e gira — handled por char_update_gfx
        // Cada ciclo de animação decrementa specialHits
        if (c->frameCounter == 0 && c->specialHits > 0) {
            c->specialHits--;
            char_set_velocity(c, c->hflip ? -3 : 3, 0);
        }
        if (c->specialHits == 0)
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
        return;
    }

    // ── MOVIMENTO NORMAL ──────────────────────────────────────
    bool moving = false;

    if (pad & KEY_UP) {
        char_set_velocity(c, c->velX, -PLAYER_SPEED_Y); moving = true;
    } else if (!(pad & KEY_DOWN)) {
        char_set_velocity(c, c->velX, 0);
    }

    if (pad & KEY_LEFT) {
        char_set_velocity(c, -PLAYER_SPEED_X, c->velY); moving = true;
    } else if (pad & KEY_RIGHT) {
        char_set_velocity(c, +PLAYER_SPEED_X, c->velY); moving = true;
    } else {
        char_set_velocity(c, 0, c->velY);
    }

    // Transição de estado — FIX: só muda ao entrar/sair do movimento
    if (moving) {
        if (c->state != STATE_WALK)
            char_set_state(c, STATE_WALK, GUY_WALK_1);
    } else {
        char_set_velocity(c, 0, 0);
        if (c->state == STATE_WALK)
            char_set_state(c, STATE_IDLE, GUY_IDLE_1);
    }

    // ── SOCO / GRAB ──────────────────────────────────────────
    if (pad & KEY_A) {
        char_set_velocity(c, 0, 0);
        Character *target = _find_grab_target(c);
        if (target) {
            // Grab
            c->grabTarget    = target;
            target->isGrabbed= true;
            target->isInvincible = true;
            char_set_state(c,      STATE_GRAB,  GUY_GRAB_1);
            char_set_state(target, STATE_HURT,  ENEMY_HURT_1);
        } else {
            char_set_state(c, STATE_PUNCH, GUY_PUNCH_1);
        }
    }

    // ── CHUTE ────────────────────────────────────────────────
    if (pad & KEY_B) {
        char_set_velocity(c, 0, 0);
        char_set_state(c, STATE_KICK, GUY_KICK_1);
    }
}
