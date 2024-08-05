// Ship.h : ゲーム画面／自機

// 状態
#define SHIP_STATE_NULL    0
#define SHIP_STATE_PLAY    1

typedef struct {
    u8 state;
    u8 phase;
    u16 x;
    u16 y;
    u8 animation;
} SHIP;
// 外部関数宣言
    void ShipInitialize(void);
    void ShipUpdate(void);
// 外部変数宣言
    extern SHIP ship;
