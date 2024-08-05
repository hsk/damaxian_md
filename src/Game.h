// Game.h : ゲーム画面
// マクロの定義
// 状態
#define GAME_STATE_INITIALIZE   0
#define GAME_STATE_LOAD         1
#define GAME_STATE_START        2
#define GAME_STATE_PLAY         3
// フラグ
#define GAME_FLAG_PLAYABLE      0
#define GAME_FLAG_PAUSE         1
#define GAME_FLAG_STATUS        2
// 外部関数宣言
void GameUpdate(void);
// 外部変数宣言
extern u8 gameFlag;
