#define MAX_SCORE  0x99999999
#define SCORE_INIT 0x00000000
#define HISCORE_INIT 0x00005000
void score_init(void);
void score_update(void);
void score_add(void);
u32 sbcd32(u32 a,u32 b);
