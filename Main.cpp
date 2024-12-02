#include "KeyboardHook.h"
#include <iostream>

#include "Logger.h"


int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR, int) {
    LogConfig::ENABLE_DEBUG = false;
    LogConfig::ENABLE_CONSOLE = false;
    LogConfig::ENABLE_FILE = true;
#ifdef DEBUG
        LogConfig::ENABLE_DEBUG = true;
        LogConfig::ENABLE_CONSOLE = true;
        LogConfig::ENABLE_FILE = false;
#endif

    if (auto keyboardHook = KeyboardHook(); !keyboardHook.Install(hInstance)) {
        return 1;
    }
    Logger::getInstance().log(INFO_LEVEL, APPLICATION, "Keyboard hook installed");


    // Message loop to keep the hook
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
