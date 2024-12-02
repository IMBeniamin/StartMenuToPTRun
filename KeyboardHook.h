#ifndef KEYBOARDHOOK_H
#define KEYBOARDHOOK_H
#include <atomic>
#include <Windows.h>

class KeyboardHook {
public:
    KeyboardHook();

    ~KeyboardHook();

    bool Install(HINSTANCE hinstance);

    void Uninstall();

private:
    HHOOK hHook;
    // atomic used for thread safety
    std::atomic<bool> windowsKeyPressed;
    std::atomic<bool> otherKeyPressed;

    static LRESULT CALLBACK HookCallback(int nCode, WPARAM wParam, LPARAM lParam);

    static KeyboardHook *instance;

    int HandleKeyEvent(WPARAM wParam, const KBDLLHOOKSTRUCT *pKeyboard);

    static void SimulateKeyPress(BYTE key);

    static void TriggerAltSpace();
};

#endif //KEYBOARDHOOK_H
