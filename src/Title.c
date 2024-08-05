#include "bios.h"
#include "gfx.h"
#include "App.h"
#include "Title.h"
#include "Back.h"
void TitleInitialize(void) {
    appState = TITLE_STATE_MOVE;
    VDP_drawImageEx(BG_A, &title_img, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, titleInd), 7, 10, FALSE, TRUE);
    VDP_drawText("PRESS START BUTTON",6,15);
}
void TitleMove(void) {
    if(input&BUTTON_START) {
      appState = TITLE_STATE_LOOP;
      appPhase = 0;
  }
}
void TitleLoop(void) {
  // 待機の完了待ち
  if (++appPhase < 60) return;
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
