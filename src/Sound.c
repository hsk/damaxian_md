// Sound.c : サウンドライブラリ
#include "bios.h"
// 定数の定義
static u16 const soundToneFrequencyTable[] = {
    0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x0000,0x0000,
    0x03f9,0x03c0,0x038a,0x0357,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x03ff,0x0000,0x0000,
    0x01fc,0x01e0,0x01c5,0x01ac,0x0357,0x0327,0x02fa,0x02cf,0x02a7,0x0281,0x0281,0x025d,0x023b,0x021b,0x0000,0x0000,
    0x00ff,0x00f0,0x00e3,0x00d6,0x01ac,0x0193,0x017d,0x0167,0x0154,0x0140,0x0140,0x012e,0x011d,0x010e,0x0000,0x0000,
    0x007f,0x0078,0x0071,0x006b,0x00d6,0x00ca,0x00be,0x00b4,0x00a9,0x00a0,0x00a0,0x0097,0x008f,0x0086,0x0000,0x0000,
    0x0040,0x003c,0x0039,0x0036,0x006b,0x0065,0x005f,0x005a,0x0055,0x0050,0x0050,0x004c,0x0048,0x0044,0x0000,0x0000,
    0x0020,0x001e,0x001d,0x001b,0x0036,0x0032,0x0030,0x002d,0x002a,0x0028,0x0028,0x0026,0x0024,0x0022,0x0000,0x0000,
    0x0010,0x000f,0x000e,0x000d,0x001b,0x0019,0x0018,0x0016,0x0015,0x0014,0x0014,0x0013,0x0012,0x0011,0x0000,0x0000,
};
static u8 const soundLengthTable[] = {
    1,       // T1 L0 32
    2,       // T1 L1 16
    3,       // T1 L2 16.
    4,       // T1 L3  8
    6,       // T1 L4  8.
    8,       // T1 L5  4
    12,      // T1 L6  4.
    16,      // T1 L7  2
    20,      // T1 L8  2.
    24,      // T1 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    2,       // T2 L0 32
    4,       // T2 L1 16
    6,       // T2 L2 16.
    8,       // T2 L3  8
    12,      // T2 L4  8.
    16,      // T2 L5  4
    24,      // T2 L6  4.
    32,      // T2 L7  2
    48,      // T2 L8  2.
    64,      // T2 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    3,       // T3 L0 32
    6,       // T3 L1 16
    9,       // T3 L2 16.
    12,      // T3 L3  8
    18,      // T3 L4  8.
    24,      // T3 L5  4
    36,      // T3 L6  4.
    48,      // T3 L7  2
    72,      // T3 L8  2.
    96,      // T3 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    4,       // T4 L0 32
    8,       // T4 L1 16
    12,      // T4 L2 16.
    16,      // T4 L3  8
    24,      // T4 L4  8.
    32,      // T4 L5  4
    48,      // T4 L6  4.
    64,      // T4 L7  2
    96,      // T4 L8  2.
    128,     // T4 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    5,       // T5 L0 32
    10,      // T5 L1 16
    15,      // T5 L2 16.
    20,      // T5 L3  8
    30,      // T5 L4  8.
    40,      // T5 L5  4
    60,      // T5 L6  4.
    80,      // T5 L7  2
    120,     // T5 L8  2.
    160,     // T5 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    6,       // T6 L0 32
    12,      // T6 L1 16
    18,      // T6 L2 16.
    24,      // T6 L3  8
    32,      // T6 L4  8.
    48,      // T6 L5  4
    72,      // T6 L6  4.
    96,      // T6 L7  2
    144,     // T6 L8  2.
    192,     // T6 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    7,       // T7 L0 32
    14,      // T7 L1 16
    21,      // T7 L2 16.
    28,      // T7 L3  8
    42,      // T7 L4  8.
    56,      // T7 L5  4
    84,      // T7 L6  4.
    112,     // T7 L7  2
    168,     // T7 L8  2.
    224,     // T7 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    8,       // T8 L0 32
    16,      // T8 L1 16
    24,      // T8 L2 16.
    32,      // T8 L3  8
    48,      // T8 L4  8.
    64,      // T8 L5  4
    96,      // T8 L6  4.
    128,     // T8 L7  2
    192,     // T8 L8  2.
    0,       // T8 L9  1
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
    1,       //
};
// 変数の定義
u8* soundRequest[4];    // サウンド
u8* soundHead[4];
u8* soundPlay[4];
static u8 soundT[4];
static u8 soundV[4];
static u8 soundO[4];
static u8 soundL[4];
static u16 soundFrequency[4];
static u8 soundRest[4];
static u8 soundUpdate[4];
static void SystemClearSoundChannel(u8 a);
void SystemInitializeSound(void) { // サウンドを初期化する
    // 1. サウンドデータの初期化
        soundRequest[0] = 0;
        soundRequest[1] = 0;
        soundRequest[2] = 0;
        soundRequest[3] = 0;
        soundHead[0] = 0;
        soundHead[1] = 0;
        soundHead[2] = 0;
        soundHead[3] = 0;
        soundPlay[0] = 0;
        soundPlay[1] = 0;
        soundPlay[2] = 0;
        soundPlay[3] = 0;
    // 2. サウンドパラメータの初期化
        for(u8 i=0;i<4;i++) SystemClearSoundChannel(i);
}
static void SystemUpdateSoundChannel(void) { // サウンドを更新する
    u8 a;
    // チャンネルの走査
    for(u16 bc = 0;bc<3;bc++){ // １チャンネルの処理
        if (soundRequest[bc]) {// リクエスト
            soundHead[bc] = soundRequest[bc];
            soundPlay[bc] = soundRequest[bc];
            soundRequest[bc] = 0;
            SystemClearSoundChannel(bc);
        }
        if (soundPlay[bc]==0) continue;// サウンドデータの存在
        if(--soundRest[bc])continue;// 待機
        u8* hl = soundPlay[bc];// 再生ポインタの取得
        for(;;) { // MML の解析
            switch (a=*hl++) {
            case 0x00: // 0x00 : 終端コード
                soundHead[bc]=0;
                soundPlay[bc]=0;
                soundFrequency[bc]=0;
                soundUpdate[bc]=1;
                goto SystemUpdateSoundNext;
            case 0xff: // $ff : 繰り返し
                hl=soundPlay[bc]=soundHead[bc];
                continue;
            case 'T': // 'T' : テンポ（T1 ～ T8）
                soundT[bc] = *hl++ - '1';
                continue;
            case 'V': // 'V' : 音量（V0 ～ V16）
                soundV[bc] = *hl++ - '0';
                a = *hl;
                if (a < '0' || '9' < a) continue;
                soundV[bc] = 10 + a - '0';
                hl++;
                continue;
            case 'O': // 'O' : オクターブ（O1 ～ O8）
                soundO[bc] = *hl++ - '1';
                continue;
            case 'L': // 'L' : 音の長さ（L0 ～ L9）
                soundL[bc] = *hl++ - '0';
                continue;
            case 'R': // 'R' : 休符
                soundFrequency[bc]=0;
                break;
            default: // 'A' : 音符
                {
                    u8 ix = (soundO[bc]*16)+((a-'A')*2);
                    if (*hl=='#') {
                        hl++;
                        ix++;
                    }
                    soundFrequency[bc]=soundToneFrequencyTable[ix];
                }
                break;
            }
            // 音の長さの設定
            a = *hl;
            if ('0' <= a && a <= '9') {
                a -= '0';
                hl++;
            } else {// else
                a = soundL[bc];
            }
            soundRest[bc]=soundLengthTable[(soundT[bc]<<4)+a];
            soundUpdate[bc]=0x1;
            soundPlay[bc]=hl;// 再生ポインタの保存
            break;// チャンネルの走査の完了
        }
    SystemUpdateSoundNext: // 次のチャンネルへ
    }
}
static void SetSoundChannel(u8 a) {// チャンネルの設定
    if (soundUpdate[a]) {
        if (soundFrequency[a]) {
            PSG_setEnvelope(a,16-soundV[a]);
            PSG_setTone(a,soundFrequency[a]);
        } else {
            PSG_setEnvelope(a,15);
            if(soundPlay[a]==0) soundUpdate[a]=0;
        }
    }
}
void SystemUpdateSound(void) { // サウンドを更新する
    SystemUpdateSoundChannel();
    SetSoundChannel(0);
    SetSoundChannel(1);
    SetSoundChannel(2);
}
static void SystemClearSoundChannel(u8 a) { // サウンドのチャンネルをクリアする
    soundT[a] = 0;
    soundV[a] = 0x0f;
    soundO[a] = 0x03;
    soundL[a] = 0x05;
    soundFrequency[a]=0;
    soundRest[a]=0x01;
    soundUpdate[a]=0;
}
// MML データ
u8 const mmlNull[] = "";
u8 const mmlTitleStartChannel0[] = "T2V16L1" "O5DEADEAB2" "R4";
u8 const mmlTitleStartChannel1[] = "T2V16L1" "O4AO5DEO4AO5DEF#2" "R4";
u8 const mmlTitleStartChannel2[] = "T2V16L1" "O4EAO5DO4EAO5DE2" "R4";
u8 const mmlStartChannel0[] = "T1S0V16" "L0O6CO5BAGFEDCO4BAG"
    "L3O3GO4CO3GO4CECEGEGO5C5R5" "L3O3GO4CO3GO4CECEGEGO5C5R5"
    "L1O4E3DD#DD#DD#DD#DD#DD#DD#DD#"
    "L0O4CDEFGABO5C" "L0O4CDEFGABO5C" "L0O4CDEFGABO5C" "R5";
u8 const mmlStartChannel1[] = "T1V16" "L0RRRRRRRRRRR"
    "L5RRRRRRR" "L5RRRRRRR" "L3RRRRRRRRR"
    "L0RRRRRRRR" "L0RRRRRRRR" "L0RRRRRRRR" "R5";
u8 const mmlStartChannel2[] = "T1V16" "L0RRRRRRRRRRR"
    "L5RRRRRRR" "L5RRRRRRR" "L3RRRRRRRRR"
    "L0RRRRRRRR" "L0RRRRRRRR" "L0RRRRRRRR" "R5";
u8 const mmlHiScoreChannel0[] = "T2V16L3" "O5D5" "RDD#""FG6" "R9";
u8 const mmlHiScoreChannel1[] = "T2V16L3" "O4A5" "RAA#O5CD6" "R9";
u8 const mmlHiScoreChannel2[] = "T2V16L3" "O4F#5""RF#G""AB6" "R9";
u8 const mmlShotChannel0[] = "T1V15L0" "O5C#CC#RCO4BA#AG#GFD#C#O3BG#";
u8 const mmlBombChannel0[] = "T1V15L0"
    "O4CO3BAGABO4CDEFGG#R1GFEDCDFGABO5CC#R1CO4BAGAO5CDEGG#R1GFEDCDFGABO6CDO5A#";
u8 const mmlEnemyTurnChannel1[] = "T1V15L1"
    "O6CO5BA#AG#GF#FED#DDC#CO4BA#AAGF#FED#DC#CO3BA#AG#GF#FED#DC#CO2BA#A";
u8 const mmlEnemyBombChannel2[] = "T1V15L0"
    "O4AGFEDCAGFEDCCDEFGABO5CDEFGABO6CO5AFEDEF";
