#include "KeyboardHook.h"
#include <thread>
#include <chrono>
#include <iostream>

#include "Logger.h"

KeyboardHook *KeyboardHook::instance = nullptr;

KeyboardHook::KeyboardHook()
    : hHook(nullptr), windowsKeyPressed(false), otherKeyPressed(false) {
}

KeyboardHook::~KeyboardHook() {
    Uninstall();
}

bool KeyboardHook::Install(HINSTANCE hinstance) {
    if (hHook) return false; // Hook already installed
    instance = this;
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, HookCallback, hinstance, 0);
    if (!hHook) {
        Logger::getInstance().log(CRITICAL_LEVEL, EVENTS, "Failed to install keyboard hook");
        const auto errorMessage = GetLastError();
        Logger::getInstance().log(CRITICAL_LEVEL, EVENTS, std::to_string(errorMessage));
    }
    return hHook != nullptr;
}

void KeyboardHook::Uninstall() {
    if (hHook) {
        UnhookWindowsHookEx(hHook);
        hHook = nullptr;
    }
}

LRESULT CALLBACK KeyboardHook::HookCallback(const int nCode, const WPARAM wParam, const LPARAM lParam) {
    if (nCode == HC_ACTION && instance) {
        const auto *pKeyboard = reinterpret_cast<KBDLLHOOKSTRUCT *>(lParam);
        if (const auto result = instance->HandleKeyEvent(wParam, pKeyboard)) return result;
    }
    return CallNextHookEx(nullptr, nCode, wParam, lParam);
}

int KeyboardHook::HandleKeyEvent(const WPARAM wParam, const KBDLLHOOKSTRUCT *pKeyboard) {
    if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
        if (pKeyboard->vkCode == VK_LWIN) {
            windowsKeyPressed = true;
            otherKeyPressed = false;
            return 1;
        }
        if (windowsKeyPressed && pKeyboard->vkCode != VK_LWIN) {
            otherKeyPressed = true;
            Logger::getInstance().log(DEBUG_LEVEL, EVENTS, "Windows key + other key pressed");
            keybd_event(VK_LWIN, 0, 0, 0); // Simulate Windows key press
            keybd_event(static_cast<BYTE>(pKeyboard->vkCode), 0, 0, 0); // Simulate the other key press
            keybd_event(static_cast<BYTE>(pKeyboard->vkCode), 0, KEYEVENTF_KEYUP, 0); // Release the other key
            keybd_event(VK_LWIN, 0, KEYEVENTF_KEYUP, 0); // Release the Windows key
            return 1;
        }
    }
    if (wParam == WM_KEYUP && pKeyboard->vkCode == VK_LWIN) {
        if (!otherKeyPressed) {
            std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
                TriggerAltSpace();
            }).detach();
        }
        windowsKeyPressed = false;
        return 1;
    }
    return NULL;
}

void KeyboardHook::SimulateKeyPress(const BYTE key) {
    INPUT inputs[2] = {};
    inputs[0].type = inputs[1].type = INPUT_KEYBOARD;
    inputs[0].ki.wVk = inputs[1].ki.wVk = key;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    if (const auto _sentInputs = SendInput(2, inputs, sizeof(INPUT)); _sentInputs != 2) {
        Logger::getInstance().log(CRITICAL_LEVEL, EVENTS, "Failed to simulate key press");
        const auto errorMessage = GetLastError();
        Logger::getInstance().log(CRITICAL_LEVEL, EVENTS, std::to_string(errorMessage));
    }
}

void KeyboardHook::TriggerAltSpace() {
    Logger::getInstance().log(DEBUG_LEVEL, EVENTS, "Alt+Space triggered");
    keybd_event(VK_MENU, 0, 0, 0); // Simulate Alt press
    keybd_event(VK_SPACE, 0, 0, 0); // Simulate Space press
    keybd_event(VK_SPACE, 0, KEYEVENTF_KEYUP, 0); // Release Space
    keybd_event(VK_MENU, 0, KEYEVENTF_KEYUP, 0); // Release Alt
}
