// Game.c : ゲーム画面
#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Enemy.h"
#include "Bullet.h"
#include "Back.h"
#include "score.h"
// 変数の定義
u8 gameFlag;   // フラグ
static u8 gameCount;   // カウント
static void GameInitialize(void);
static void GameLoad(void);
static void GameStart(void);
static void GamePlay(void);
static void GameTimeUp(void);
static void GameOver(void);
static void GameHiscore(void);
static void GameUnload(void);
static void GameEnd(void);

void GameUpdate(void) { // ゲームを更新する
    if (appState == GAME_STATE_INITIALIZE)   GameInitialize();// 初期化
    else if (appState == GAME_STATE_LOAD)    GameLoad();      // ロード
    else if (appState == GAME_STATE_START)   GameStart();     // 開始
    else if (appState == GAME_STATE_PLAY)    GamePlay();      // プレイ
    else if (appState == GAME_STATE_TIMEUP)  GameTimeUp();    // タイムアップ
    else if (appState == GAME_STATE_OVER)    GameOver();      // オーバー
    else if (appState == GAME_STATE_HISCORE) GameHiscore();   // ハイスコア
    else if (appState == GAME_STATE_UNLOAD)  GameUnload();    // アンロード
    else                                     GameEnd();       // 終了
    // 一時停止
    if (gameFlag & (1 << GAME_FLAG_PAUSE)) return;
    ShipUpdate();   // 自機の更新
    ShotUpdate();   // ショットの更新
    EnemyUpdate();  // 敵の更新
    BulletUpdate(); // 弾の更新
    score_update();
    BackUpdate();
}
static void GameInitialize(void) { // ゲームを初期化する
    // スプライトのクリア
    SystemClearSprite();
    appTimer = 0x3000;
    appRate = 0x10;
    appScore = 0x0;
    ShipInitialize();   // 自機の初期化
    ShotInitialize();   // ショットの初期化
    EnemyInitialize();  // 敵の初期化
    BulletInitialize(); // 弾の初期化
    score_init();

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
static void GameCheckShipBullet(void);
static void GameCheckShipEnemy(void);
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
    appTimer=sbcd32(appTimer,1);
    if (appTimer==0) {
        appState = GAME_STATE_TIMEUP;
        appPhase = APP_PHASE_NULL;
    }
    GameCheckShotEnemy();
    GameCheckShipBullet(); // 自機と弾のヒットチェック
    GameCheckShipEnemy();  // 自機と敵のヒットチェック
}
static void GameTimeUp(void) { // ゲームがタイムアップする
    if (appPhase == 0) {
        VDP_drawText("TIME  UP",11,12);
        gameCount = 180; // カウントの設定
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++; // 状態の更新
    }
    // カウントの更新
    gameCount--;
    if (gameCount) return;
    // 更新
    // 状態の更新
    appState = (appHiscore > appScore) ? GAME_STATE_OVER : GAME_STATE_HISCORE;
    appPhase = APP_PHASE_NULL;
}
static void GameOver(void) { // ゲームオーバーになる
    if (appPhase == 0) {// 初期化
        VDP_drawText("GAME  OVER",10,12);
        gameCount = 180;// カウントの設定
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++;// 状態の更新
    }
    // カウントの更新
    gameCount--;
    if (gameCount==0) {
        // メッセージのアンロード
        VDP_drawText("          ",10,12);
        // 状態の更新
        appState = GAME_STATE_UNLOAD;
        appPhase = APP_PHASE_NULL;
    }
}
static void GameHiscore(void) { // ハイスコアを更新する
    if (appPhase==0) {// 初期化
        VDP_drawText("HI SCORE",11,12);
        // フラグの設定
        gameFlag &= ~((1<<GAME_FLAG_PLAYABLE)|(1<<GAME_FLAG_PAUSE)|(1<<GAME_FLAG_STATUS));
        appPhase++;// 状態の更新
    }
    // ハイスコアの処理
    if (++appPhase < 60) return;
    VDP_drawText("          ",10,12);
    appState = GAME_STATE_UNLOAD;// 状態の更新
    appPhase = APP_PHASE_NULL;
}
static void GameUnload(void) { // ゲームをアンロードする
    if (appPhase == 0) {// 初期化
        gameFlag = 0;   // フラグの設定
        appPhase++;     // 状態の更新
    }
    appState = GAME_STATE_END; // 状態の更新
}
static void GameEnd(void) { // ゲームを終了する
    appMode = APP_MODE_TITLE; // モードの更新
    appState = 0; // 状態の更新
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
            score_add();
            GameShootBack(ix);// 敵の撃ち返し
            shot->state = SHOT_STATE_NULL;// ショットの状態の更新
        }
    }
}
static void GameCheckShipBullet(void) { // 自機と弾のヒットチェックを行う
    if (ship.nodamage) return;// 自機の存在
    for (BULLET* ix = bullets; ix<&bullets[BULLET_SIZE]; ix++) {// 弾の走査
        if (ix->state == BULLET_STATE_NULL) continue;// 弾の存在
        // ヒットチェック
        s16 a = ix->x-ship.x;
        if (a < 0) a = -a;
        if (a >= FIX16(4)) continue;
        a = ix->y - ship.y;
        if (a < 0) a = -a;
        if (a >= FIX16(4)) continue;
        ix->state=BULLET_STATE_NULL;// 弾の状態の更新
        ix->next = free_bullet;free_bullet=ix;
        ship.state = SHIP_STATE_BOMB;// 自機の状態の更新
        ship.phase = APP_PHASE_NULL;
    }
}
static void GameCheckShipEnemy(void) { // 自機と敵のヒットチェックを行う
    if (ship.nodamage)return;// 自機の存在
    for (ENEMY* ix = enemies;ix<&enemies[ENEMIES_SIZE];ix++) {// 敵の走査
        if (ix->nodamage) continue;// 敵の存在
        // ヒットチェック X
        s16 a = ix->x-ship.x;
        if (a < 0) a = -a;
        if (a >= FIX16(6)) continue;
        // ヒットチェック Y
        a = ix->y-ship.y;
        if (a < 0) a = -a;
        if (a >= FIX16(6)) continue;
        score_add();
        // 更新
        ix->nodamage = 0x80; // 敵のノーダメージの更新
        ix->state = ENEMY_STATE_BOMB;// 敵の状態の更新
        ix->phase = APP_PHASE_NULL;
        ship.state = SHIP_STATE_BOMB;// 自機の状態の更新
        ship.phase = APP_PHASE_NULL;
    }
}
static void GameShootBack(ENEMY* ix) { // 敵が弾を打ち返す
    // 敵が自機に近い場合は撃ち返さない
    if (ship.y-FIX16(0x20) < ix->y) {
        s16 a = ship.x-FIX16(0x18);
        if (a < ix->x && ix->x <= a+FIX16(0x30)) return;
    }
    u16 angle = atan2Fix16(ship.x - ix->x,ship.y - ix->y);
 
    static u8 const backTypeTable[] = {0x10, 0x11, 0x11, 0x10, 0x10}; // 撃ち返しデータ
    static s8 const backAngleTable[] = {0, 48, -48, 96, -96};
    static u8 const backSpeedTable[] = {
        //0x03, 0x00, 0x02, 0x01, 0x02, 0x01, 0x03, 0x00, 0x03, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x02, 0x01, 0x02, 0x00, 0x02, 0x00, 0x02, 0x01, 0x02, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x02, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
        0x01, 0x01, 0x01, 0x00, 0x01, 0x00, 0x01, 0x01, 0x01, 0x01, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
    };
    // 弾の位置の設定
    for(u8 b = 5,c = 0;b;c++,--b) {// 弾のエントリ
        // メインの処理
        BULLET* iy=BulletEntry(backTypeTable[c]);
        if (iy==NULL) break;
        iy->x = ix->x;
        iy->y = ix->y;
        u16 a = angle + backAngleTable[c];
        s16 x = cosFix16(a);
        s16 y = sinFix16(a);
        iy->spx = 0;
        iy->spy = 0;
        u8* hl = (void*)(backSpeedTable+c*2+16*(appTimer>>4*3));
        for (u8 d = *hl++;d;d--) {
            iy->spx += x;
            iy->spy += y;
        }
        u8 e = *hl;
        if(e) {
            x = x>>1;
            y = y>>1;
            for(;e;e--) {
                iy->spx += x;
                iy->spy += y;
            }
        }
    }
}
