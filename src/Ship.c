// Ship.c : ゲーム画面／自機
#include "bios.h"
#include "Game.h"
#include "Ship.h"
#include "Shot.h"
#include "Sound.h"
// 変数の定義
SHIP ship; // パラメータ
void ShipInitialize(void) { // 自機を初期化する
    ship.state = SHIP_STATE_PLAY; // 状態の設定
    ship.phase = 0;
    ship.nodamage = 0x80;// ノーダメージの設定
}
static void ShipNull(void);
static void ShipPlay(void);
static void ShipBomb(void);
void ShipUpdate(void) { // 自機を更新する
    u8 a = ship.state;
    if      (a == SHIP_STATE_NULL) ShipNull(); // 自機はなし
    else if (a == SHIP_STATE_PLAY) ShipPlay(); // 操作
    else                           ShipBomb(); // 爆発
}
static void ShipNull(void) { // 自機はなし
}
static void ShipPlay(void) { // 自機を操作する
    if (ship.phase==0) {// 初期化
        ship.x = FIX16(0x60);// 位置の設定
        ship.y = FIX16(224+8);
        ship.nodamage = 0x80;// ノーダメージの設定
        ship.phase++;// 状態の更新
    }// 待機処理
    if (ship.nodamage) ship.nodamage--; // ノーダメージの更新
    // 自機を定位置に移動
    if (ship.y >= FIX16(0xc1)) {
        ship.y-=FIX16(1);
    } else if (gameFlag&(1<<GAME_FLAG_PLAYABLE)) {// 操作可能かどうか
        // Ｘ方向の移動
        if (input&BUTTON_LEFT) // ←が押された
            if (ship.x>=FIX16(0x8+2)) ship.x-=FIX16(2);
        if(input&BUTTON_RIGHT) // →が押された
            if (ship.x<FIX16(0xb8)) ship.x+=FIX16(2);
        if (ship.phase==1) {
            // 発射処理
            if(input&BUTTON_A) {// A ボタンが押された
                ShotEntry();// ショットのエントリ
                ship.phase = 6;
            }
        } else if(!(input&BUTTON_A)) {
            ship.phase=1;
        } else {
            ship.phase--;
        }
    }
    // 描画の開始
    SystemSetSprite((ship.nodamage & 2)>>1, ship.x,ship.y);
}
static void ShipBomb(void) { // 自機が爆発する
    if(ship.phase==0) {//初期化
        ship.nodamage = 0x80;// ノーダメージの設定
        ship.animation = 0;// アニメーションの設定
        soundRequest[0] = (void*)mmlBombChannel0; // 演奏の開始
        ship.phase++;// 状態の更新
    }
    // 爆発の処理
    // アニメーションの更新
    ship.animation++;
    if (ship.animation==0x1f) {
        ship.state = SHIP_STATE_PLAY; // 状態の更新
        ship.phase = 0;
    }
    // 描画の開始
    SystemSetSprite(4+((ship.animation >> 3)&3),ship.x,ship.y);
}
