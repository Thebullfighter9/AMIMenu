#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "il2cpp-init.h"
#include "main.h"
#include "spoofer.h"
#include "cloak.h"
#if _VERSION
#include "version.h"
#endif

BOOL APIENTRY DllMain(HMODULE hModule, DWORD reason, LPVOID) {
    if (reason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        InitSpoofer();                // API & memory spoofing
        cloak::InitCloakSystem();     // GUI & string obfuscation
        cloak::StartCloaking(hModule); // Anti-scanner memory blocking
#if _VERSION
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Load, hModule, 0, nullptr);
#else
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Run, hModule, 0, nullptr);
#endif
    }
    return TRUE;
}
