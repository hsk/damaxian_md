#include "bios.h"
#include "App.h"
#include "Sound.h"
extern u16 m;
int main() {
    SystemInitialize();
    SystemInitializeSound();// サウンドの初期化
    SystemTransferSprite();
    AppInitialize();
    SystemTransferSprite();
    m=9999;
    while (TRUE) {
        AppUpdate();
        SystemTransferSprite();
    }
    return 0;
}
