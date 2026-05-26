#include "enemy.h"
#include "character.h"

// ============================================================
//  enemy.c — IA completa de Hunter, May e Andore
//
//  Cada inimigo tem 3 fases de comportamento:
//    APPROACH  → persegue o jogador mantendo distância lateral
//    ATTACK    → ataca quando dentro do alcance
//    RECOVER   → recua após atacar
//
//  O "aggroTimer" controla agressividade: aumenta se o inimigo
//  está perto e diminui quando recua — evita rush constante.
//
//  Depth Z: todos os inimigos se movem no eixo Y para simular
//  profundidade. A ordenação Y é feita em char_sort_y_order.
// ============================================================

// Frames dos inimigos (apenas idle por enquanto — DMA troca o frame)
SpriteFrame gHunterFrames[ENEMY_TOTAL_FRAMES];
SpriteFrame gMayFrames   [ENEMY_TOTAL_FRAMES];
SpriteFrame gAndoreFrames[ENEMY_TOTAL_FRAMES];

static char sHunterName[] = "HUNTER";
static char sMayName[]    = "MAY";
static char sAndoreName[] = "ANDORE";

// IA por inimigo
typedef struct {
    u8  aggroTimer;    // agressividade atual (0-60)
    u8  attackCooldown;// frames até poder atacar novamente
    u8  phase;         // 0=approach 1=attack 2=recover
    u8  retreatTimer;  // frames de recuo
} EnemyAI;

static EnemyAI sAI[MAX_CHARS];  // índice = gCharacters slot

// ----------------------------------------------------------------
static void _init_frames(SpriteFrame *f, u8 *tiles, u8 *pal) {
    int i;
    for (i = 0; i < ENEMY_TOTAL_FRAMES; i++) {
        f[i].tiles     = tiles;
        f[i].palette   = pal;
        f[i].state     = (i == ENEMY_DEAD_1) ? STATE_DEAD : STATE_IDLE;
        f[i].nextState = f[i].state;
        f[i].nextSubState = i;
        f[i].duration  = 8;
        f[i].hitboxX1  = 8;
        f[i].hitboxX2  = 56;
        f[i].hitboxY1  = 0;
        f[i].hitboxY2  = 64;
        f[i].hasHitbox = (i == ENEMY_ATTACK_1);
    }
    // Walk cycle
    f[ENEMY_WALK_1].nextSubState = ENEMY_WALK_2;
    f[ENEMY_WALK_2].nextSubState = ENEMY_WALK_1;
    f[ENEMY_WALK_1].nextState    = STATE_WALK;
    f[ENEMY_WALK_2].nextState    = STATE_WALK;
    // Attack → volta para idle
    f[ENEMY_ATTACK_1].nextSubState = ENEMY_IDLE_1;
    f[ENEMY_ATTACK_1].nextState    = STATE_IDLE;
    f[ENEMY_ATTACK_1].duration     = 20;
    // Hurt: fica preso
    f[ENEMY_HURT_1].duration = 999;
    f[ENEMY_DEAD_1].duration = 999;
}

// ----------------------------------------------------------------
void enemy_init_hunter(Character *c) {
    _init_frames(gHunterFrames, &hunter_idle_tiles, &hunter_palette);
    c->frames      = gHunterFrames;
    c->totalFrames = ENEMY_TOTAL_FRAMES;
    c->tileSize    = 0x200;
    c->hp = c->maxHp = 6;
    c->isEnemy     = true;
    c->name        = sHunterName;
    c->speed       = 1;
    c->hurtDuration= 28;
}

void enemy_init_may(Character *c) {
    _init_frames(gMayFrames, &may_idle_tiles, &may_palette);
    c->frames      = gMayFrames;
    c->totalFrames = ENEMY_TOTAL_FRAMES;
    c->tileSize    = 0x200;
    c->hp = c->maxHp = 5;
    c->isEnemy     = true;
    c->name        = sMayName;
    c->speed       = 0;           // May é mais rápida
    c->hurtDuration= 22;
}

void enemy_init_andore(Character *c) {
    _init_frames(gAndoreFrames, &andore_idle_tiles, &andore_palette);
    c->frames      = gAndoreFrames;
    c->totalFrames = ENEMY_TOTAL_FRAMES;
    c->tileSize    = 0x200;
    c->hp = c->maxHp = 16;
    c->isEnemy     = true;
    c->name        = sAndoreName;
    c->speed       = 2;           // Andore é lento
    c->hurtDuration= 40;
}

// ----------------------------------------------------------------
//  Profundidade Z: inimigos se ajustam ao Y do jogador
// ----------------------------------------------------------------
static void _adjust_depth_z(Character *c, Character *player) {
    // Tenta se alinhar no mesmo Y do jogador enquanto se aproxima
    int dy = player->y - c->y;
    if (dy >  3) c->y++;
    if (dy < -3) c->y--;
}

// ----------------------------------------------------------------
//  IA principal
// ----------------------------------------------------------------
#define ATTACK_RANGE_X   40
#define ATTACK_RANGE_Y    8
#define AGGRO_RANGE_X   100
#define RETREAT_X        60

void enemy_update(Character *c, Character *player) {
    if (!c->alive || !player->alive) return;

    // Hurt/Dead: character.c cuida do timer
    if (c->state == STATE_HURT || c->state == STATE_DEAD) {
        if (c->state == STATE_HURT) char_update_hurt(c);
        return;
    }

    // Acha o índice desta entidade para a IA
    int idx = (int)(c - gCharacters);
    EnemyAI *ai = &sAI[idx];

    // Diminui cooldown de ataque
    if (ai->attackCooldown > 0) ai->attackCooldown--;

    int dx    = player->x - c->x;
    int dy    = player->y - c->y;
    int absDx = (dx > 0) ? dx : -dx;
    int absDy = (dy > 0) ? dy : -dy;

    // Flip: enfrenta o jogador
    c->hflip = (dx < 0) ? 1 : 0;

    switch (ai->phase) {

        // ── FASE 0: Aproximação ───────────────────────────────
        case 0:
            if (absDx > AGGRO_RANGE_X) {
                // Longe: caminha normalmente
                char_set_velocity(c, (dx > 0) ? ENEMY_SPEED_X : -ENEMY_SPEED_X,
                                     (dy > 0) ? ENEMY_SPEED_Y : -ENEMY_SPEED_Y);
                if (c->state != STATE_WALK)
                    char_set_state(c, STATE_WALK, ENEMY_WALK_1);
            } else {
                // Dentro do alcance de ataque: aumenta agressão
                ai->aggroTimer++;
                _adjust_depth_z(c, player);

                if (ai->aggroTimer > 30 && ai->attackCooldown == 0) {
                    ai->phase = 1;   // → atacar
                } else {
                    // Orbita: mantém distância lateral
                    s8 orbitX = (absDx > ATTACK_RANGE_X) ?
                                (dx > 0 ? 1 : -1) : 0;
                    char_set_velocity(c, orbitX, 0);
                    if (c->state != STATE_WALK && orbitX != 0)
                        char_set_state(c, STATE_WALK, ENEMY_WALK_1);
                    else if (orbitX == 0 && c->state == STATE_WALK)
                        char_set_state(c, STATE_IDLE, ENEMY_IDLE_1);
                }
            }
            break;

        // ── FASE 1: Ataque ────────────────────────────────────
        case 1:
            char_set_velocity(c, 0, 0);
            if (absDx < ATTACK_RANGE_X && absDy < ATTACK_RANGE_Y) {
                // Ataca
                if (c->state != STATE_PUNCH) {
                    char_set_state(c, STATE_PUNCH, ENEMY_ATTACK_1);
                    ai->attackCooldown = 60;
                    ai->aggroTimer     = 0;
                    ai->phase          = 2;  // → recuar
                }
            } else {
                // Perdeu o alvo: volta à aproximação
                ai->phase = 0;
            }
            break;

        // ── FASE 2: Recuo ─────────────────────────────────────
        case 2:
            ai->retreatTimer++;
            // Recua levemente
            char_set_velocity(c, (dx > 0) ? -1 : 1, 0);
            if (c->state != STATE_WALK)
                char_set_state(c, STATE_WALK, ENEMY_WALK_1);

            if (ai->retreatTimer > 20) {
                ai->retreatTimer = 0;
                ai->phase        = 0;
            }
            break;
    }
}
