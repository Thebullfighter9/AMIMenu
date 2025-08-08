#include "cloak.h"
#include <Windows.h>
#include <TlHelp32.h>
#include <Psapi.h>
#include <string>
#include <thread>
#include <chrono>
#include <random>

namespace cloak {

    void InitCloakSystem() {
        // Placeholder: Add GUI string masking, window randomization etc. if needed
    }

    std::string GenerateRandomClassName(size_t len) {
        static const char charset[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
        std::string result;
        result.reserve(len);
        for (size_t i = 0; i < len; ++i)
            result += charset[rand() % (sizeof(charset) - 1)];
        return result;
    }

    void RandomizeWindowTitle(HWND hwnd) {
        std::string title = GenerateRandomClassName(12);
        std::wstring wtitle(title.begin(), title.end());
        SetWindowTextW(hwnd, wtitle.c_str());
    }

    void HideMemoryRegion(void* address, size_t size) {
        DWORD oldProtect;
        VirtualProtect(address, size, PAGE_NOACCESS, &oldProtect);
    }

    void RestoreMemoryRegion(void* address, size_t size) {
        DWORD oldProtect;
        VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect);
    }

    bool IsScannerPresent(const std::string& targetName) {
        HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
        if (hSnapshot == INVALID_HANDLE_VALUE) return false;

        PROCESSENTRY32 pe;
        pe.dwSize = sizeof(PROCESSENTRY32);

        if (Process32First(hSnapshot, &pe)) {
            do {
                std::wstring ws(pe.szExeFile);
                std::string processName(ws.begin(), ws.end());

                if (processName == targetName) {
                    CloseHandle(hSnapshot);
                    return true;
                }
            } while (Process32Next(hSnapshot, &pe));
        }
        CloseHandle(hSnapshot);
        return false;
    }

    void MonitorForScannersAndCloak(HMODULE hModule) {
        while (true) {
            if (IsScannerPresent("NCAU.exe") || IsScannerPresent("modscan.exe")) {
                MEMORY_BASIC_INFORMATION mbi;
                if (VirtualQuery(hModule, &mbi, sizeof(mbi))) {
                    HideMemoryRegion(mbi.BaseAddress, mbi.RegionSize);
                    std::this_thread::sleep_for(std::chrono::seconds(10));
                    RestoreMemoryRegion(mbi.BaseAddress, mbi.RegionSize);
                }
            }
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }

    void StartCloaking(HMODULE hModule) {
        std::thread([=]() { MonitorForScannersAndCloak(hModule); }).detach();
    }

}
