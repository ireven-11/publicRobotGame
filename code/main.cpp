#include"DxLibForIreven.h"
#include"Game.h"

int WINAPI WinMain(winmainSetting)
{
    InitDxLib();

    Game::instance().update();

    DxLib_End();

    return 0;
}