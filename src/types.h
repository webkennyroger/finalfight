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
#define STATE_IDLE      0
#define STATE_WALK      1
#define STATE_PUNCH     2
#define STATE_KICK      3
#define STATE_HURT      4
#define STATE_DEAD      5
#define STATE_JUMP      6
#define STATE_GRAB      7
#define STATE_JUMP_KICK 8
#define STATE_DASH      9
#define STATE_SPECIAL1 10
#define STATE_SPECIAL2 11

// ============================================================
//  Sub-estados do Guy (índice no array gGuyFrames)
//
//  Contagem de frames reais dos sprites:
//    idle=1  walk=6  punch=4  kick=4  damage=3  grab=3
//    jump=4  jump_kick=4
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

#define GUY_JUMP_KICK_1 27
#define GUY_JUMP_KICK_2 28
#define GUY_JUMP_KICK_3 29
#define GUY_JUMP_KICK_4 30

// DASH (6 frames, reuses walk tiles but STATE_DASH)
#define GUY_DASH_1      31
#define GUY_DASH_2      32
#define GUY_DASH_3      33
#define GUY_DASH_4      34
#define GUY_DASH_5      35
#define GUY_DASH_6      36

// Bushin Senpukyaku — spinning kick (4 frames, reuses kick tiles)
#define GUY_SPECIAL1_1  37
#define GUY_SPECIAL1_2  38
#define GUY_SPECIAL1_3  39
#define GUY_SPECIAL1_4  40

// Bushin Goraisenpujin — projectile (3 frames, reuses punch tiles)
#define GUY_SPECIAL2_1  41
#define GUY_SPECIAL2_2  42
#define GUY_SPECIAL2_3  43

#define GUY_TOTAL_FRAMES 44

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
//  Estrutura de scroll de background
// ============================================================
typedef struct {
    u8   bgLayer;
    u16  vramAddress;
    u16  scrollX;
    u16  scrollY;
    u8  *mapData;
    u8   mapPage;
    bool firstPage;
    u16  maxScrollX;
    u8   scrollSpeed;
} ScrollBG;

// ============================================================
//  Fila DMA para VRAM
// ============================================================
typedef struct {
    u8  *data;
    u16  vramDest;
    u16  size;
} VramEntry;

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
