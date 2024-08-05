// Bullet.c : ゲーム画面／弾
#include "bios.h"
#include "App.h"
#include "Game.h"
#include "Bullet.h"
// 変数の定義
BULLET bullets[BULLET_SIZE];    // パラメータ
BULLET* free_bullet;
void BulletInitialize(void) { // 弾を初期化する
    free_bullet = NULL;
    for (BULLET* ix = bullets; ix<&bullets[BULLET_SIZE]; ix++) {// 弾の走査
        ix->state = BULLET_STATE_NULL;// 状態の設定
        ix->next = free_bullet;free_bullet = ix;
    }
}
static void BulletNull(BULLET* ix);
static void BulletMove(BULLET* ix);
void BulletUpdate(void) { // 弾を更新する
    for (BULLET* ix = bullets; ix<&bullets[BULLET_SIZE]; ix++) {// 弾の走査
        if (ix->state == BULLET_STATE_NULL)BulletNull(ix);// なし
        else BulletMove(ix); // 移動
    } // 更新の終了
}
static void BulletNull(BULLET* ix) { // 弾はなし
}
static void BulletMove(BULLET* ix) { // 弾が移動する
    ix->x += ix->spx;// 移動
    ix->y += ix->spy;
    if (ix->x < FIX16(-8) || ix->x > FIX16(0xd4) || ix->y > FIX16(228)) {
        ix->state = BULLET_STATE_NULL;// 状態の更新
        ix->next = free_bullet;free_bullet = ix;
    }
    SystemSetSprite(ix->frame, ix->x, ix->y);
}
BULLET* BulletEntry(u8 frame) {// 弾をエントリする
    // 弾の走査
    if (!free_bullet) return NULL;
    BULLET* ix = free_bullet;
    ix->frame = frame;// 状態の更新
    ix->state = BULLET_STATE_MOVE;// 状態の更新
    free_bullet = ix->next;
    return ix;
}
