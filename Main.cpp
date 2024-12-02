#include "KeyboardHook.h"
#include <iostream>

#include "Logger.h"


int WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int) {
    LogConfig::ENABLE_DEBUG = true;
    LogConfig::ENABLE_CONSOLE = true;
    LogConfig::ENABLE_FILE = false;

    auto keyboardHook = KeyboardHook();

    if (!keyboardHook.Install(hInstance)) {
        return 1;
    }


    // Message loop to keep the hook
    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
