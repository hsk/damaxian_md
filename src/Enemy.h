// Enemy.h : ゲーム画面／敵
// 敵
    #define ENEMIES_SIZE           11
// 状態
    #define ENEMY_STATE_NULL       0
    #define ENEMY_STATE_IN         1
    #define ENEMY_STATE_STAY       2
    #define ENEMY_STATE_TURN       3
    #define ENEMY_STATE_APPROACH   4
    #define ENEMY_STATE_BOMB       5
typedef struct {
    u8 state;
    u8 phase;
    s16 x;
    s16 y;
    union {
        struct {
            u8 xs;
            u8 ys;
        };
        s16 ss;
    };
    u8 angle;
    u8 turn;
    u8 nodamage;
    u8 count0;
    u8 animation;
    u8 frame;
} ENEMY;
// 外部関数宣言
    void EnemyInitialize(void);
    void EnemyUpdate(void);
// 外部変数宣言
    extern ENEMY enemies[ENEMIES_SIZE];
