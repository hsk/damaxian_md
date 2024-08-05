#include "bios.h"
#include "gfx.h"
#include "game.h"
#include "sound.h"
u16 input,trigger;

volatile u8 flg;
u16 m = 9999;
static void vint() {
  flg = FALSE;
}
typedef struct
{
    s8 offsetY;          // respect VDP sprite field order, may help
    u8 offsetYFlip;
    u8 size;
    s8 offsetX;
    u8 offsetXFlip;
    u16 attribut;
}  FrameSPR;
FrameSPR framesprs[MAX_SPRITE_ANIM];
typedef struct FSPR {
  u8 numSpr;
  u8 dummy;
  FrameSPR* spr;
} FSPR;
FSPR fsprs[65];

typedef struct VSPR {
  s16 x;
  s16 y;
  FSPR* frame;
} VSPR;
VSPR* vsprs_pos;
VSPR vsprs[64];
u8 vdpPos=0;
u16 titleInd;
u16 backInd;
void SystemInitialize(void) {
    SYS_disableInts();
    VDP_setPlaneSize(64, 32, TRUE);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_COLUMN);
    FrameSPR*frp=framesprs;
    curTileInd = TILE_USER_INDEX;
    for (u8 j=0;j<spr.numAnimation;j++) {
        Animation* anim = spr.animations[j];
        for (u8 i = 0; i < anim->numFrame; i++) {
            AnimationFrame* animFrm = anim->frames[i];
            TileSet* tileset = animFrm->tileset;
            VDP_loadTileSet(tileset, curTileInd, TRUE);
            fsprs[j*8+i].numSpr = animFrm->numSprite;
            for(u8 k=0;k<animFrm->numSprite;k++){
                frp->attribut = TILE_ATTR_FULL(PAL1,0,0,0,curTileInd);
                frp->offsetY = animFrm->frameVDPSprites[k].offsetY-8;
                frp->offsetYFlip = animFrm->frameVDPSprites[k].offsetYFlip;
                frp->size = animFrm->frameVDPSprites[k].size;
                frp->offsetX = animFrm->frameVDPSprites[k].offsetX-8;
                frp->offsetXFlip = animFrm->frameVDPSprites[k].offsetXFlip;
                fsprs[j*8+i].spr = frp++;
                curTileInd += animFrm->frameVDPSprites[k].numTile;
            }
        }
    }
    titleInd = curTileInd;
    curTileInd += title_img.tileset->numTile;
    backInd=curTileInd;
    curTileInd += bg_img.tileset->numTile;

    fsprs[64].numSpr=0;
    PAL_setPalette(PAL1, spr.palette->data, CPU);

    SPR_initEx(0);
    for(u8 i = 0; i < 80; i++) vdpSpriteCache[i].link=i+1;
    vdpPos=0;
    SystemClearSprite();
    SYS_setVIntCallback(&vint);
    SYS_enableInts();
    m = 9999;
}
void SystemClearSprite(void) { // スプライトをクリアする
    // スプライトのクリア
    vsprs_pos=vsprs;
    vdpPos=1;
}
void SystemSetSprite(u8 frame,s16 x,s16 y) { // スプライトを設定する
    VSPR* spr = vsprs_pos++;
    spr->x = 0x80+ fix16ToInt(x)+24;
    spr->y = 0x80+ fix16ToInt(y);
    spr->frame = &fsprs[frame];
}

char str[32];
#if 0
static void wait() {
    u16 n=0;
    flg=TRUE;
    while(flg)n++;
    if (m > n)m = n;
    uintToStr(m,str,4);
    VDP_drawText(str,10,2);

}
#else
static void wait() {
    flg=TRUE;
    while(flg);
}
#endif
void doDmaFast(void* from, u16 to, u16 len, s16 step) {
    vu16 *pw;
    u32 fromAddr;
    vu32 cmdbuf[1];
    u16* cmdbufp;

    fromAddr = (u32) from;

    VDP_setAutoInc(step);

    pw = (vu16*) VDP_CTRL_PORT;

    // Setup DMA length (in word here)
    *pw = 0x9300 + (len & 0xff);
    *pw = 0x9400 + ((len >> 8) & 0xff);

    // Setup DMA address
    fromAddr >>= 1;
    *pw = 0x9500 + (fromAddr & 0xff);
    fromAddr >>= 8;
    *pw = 0x9600 + (fromAddr & 0xff);
    fromAddr >>= 8;
    *pw = 0x9700 + (fromAddr & 0x7f);
    cmdbuf[0] = VDP_DMA_VRAM_ADDR((u32)to);
    cmdbufp = (u16*) cmdbuf;
    *pw = *cmdbufp++;
    *pw = *cmdbufp;
}
void SystemTransferSprite(void) {
    vdpSpriteCache[vdpPos-1].link=vdpPos;
    vdpPos=0;
    for (VSPR* vs = vsprs;vs<vsprs_pos;vs++) {
        FSPR* fs = vs->frame;
        FrameSPR* sp = fs->spr;
        for (u8 j=0;j<fs->numSpr;j++,sp++) {
            VDPSprite* v = &vdpSpriteCache[vdpPos++];
            v->x=vs->x+sp->offsetX;
            v->y=vs->y+sp->offsetY;
            v->size = sp->size;
            v->attribut = sp->attribut;
        }
    }
    vsprs_pos=vsprs;
    if (vdpPos==0) {
        vdpPos=1;
        vdpSpriteCache[0].y=0;
    }
    vdpSpriteCache[vdpPos-1].link=0;
    wait();
    SystemUpdateSound();
    //VDP_updateSprites(vdpPos,DMA);
    //DMA_transfer(DMA, DMA_VRAM, vdpSpriteCache, VDP_SPRITE_TABLE, (sizeof(VDPSprite) * vdpPos) / 2, 2);
    if (!(gameFlag & (1 << GAME_FLAG_PAUSE))) 
        doDmaFast(vdpSpriteCache, VDP_SPRITE_TABLE, (sizeof(VDPSprite)/ 2 * vdpPos) , 2);
    JOY_update();
    trigger = ~input;
    input = JOY_readJoypad(JOY_1);
}
