#include "bios.h"
#include "score.h"
#include "App.h"
static inline u8 abcd(u8 d0, u8 d1, u8 flags_in, u8* flags_out) {
	u8 res;
	asm volatile ("move %4,%%cc\n"
		"abcd %3,%0\n"
		"move %%sr,%1\n"
		: "=d" (res), "=d" (*flags_out)
		: "0" (d0), "d" (d1), "d" (flags_in)
		: "cc"
	);
	return res;
}
static inline u32 abcd32(u32 a,u32 b) {
  u8 flg=0;
  u32 res=0;
  for(int i=0;i<4;i++) {
    res += abcd(a & 0xff,b & 0xff,flg,&flg)<<(i*8);
    a >>= 8;
    b >>= 8;
  }
  if (flg&1) return 0x99999999;
  return res;
}
static u8 sbcd_(u8 d0, u8 d1, u8 flags_in, u8* flags_out) {
	u8 res;
	asm volatile ("move %4,%%cc\n"
		"sbcd %3,%0\n"
		"move %%sr,%1\n"
		: "=d" (res), "=d" (*flags_out)
		: "0" (d0), "d" (d1), "d" (flags_in)
		: "cc"
	);
	return res;
}
u32 sbcd32(u32 a,u32 b) {
  u8 flg=0;
  u32 res=0;
  for(int i=0;i<4;i++) {
    res += sbcd_(a & 0xff,b & 0xff,flg,&flg)<<(i*8);
    a >>= 8;
    b >>= 8;
  }
  if (flg) return 0;
  return res;
}

u32 appScore = SCORE_INIT;
static char str[10];
void score_add(void) {
  appScore = abcd32(appScore,0x100);
}
void scoreToStr(u32 score,char* score_str) {
  int j=28;
  for(;j>=1;j-=4) {
    if(score>>j) break;
    *score_str++ = ' ';
  }
  for(;j>=0;j-=4) *score_str++ = ((score>>j) & 0xf)+'0'; 
}

void score_update(void) {
    scoreToStr(appScore,str);
    VDP_drawText(str,31,9);
    scoreToStr(appTimer,str);
    VDP_drawText(&str[4],31+4,22);
}
void score_init(void) {
    str[8]=0;
}
