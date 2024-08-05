// Bullet.h : ゲーム画面／弾
// マクロの定義
    // 弾
        #define BULLET_SIZE            (5*3+3)
    // 状態
        #define BULLET_STATE_NULL      0
        #define BULLET_STATE_MOVE      1
typedef struct BULLET {
    u8 state;
    s16 x;
    s16 y;
    s16 spx;
    s16 spy;
    u8 frame;
    struct BULLET* next;
} BULLET;
// 外部関数宣言
    void BulletInitialize(void);
    void BulletUpdate(void);
    BULLET* BulletEntry(u8 frame);
// 外部変数宣言
    extern BULLET bullets[BULLET_SIZE];
    extern BULLET* free_bullet;
