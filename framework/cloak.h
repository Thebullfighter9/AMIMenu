#pragma once
#include <string>
#include <random>
#include <Windows.h>
#include <imgui/imgui.h>

#define XOR_KEY 0x7A
#define XOR_STR(str) [] { static char s[] = str; for (int i = 0; s[i] != '\0'; ++i) s[i] ^= XOR_KEY; return s; }()

inline std::string GenerateRandomName(size_t len = 10) {
    static const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::string name;
    for (size_t i = 0; i < len; ++i)
        name += chars[rand() % (sizeof(chars) - 1)];
    return name;
}

inline const char* SafeLabel(const char* label) {
    static thread_local std::string obfuscated = GenerateRandomName(8);
    return obfuscated.c_str();
}

#define SAFE_TEXT(str) ImGui::Text(XOR_STR(str))
#define SAFE_BUTTON(str) ImGui::Button(SafeLabel(XOR_STR(str)))
#define SAFE_BEGIN(str, open, flags) ImGui::Begin(SafeLabel(XOR_STR(str)), open, flags)
#define SAFE_END() ImGui::End()
#define SAFE_WINDOW(str, open) ImGui::Begin(SafeLabel(XOR_STR(str)), open)

namespace cloak {
    void InitCloakSystem(); // <-- Add this so dllmain compiles
    void StartCloaking(HMODULE hModule);
}
