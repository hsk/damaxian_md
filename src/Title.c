#include "bios.h"
#include "gfx.h"
#include "App.h"
#include "Title.h"
#include "Back.h"
#include "Sound.h"
u8 title_n;
u8 title_x;
void TitleInitialize(void) {
    soundRequest[0] = (void*)mmlNull;// 演奏の停止
    soundRequest[1] = (void*)mmlNull;
    soundRequest[2] = (void*)mmlNull;
    soundRequest[3] = (void*)mmlNull;
    title_x = 0;
    title_n = 0;
    appState = TITLE_STATE_MOVE;
    VDP_drawImageEx(BG_A, &title_img, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, titleInd), 7, 10, FALSE, TRUE);
    VDP_drawText("PRESS START BUTTON",6,15);
}
void TitleMove(void) {
    title_n++;
    if((title_n&7)==0) {
      VDP_setTileMapEx(BG_A, title_img.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, titleInd),
        7+title_x*2, 10, title_x*2, 0, 2, 2, DMA);
      title_x = random()&7;
      VDP_setTileMapEx(BG_A, title_img.tilemap, TILE_ATTR_FULL(PAL2+(random()&1), FALSE, FALSE, FALSE, titleInd),
        7+title_x*2, 10, title_x*2, 0, 2, 2, DMA);
    }
    if(input&BUTTON_START) {
      appState = TITLE_STATE_LOOP;
      soundRequest[0] = (void*)mmlTitleStartChannel0;// 演奏の開始
      soundRequest[1] = (void*)mmlTitleStartChannel1;
      soundRequest[2] = (void*)mmlTitleStartChannel2;
  }
}
void TitleLoop(void) {
  // 待機の完了待ち
  if (soundRequest[0]||soundPlay[0]) return;
  appMode = APP_MODE_GAME;
  appState = 0;
  VDP_drawText("                    ",7,10);
  VDP_drawText("                    ",7,11);
  VDP_drawText("                  ",6,15);
}
void TitleUpdate(void) {
  if (appState == TITLE_STATE_INIT) TitleInitialize();
  else if (appState == TITLE_STATE_MOVE) TitleMove();
  else                                   TitleLoop();
  BackUpdate();
}
