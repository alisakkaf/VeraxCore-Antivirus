// harden.h - Drop-in Win32 process hardening for Qt apps.
// Single header. No .cpp. No extra libs. Win7+ safe (newer APIs degrade).
// Reusable across any project - just include and call shield::harden() first.
// By Ali Sakkaf  -  https://alisakkaf.com
//
// Usage:
//   #include "harden.h"
//   int main(int argc, char *argv[]) {
//       shield::harden();              // FIRST. Before QApplication.
//       QApplication a(argc, argv);
//       ...
//   }
#pragma once
#ifdef _WIN32
#include <windows.h>

namespace shield {
inline void harden() noexcept {
    // 1) Drop CWD from DLL search - kills classic DLL planting (Floxif loves
    //    dropping fake version.dll / dwmapi.dll / winmm.dll alongside the exe).
    SetDllDirectoryW(L"");

    HMODULE k32 = GetModuleHandleW(L"kernel32.dll");
    if (!k32) return;

    // 2) Restrict default DLL search to System32 + App dir + AddDllDirectory.
    typedef BOOL (WINAPI *PFN_SDD)(DWORD);
    if (auto sdd = (PFN_SDD)GetProcAddress(k32, "SetDefaultDllDirectories"))
        sdd(0x1000 /* LOAD_LIBRARY_SEARCH_DEFAULT_DIRS */);

    // 3) Whitelist our exe directory explicitly.
    wchar_t dir[MAX_PATH] = {0};
    if (GetModuleFileNameW(nullptr, dir, MAX_PATH))
        if (wchar_t *p = wcsrchr(dir, L'\\')) {
            *p = 0;
            typedef PVOID (WINAPI *PFN_ADD)(PCWSTR);
            if (auto add = (PFN_ADD)GetProcAddress(k32, "AddDllDirectory"))
                add(dir);
        }

    // 4) Process Mitigation Policies (Win8+). Silent fallback on older Windows.
    typedef BOOL (WINAPI *PFN_SPMP)(int, PVOID, SIZE_T);
    auto spmp = (PFN_SPMP)GetProcAddress(k32, "SetProcessMitigationPolicy");
    if (!spmp) return;

    // a) Prefer System32 + reject remote/Low-IL DLLs.
    DWORD imageLoad = 0x7;
    spmp(10 /* ProcessImageLoadPolicy */, &imageLoad, sizeof(imageLoad));

    // b) Disable AppInit_DLLs / extension-point hooks.
    DWORD extPoint = 1;
    spmp(6 /* ProcessExtensionPointDisablePolicy */, &extPoint, sizeof(extPoint));

    // c) Force ASLR + reject DLLs without relocation info.
    DWORD aslr = 0xF;
    spmp(1 /* ProcessASLRPolicy */, &aslr, sizeof(aslr));
}
} // namespace shield

#else
namespace shield { inline void harden() noexcept {} }
#endif
