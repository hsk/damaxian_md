// Enemy.c : ゲーム画面／敵
#include "bios.h"
#include "Math.h"
#include "App.h"
#include "Game.h"
#include "Ship.h"
#include "Enemy.h"
// 定数の定義
// 敵データ
extern u8 enemyPointTable[];
static u8 const enemyStayTable[] = {
    0x7f, 0x00,
    0x7f, 0x40,
    0xff, 0x00,
    0xff, 0x00,
};
static u8 const enemySpritePatternTable[] = {
    0x00,
    0x00,
    0x08,
    0x18,
    0x18,
    0x18,
    0x08,
    0x10,
    0x18,
    0x18,
    0x10,
};
// 変数の定義
ENEMY enemies[ENEMIES_SIZE];   // パラメータ
void EnemyInitialize(void) { // 敵を初期化する
    // 敵の走査
    ENEMY* ix = enemies;
    for (u8 c=0;c<ENEMIES_SIZE;ix++,c++) {
        ix->ss = *(s16*)&enemyPointTable[c<<1];// 位置の設定
        // スプライトの設定
        ix->frame = enemySpritePatternTable[c]+32;
        ix->state = ENEMY_STATE_IN;// 状態の設定
        ix->phase = 0;
    }
}
static void EnemyNull(ENEMY* ix);
static void EnemyIn(ENEMY* ix);
static void EnemyStay(ENEMY* ix);
static void EnemyTurn(ENEMY* ix);
static void EnemyApproach(ENEMY* ix);
static void EnemyBomb(ENEMY* ix);
static void EnemyDraw(ENEMY* ix);
void EnemyUpdate(void) { // 敵を更新する
    ENEMY* ix = enemies;// 敵の走査
    for (u8 b = ENEMIES_SIZE;b;ix++,--b) {
        u8 a = ix->state;
        if      (a == ENEMY_STATE_NULL)     EnemyNull(ix);     // なし
        else if (a == ENEMY_STATE_IN)       EnemyIn(ix);       // イン
        else if (a == ENEMY_STATE_STAY)     EnemyStay(ix);     // 待機
        else if (a == ENEMY_STATE_TURN)     EnemyTurn(ix);     // ターン
        else if (a == ENEMY_STATE_APPROACH) EnemyApproach(ix); // アプローチ
        else                                EnemyBomb(ix);     // 爆発
    }
}
static void EnemyNull(ENEMY* ix) { // 敵はなし
    if (ix->phase == 0) {// 初期化
        ix->nodamage = 0x80;// ノーダメージの設定
        ix->phase++;// 状態の更新
    }// 待機の処理
}
static void EnemyIn(ENEMY* ix) { // 敵がインする
    if (ix->phase==0) {// 初期化
        // 位置の設定
        ix->x = intToFix16(ix->xs);
        ix->y = FIX16(-12);
        ix->angle = 0x40;// 方向の設定
        ix->nodamage = 0;// ノーダメージの設定
        ix->count0 = 0x40 - ix->ys;// カウントの設定
        ix->animation = 0;// アニメーションの設定
        ix->phase++;// 状態の更新
    } // インの処理
    // カウントの更新
    if (ix->count0 == 0) ix->count0 -= 2; // 位置の更新
    else if (ix->y != intToFix16(ix->ys)) ix->y += FIX16(2); // 方向の更新
    else if (ix->angle != 0xc0) ix->angle -= 8;
    else {
        ix->state = ENEMY_STATE_STAY;// 状態の設定
        ix->phase = 0;
    } // 処理の完了
    EnemyDraw(ix); // 敵の描画
}
static void EnemyStay(ENEMY* ix) { // 敵が待機する
    if (ix->phase==0) {
        u8* hl = (void*)&enemyStayTable[(appTimer>>(4*3))<<1];
        u8 a = random() & (*hl++);
        a += *hl;
        ix->count0 = a + 0x40;
        ix->phase++;// 状態の更新
    } // 待機の処理
    if (gameFlag & (1<<GAME_FLAG_PLAYABLE)) {// 操作可能かどうか
        if(ix->count0)      ix->count0--;// カウント0更新
        else {
            ix->state = ENEMY_STATE_TURN;// 状態の設定
            ix->phase = 0;
        }
    }
    EnemyDraw(ix);// 敵の描画
}
static void EnemyTurn(ENEMY* ix) { // 敵がターンする
    if (ix->phase == 0) {
        ix->turn = 0x02;// 回転の設定
        if ((random()&0b00010000) == 0) ix->turn = 0xfe;
        ix->phase++;// 状態の更新
    }
    ix->angle += ix->turn;// 方向の更新
    if (ix->angle==0x40) ix->state = ENEMY_STATE_APPROACH;// 状態の更新
    // 位置の更新
    s16 hl = cosFix16(((u16)ix->angle)*4);
    ix->x += hl + (hl>>1);// X 方向は x1.5 の移動
    ix->y += sinFix16(((u16)ix->angle)*4)<<1;// Y 方向は x2.0 の移動
    EnemyDraw(ix);
}
static void EnemyApproach(ENEMY* ix) { // 敵がアプローチする
    // 方向の更新
    u8 a = fix16ToInt(ix->x);
    u8 b = fix16ToInt(ship.x);
    if (a >= 0xe0) a = 0xfe;
    else if (a == b) a = 0;
    else if (a < b) a = 0xfe;
    else a = 0x02;
    a += ix->angle;
    if (a < 0x22) a = 0x22;
    else if (a > 0x5e) a = 0x5e;
    ix->angle = a;
    
    ix->x += cosFix16(((u16)ix->angle)*4) << 1;// 位置の更新
    ix->y += sinFix16(((u16)ix->angle)*4) << 1;
    
    if (ix->y >= FIX16(224+8)) {
        ix->state = ENEMY_STATE_IN;
        ix->phase = 0;
    }
    EnemyDraw(ix);// 敵の描画
}
static void EnemyBomb(ENEMY* ix) { // 敵が爆発する
    if (ix->phase==0) {// 初期化処理
        ix->nodamage = 0x80;// ノーダメージの設定
        ix->animation = 0;// アニメーションの設定
        ix->phase++;// 状態の更新
    }
    ix->animation++;// アニメーションの更新
    if (ix->animation == 0x1f) { // 状態の更新
        ix->state = ENEMY_STATE_IN;
        ix->phase = 0;
    }
    SystemSetSprite(4 + ((ix->animation>>3)&3),ix->x,ix->y);
}

static void EnemyDraw(ENEMY* ix) { // 敵を描画する
    ix->animation++;// アニメーションの更新
    // 描画の開始
    SystemSetSprite(ix->frame+(((ix->angle + 0x20) & 0b11000000) >> 5)
                             +((ix->animation & 0b00001000) >> 3),ix->x,ix->y);
}
