// App.c : アプリケーション
#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Back.h"
#include "score.h"
#include "Title.h"
// 変数の定義
u8 appMode;         // モード
u8 appState;        // 状態
u8 appPhase;
u32 appTimer;
void AppInitialize(void) { // アプリケーションを初期化する
    // アプリケーションの初期化
    appMode = APP_MODE_LOAD;// モードの初期化
    appTimer = 0;
}
static void AppLoad(void) { // アプリケーションを読み込む
    BackLoad();
    BackUpdate();
    score_update();
    appMode = APP_MODE_TITLE;// モードの更新
    appState = 0;// 状態の更新
}
void AppUpdate(void) { // アプリケーションを更新する
    if (appMode == APP_MODE_GAME)       GameUpdate();   // ゲーム画面
    else if (appMode == APP_MODE_TITLE) TitleUpdate();  // タイトル画面
    else if (appMode == APP_MODE_LOAD)  AppLoad();      // 読み込み
    random();
}
