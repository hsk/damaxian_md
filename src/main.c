#include "bios.h"
#include "App.h"
extern u16 m;
int main() {
    SystemInitialize();
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
