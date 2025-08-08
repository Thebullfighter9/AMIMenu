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
        InitSpoofer();               // Evades API scanning
        InitCloakSystem();           // Starts GUI/string cloaking
        cloak::StartCloaking(hModule); // Anti-scanner memory protection

#if _VERSION
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Load, hModule, 0, nullptr);
#else
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)Run, hModule, 0, nullptr);
#endif
    }
    else if (reason == DLL_PROCESS_DETACH) {
#if _VERSION
        FreeLibrary(version_dll);
#endif
    }
    return TRUE;
}
