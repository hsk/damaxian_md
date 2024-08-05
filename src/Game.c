// Game.c : ゲーム画面
#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
// 変数の定義
u8 gameFlag;   // フラグ
static void GameInitialize(void);
static void GameLoad(void);
static void GameStart(void);
static void GamePlay(void);
void GameUpdate(void) { // ゲームを更新する
    if (appState == GAME_STATE_INITIALIZE)   GameInitialize();// 初期化
    else if (appState == GAME_STATE_LOAD)    GameLoad();      // ロード
    else if (appState == GAME_STATE_START)   GameStart();     // 開始
    else if (appState == GAME_STATE_PLAY)    GamePlay();      // プレイ
    // 一時停止
    if (gameFlag & (1 << GAME_FLAG_PAUSE)) return;
    ShipUpdate();   // 自機の更新
    ShotUpdate();   // ショットの更新
    EnemyUpdate();  // 敵の更新
    BulletUpdate(); // 弾の更新
}
static void GameInitialize(void) { // ゲームを初期化する
    // スプライトのクリア
    SystemClearSprite();
    ShipInitialize();   // 自機の初期化
    ShotInitialize();   // ショットの初期化
    EnemyInitialize();  // 敵の初期化
    BulletInitialize(); // 弾の初期化
    gameFlag = 0; // フラグの初期化
    // 状態の更新
    appState = GAME_STATE_LOAD;
    appPhase = APP_PHASE_NULL;
}
static void GameLoad(void) { // ゲームをロードする
    if (appPhase == 0) { // フェーズ0
        gameFlag |= 1 << GAME_FLAG_STATUS; // フラグの設定
        appPhase++;                        // 状態の更新
    } else {             // フェーズ1 ロードの処理
        appState = GAME_STATE_START; // 状態の更新
        appPhase = APP_PHASE_NULL;
    }
}
static void GameStart(void) { // ゲームを開始する
    if (appPhase == 0) {// 初期化
        VDP_drawText("START!",12,12);
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++; // 状態の更新
    }
    if (++appPhase < 60) return;
    VDP_drawText("                  ",6,12);
    appState = GAME_STATE_PLAY; // 状態の更新
    appPhase = APP_PHASE_NULL;
}
static void GameCheckShotEnemy(void);
static void GamePlay(void) { // ゲームをプレイする
    if (appPhase == 0) {// 初期化
        // フラグの設定
        gameFlag &= ~(1<<GAME_FLAG_PAUSE);
        gameFlag |= (1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_STATUS);
        appPhase++; // 状態の更新
    }
    // プレイの処理
    if (input&BUTTON_START&trigger) { // ESC ボタンが押された
        gameFlag ^= (1 << GAME_FLAG_PAUSE); // ポーズフラグをフリップ
    }
    if (gameFlag & (1<<GAME_FLAG_PAUSE)) return; // 一時停止
    GameCheckShotEnemy();
}

static void GameShootBack(ENEMY* ix);
static void GameCheckShotEnemy(void) { // ショットと敵のヒットチェックを行う
    for(SHOT* shot = shots;shot<&shots[SHOT_SIZE];shot++) {// ショットの走査
        if (shot->state == SHOT_STATE_NULL) continue; // ショットの存在
        ENEMY* ix = enemies;
        for (u8 b=0;b<ENEMIES_SIZE;ix++, b++) {// 敵の走査
            if (ix->nodamage) continue;// 敵の存在
            // ヒットチェック
            s16 a = ix->x-shot->x;
            if (!(FIX16(-8) <= a && a <= FIX16(8))) continue;
            a = ix->y-shot->y;
            if (!(FIX16(-0xb) <= a && a <= FIX16(0xb))) continue;
            // あたり
            ix->nodamage = 0x80;// 敵のノーダメージの更新
            ix->state = ENEMY_STATE_BOMB;// 敵の状態の更新
            ix->phase = APP_PHASE_NULL;
            GameShootBack(ix);// 敵の撃ち返し
            shot->state = SHOT_STATE_NULL;// ショットの状態の更新
        }
    }
}
static void GameShootBack(ENEMY* ix) { // 敵が弾を打ち返す
    // 敵が自機に近い場合は撃ち返さない
    if (ship.y-FIX16(0x20) < ix->y) {
        s16 a = ship.x-FIX16(0x18);
        if (a < ix->x && ix->x <= a+FIX16(0x30)) return;
    }
    u16 angle = atan2Fix16(ship.x - ix->x,ship.y - ix->y);
    BULLET* iy=BulletEntry(0x10);
    if (iy==NULL) return;
    // 弾の位置の設定
    iy->x = ix->x;
    iy->y = ix->y;
    iy->spx = cosFix16(angle);
    iy->spy = sinFix16(angle);
}
