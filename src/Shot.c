// Shot.c : ゲーム画面／ショット
#include "bios.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Sound.h"
// 変数の定義
SHOT shots[SHOT_SIZE]; // パラメータ
void ShotInitialize(void) {// ショットを初期化する
    for (SHOT* shot = shots;shot<&shots[SHOT_SIZE];shot++)// ショットの走査
        shot->state = SHOT_STATE_NULL;// 状態の設定
}
static void ShotMove(SHOT* shot);
void ShotUpdate(void) {// ショットを更新する
    for(SHOT* shot = shots;shot<&shots[SHOT_SIZE];shot++)// ショットの走査
        if (shot->state == SHOT_STATE_MOVE) ShotMove(shot); // 移動
}
static void ShotMove(SHOT* shot) { // ショットが移動する
    if (shot->phase == 0) {// 初期化
        shot->x = ship.x;// 位置の初期化
        shot->y = ship.y;
        soundRequest[0] = (void*)mmlShotChannel0;// 演奏の開始
        shot->phase++;// 状態の更新
    }
    s16 a = shot->y = shot->y-FIX16(4);// 移動
    if (a <= -8) shot->state = SHOT_STATE_NULL; // 状態の更新
    SystemSetSprite(2,shot->x,shot->y);
}
void ShotEntry(void) { // ショットをエントリする
    for(SHOT* shot = shots;shot<&shots[SHOT_SIZE];shot++) {// ショットの走査
        if (shot->state != SHOT_STATE_NULL) continue;// 状態の更新
        shot->state = SHOT_STATE_MOVE;
        shot->phase = 0;
        break;
    }
}
