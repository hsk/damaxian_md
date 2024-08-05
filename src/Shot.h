// Shot.h : ゲーム画面／ショット

// ショット
#define SHOT_SIZE          8
// 状態
#define SHOT_STATE_NULL    0
#define SHOT_STATE_MOVE    1

typedef struct {
    u8 state;
    u8 phase;
    u16 x;
    u16 y;
} SHOT;
// 外部関数宣言
    void ShotInitialize(void);
    void ShotUpdate(void);
    void ShotEntry(void);
// 外部変数宣言
    extern SHOT shots[SHOT_SIZE];
