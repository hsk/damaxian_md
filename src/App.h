// App.h : アプリケーション

// マクロの定義
// モード
#define APP_MODE_INIT       0
#define APP_MODE_LOAD       1
#define APP_MODE_GAME       2
// 状態
#define APP_STATE_NULL      0
#define APP_PHASE_NULL      0
// 外部関数宣言
void AppInitialize(void);
void AppUpdate(void);
// 外部変数宣言
extern u8 appMode;        // モード
extern u8 appState;       // 状態
extern u8 appPhase;
