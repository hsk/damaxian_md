// Back.c : 背景
#include <genesis.h>
#include "gfx.h"

s16 vScrollB[15]     = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
s16 vScrollB2[15]    = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, };
s8 planeBDeltas[15]  = {10,15, 5,23, 9,24,14, 6,16, 7,18,27, 8,21,17, };
extern u16 titleInd;
extern u16 backInd;

void BackLoad(void) { // 背景をロードする

    VDP_loadFontData(font_tile.tiles,64,CPU);

    VDP_loadTileSet(bg_img.tileset, backInd, CPU);
    VDP_drawImageEx(BG_B, &bg_img, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, backInd), 0, 0, FALSE, TRUE);
    PAL_setPalette(PAL0, bg_img.palette->data, CPU);
    PAL_setPalette(PAL1, &title_pal.data[0], CPU);
    PAL_setPalette(PAL2, &title_pal.data[16], CPU);
    PAL_setPalette(PAL3, &title_pal.data[16*2], CPU);
    VDP_loadTileSet(title_img.tileset, titleInd, CPU);

    VDP_setTextPriority(1);
    VDP_setTextPalette(PAL1);
    VDP_drawText("HI SCORE",31,5);
    VDP_drawText("SCORE",31,8);
    VDP_drawText("^",31,20);
    VDP_drawText("_",31,22);
}
void BackUpdate(void) { // 背景を更新する
    for (int i = 0; i < 15; i++) {
      vScrollB[i] -= planeBDeltas[i];
      vScrollB2[i] = vScrollB[i]>>4;
    }
    VDP_setVerticalScrollTile(BG_B, 0, vScrollB2, 15, DMA); // use array to set plane offsets
    static u8 count = 0;
    if ((count++&7)==0)
      PAL_setColors(1,&bg_img.palette->data[1+(random()&7)],3, CPU);
}
