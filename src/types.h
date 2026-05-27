#ifndef TYPES_H
#define TYPES_H

#include <snes.h>

// ============================================================
//  Final Fight SNES - Tipos e Constantes Compartilhadas
// ============================================================

#define MAX_CHARS       5
#define MAX_SPR_QUEUE   16
#define HUD_SIZE        160
#define MAX_PROJECTILES 3

// ============================================================
//  Estados principais do personagem
// ============================================================
#define STATE_IDLE          0
#define STATE_WALK          1
#define STATE_PUNCH         2
#define STATE_KICK          3
#define STATE_HURT          4
#define STATE_DEAD          5
#define STATE_JUMP          6
#define STATE_GRAB          7
#define STATE_JUMP_KICK     8
#define STATE_DASH          9
#define STATE_SPECIAL1     10  // Bushin Senpuukyaku
#define STATE_SPECIAL2     11  // Bushin Goraisenpujin (projétil)
#define STATE_LOW_KICK     12
#define STATE_DASH_ATTACK  13  // Run attack (dash + Y)
#define STATE_ELBOW_DROP   14  // Jump + Down + Y
#define STATE_FLYING_KICK  15  // Jump with direction + Y
#define STATE_WALL_JUMP    16  // Jump toward wall + B
#define STATE_TURN_ATTACK  17  // A button (reverse attack)
#define STATE_GRAB_PUNCH   18  // Y while grabbing
#define STATE_SHOULDER_THROW 19  // Opposite direction + Y while grabbed
#define STATE_BACK_VAULT   20  // B while grabbed
#define STATE_GERMAN_SUPLEX 21  // Y after back grab
#define STATE_MEGA_CRASH   22  // Y+B (spending HP)
#define STATE_SENPUUKYAKU  23  // Motion special (qcf back + Y)
#define STATE_IZUNA_OTOSHI 24  // Motion special (qcf forward + B)
#define STATE_SUPER        25  // Super move (Musou Renka)

// ============================================================
//  Sub-estados do Guy (índice no array gGuyFrames)
//
//  Frames com tiles DEDICADOS (tem .pic próprio):
//    idle=1  walk=8  punch=4  kick=4  damage=3  grab=3  jump=4
//    jump_kick=5  dash_kick=8  low_kick=3  elbow_drop=4
//    senpuukyaku=8  super=8  superpreto=4
//
//  Frames que REUSAM tiles de outros estados:
//    dash(6) -> walk  special2(3) -> punch  wall_jump(3) -> jump
//    turn_attack(3) -> punch  grab_attack(8) -> grab
//    mega_crash(7) -> kick  izuna_otoshi(6) -> jump
//    grapple_finish(8) -> grab
// ============================================================
#define GUY_IDLE_1       0

#define GUY_WALK_1       1
#define GUY_WALK_2       2
#define GUY_WALK_3       3
#define GUY_WALK_4       4
#define GUY_WALK_5       5
#define GUY_WALK_6       6
#define GUY_WALK_7       7
#define GUY_WALK_8       8

#define GUY_PUNCH_1      9
#define GUY_PUNCH_2     10
#define GUY_PUNCH_3     11
#define GUY_PUNCH_4     12

#define GUY_KICK_1      13
#define GUY_KICK_2      14
#define GUY_KICK_3      15
#define GUY_KICK_4      16

#define GUY_HURT_1      17
#define GUY_HURT_2      18
#define GUY_DEAD_1      19

#define GUY_GRAB_1      20
#define GUY_GRAB_2      21
#define GUY_GRAB_3      22

#define GUY_JUMP_1      23
#define GUY_JUMP_2      24
#define GUY_JUMP_3      25
#define GUY_JUMP_4      26

// Jump Kick (dedicated tiles, 4 frames)
#define GUY_JUMP_KICK_1 27
#define GUY_JUMP_KICK_2 28
#define GUY_JUMP_KICK_3 29
#define GUY_JUMP_KICK_4 30
// Dash (reuses walk tiles)
#define GUY_DASH_1 31
#define GUY_DASH_2 32
#define GUY_DASH_3 33
#define GUY_DASH_4 34
#define GUY_DASH_5 35
#define GUY_DASH_6 36

// Special1 — Senpuukyaku (dedicated tiles, 8 frames)
#define GUY_SPECIAL1_1 37
#define GUY_SPECIAL1_2 38
#define GUY_SPECIAL1_3 39
#define GUY_SPECIAL1_4 40
#define GUY_SPECIAL1_5 41
#define GUY_SPECIAL1_6 42
#define GUY_SPECIAL1_7 43
#define GUY_SPECIAL1_8 44

// Special2 — Goraisenpujin (reuses punch tiles, 3 frames)
#define GUY_SPECIAL2_1 45
#define GUY_SPECIAL2_2 46
#define GUY_SPECIAL2_3 47

// Dash Kick / Run Attack (dedicated tiles, 8 frames)
#define GUY_DASH_KICK_1 48
#define GUY_DASH_KICK_2 49
#define GUY_DASH_KICK_3 50
#define GUY_DASH_KICK_4 51
#define GUY_DASH_KICK_5 52
#define GUY_DASH_KICK_6 53
#define GUY_DASH_KICK_7 54
#define GUY_DASH_KICK_8 55

// Low Kick (dedicated tiles, 3 frames)
#define GUY_LOW_KICK_1 56
#define GUY_LOW_KICK_2 57
#define GUY_LOW_KICK_3 58

// Elbow Drop (dedicated tiles, 4 frames)
#define GUY_ELBOW_DROP_1 59
#define GUY_ELBOW_DROP_2 60
#define GUY_ELBOW_DROP_3 61
#define GUY_ELBOW_DROP_4 62

// Super move (dedicated tiles, 8 frames)
#define GUY_SUPER_1 63
#define GUY_SUPER_2 64
#define GUY_SUPER_3 65
#define GUY_SUPER_4 66
#define GUY_SUPER_5 67
#define GUY_SUPER_6 68
#define GUY_SUPER_7 69
#define GUY_SUPER_8 70

// Super preto (silhouette, dedicated tiles, 4 frames)
#define GUY_SUPERPRETO_1 71
#define GUY_SUPERPRETO_2 72
#define GUY_SUPERPRETO_3 73
#define GUY_SUPERPRETO_4 74

// --- Frames que reusam tiles de outros estados ---

// Wall Jump (reuses jump tiles)
#define GUY_WALL_JUMP_1 75
#define GUY_WALL_JUMP_2 76
#define GUY_WALL_JUMP_3 77

// Turn Attack (reuses punch tiles)
#define GUY_TURN_ATTACK_1 78
#define GUY_TURN_ATTACK_2 79
#define GUY_TURN_ATTACK_3 80

// Grab Attack / Grab Punches (reuses grab tiles)
#define GUY_GRAB_ATTACK_1 81
#define GUY_GRAB_ATTACK_2 82
#define GUY_GRAB_ATTACK_3 83
#define GUY_GRAB_ATTACK_4 84
#define GUY_GRAB_ATTACK_5 85
#define GUY_GRAB_ATTACK_6 86
#define GUY_GRAB_ATTACK_7 87
#define GUY_GRAB_ATTACK_8 88

// Mega Crash (Y+B, reuses kick tiles)
#define GUY_MEGA_CRASH_1 89
#define GUY_MEGA_CRASH_2 90
#define GUY_MEGA_CRASH_3 91
#define GUY_MEGA_CRASH_4 92
#define GUY_MEGA_CRASH_5 93
#define GUY_MEGA_CRASH_6 94
#define GUY_MEGA_CRASH_7 95

// Izuna Otoshi (reuses jump tiles)
#define GUY_IZUNA_OTOSHI_1 96
#define GUY_IZUNA_OTOSHI_2 97
#define GUY_IZUNA_OTOSHI_3 98
#define GUY_IZUNA_OTOSHI_4 99
#define GUY_IZUNA_OTOSHI_5 100
#define GUY_IZUNA_OTOSHI_6 101

// Grapple Finish / Suplex (reuses grab tiles)
#define GUY_GRAPPLE_FINISH_1 102
#define GUY_GRAPPLE_FINISH_2 103
#define GUY_GRAPPLE_FINISH_3 104
#define GUY_GRAPPLE_FINISH_4 105
#define GUY_GRAPPLE_FINISH_5 106
#define GUY_GRAPPLE_FINISH_6 107
#define GUY_GRAPPLE_FINISH_7 108
#define GUY_GRAPPLE_FINISH_8 109

#define GUY_TOTAL_FRAMES 110

// ============================================================
//  Sub-estados dos inimigos
// ============================================================
#define ENEMY_IDLE_1    0
#define ENEMY_WALK_1    1
#define ENEMY_WALK_2    2
#define ENEMY_ATTACK_1  3
#define ENEMY_HURT_1    4
#define ENEMY_DEAD_1    5
#define ENEMY_TOTAL_FRAMES 6

// ============================================================
//  Parâmetros de movimento
// ============================================================
#define PLAYER_SPEED_X  3
#define PLAYER_SPEED_Y  2
#define DASH_SPEED_X    6
#define ENEMY_SPEED_X   1
#define ENEMY_SPEED_Y   1

// ============================================================
//  Física de pulo
// ============================================================
#define GRAVITY          1
#define JUMP_VEL        -7
#define GROUND_Y        160

// ============================================================
//  Limites de tela
// ============================================================
#define SCREEN_MIN_X   -16
#define SCREEN_MAX_X    218
#define SCREEN_MIN_Y    90
#define SCREEN_MAX_Y    190

// ============================================================
//  Tamanho do tile no VRAM para sprites 64×96 / 4bpp / -s 16
//
//  A saída real gerada pelo gfx2snes sem pad excessivo
//  é 8 cols x 12 rows (em blocos de 8x8) = 96 tiles x 32 bytes = 3072 bytes.
// ============================================================
#define GUY_TILE_SIZE   0x0C00   // 3072 bytes por frame no VRAM

// ============================================================
//  Estrutura de um frame de sprite
// ============================================================
typedef struct {
    u8  *tiles;         // ponteiro para os tiles no ROM
    u8  *palette;       // ponteiro para a paleta
    u16  state;         // estado atual deste frame
    u16  nextState;     // próximo estado ao expirar
    u16  nextSubState;  // próximo sub-estado ao expirar
    u16  duration;      // duração em frames de vídeo
    u8   hitboxX1;      // hitbox relativa ao sprite
    u8   hitboxX2;
    u8   hitboxY1;
    u8   hitboxY2;
    bool hasHitbox;     // true = frame tem hitbox ativa
} SpriteFrame;

// ============================================================
//  Estrutura do personagem
// ============================================================
typedef struct _Character {
    u16         tileSize;
    u16         paletteSize;
    u8          oamAddress;
    SpriteFrame *frames;
    u16         totalFrames;
    u16         vramAddress;
    u16         vramOffset;

    u8  state;
    u8  subState;
    u8  prevState;
    u8  prevSubState;
    u16 frameCounter;

    int   x, y;
    int   oldX, oldY;
    s8    velX, velY;
    u8    speed;
    u8    speedTimer;
    u8    hflip;
    int   groundY;

    u8   hp;
    u8   maxHp;
    bool alive;
    bool isEnemy;
    bool isInvincible;
    bool isHurt;
    u8   hurtTimer;
    u8   hurtDuration;
    u8   hurtShake;

    u8    paletteSlot;
    bool  visible;
    u8    priority;

    char *name;
} Character;

// ============================================================
//  Projétil
// ============================================================
typedef struct {
    int  x, y;
    int  velX;
    u8   oamId;
    u8   tileIndex;
    u8   paletteSlot;
    u8   damage;
    u8   life;
    bool active;
    bool friendly;
    u8   hflip;
} Projectile;

// ============================================================
//  Globais compartilhados
// ============================================================
extern Character  gCharacters[MAX_CHARS];
extern Character *gPlayer;
extern Character *gYOrder[MAX_CHARS];
extern Projectile gProjectiles[MAX_PROJECTILES];

#endif // TYPES_H
