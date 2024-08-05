// bios.h : BIOS コール
#include "genesis.h"
#define MAX_SPRITE       32
#define MAX_SPRITE_ANIM  64
typedef void (*FP)(void);

extern u16 input,trigger;
extern u8 sprite[128];
extern Sprite* sprites[MAX_SPRITE];

void SystemInitialize(void);
void SystemClearSprite(void);
void SystemSetSprite(u8 frame,s16 x,s16 y);
void SystemTransferSprite(void);
