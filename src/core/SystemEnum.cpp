// SystemEnum.cpp
#include "SystemEnum.h"
#include "Logger.h"
#include <QDebug>
#include <QObject>

#ifdef _WIN32
#  include <windows.h>
#  include <tlhelp32.h>
#  include <psapi.h>
#  include <wbemidl.h>
#  include <comdef.h>
#endif

namespace verax {

#ifdef _WIN32
static QString wcharToQString(const wchar_t *w) {
    return w ? QString::fromWCharArray(w) : QString();
}
#endif

QString SystemEnum::driveTypeKey(int code) {
#ifdef _WIN32
    switch (code) {
    case DRIVE_FIXED:     return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "Fixed"));
    case DRIVE_REMOVABLE: return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "Removable"));
    case DRIVE_REMOTE:    return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "Network"));
    case DRIVE_CDROM:     return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "CD/DVD"));
    case DRIVE_RAMDISK:   return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "RAM Disk"));
    default:              return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "Unknown"));
    }
#else
    Q_UNUSED(code);
    return QStringLiteral(QT_TRANSLATE_NOOP("DriveType", "Unknown"));
#endif
}

QVector<DriveInfo> SystemEnum::listDrives()
{
    QVector<DriveInfo> out;
#ifdef _WIN32
    DWORD oldMode = 0;
    HMODULE hKernel32 = GetModuleHandleW(L"kernel32.dll");
    typedef BOOL(WINAPI *SetThreadErrorMode_t)(DWORD, LPDWORD);
    SetThreadErrorMode_t pSetThreadErrorMode = nullptr;

    if (hKernel32) {
        pSetThreadErrorMode = (SetThreadErrorMode_t)GetProcAddress(hKernel32, "SetThreadErrorMode");
    }

    if (pSetThreadErrorMode) {
        pSetThreadErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX, &oldMode);
    } else {
        oldMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOOPENFILEERRORBOX);
    }

    const DWORD mask = GetLogicalDrives();
    for (int i = 0; i < 26; ++i) {
        if (!(mask & (1 << i))) continue;

        DriveInfo d;
        d.letter = QString(QChar('A' + i)) + QStringLiteral(":");
        const QString root = d.letter + QStringLiteral("\\");

        const std::wstring wroot = root.toStdWString();
        d.typeCode = GetDriveTypeW(wroot.c_str());
        d.typeName = driveTypeKey(d.typeCode);

        const bool probeSafe =
            (d.typeCode == DRIVE_FIXED) ||
            (d.typeCode == DRIVE_RAMDISK) ||
            (d.typeCode == DRIVE_REMOTE);

        if (probeSafe) {
            wchar_t label[MAX_PATH + 1] = {0};
            wchar_t fs[MAX_PATH + 1]    = {0};
            DWORD serial = 0, maxComp = 0, flags = 0;
            if (GetVolumeInformationW(wroot.c_str(), label, MAX_PATH,
                                      &serial, &maxComp, &flags,
                                      fs, MAX_PATH))
            {
                d.label      = wcharToQString(label);
                d.fileSystem = wcharToQString(fs);
            }

            ULARGE_INTEGER totalFree{}, total{}, freeAvail{};
            if (GetDiskFreeSpaceExW(wroot.c_str(), &freeAvail, &total, &totalFree))
            {
                d.totalBytes = total.QuadPart;
                d.freeBytes  = freeAvail.QuadPart;
            }
        }
        out.append(d);
    }

    if (pSetThreadErrorMode) {
        pSetThreadErrorMode(oldMode, nullptr);
    } else {
        SetErrorMode(oldMode);
    }
#endif
    return out;
}

DriveInfo SystemEnum::driveOf(const QString &letter)
{
    for (const auto &d : listDrives())
        if (d.letter.compare(letter, Qt::CaseInsensitive) == 0)
            return d;
    return {};
}

QVector<ProcInfo> SystemEnum::listProcesses()
{
    QVector<ProcInfo> out;
#ifdef _WIN32
    HANDLE snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snap == INVALID_HANDLE_VALUE) return out;

    PROCESSENTRY32W pe{};
    pe.dwSize = sizeof(pe);
    if (Process32FirstW(snap, &pe)) {
        do {
            ProcInfo p;
            p.pid  = pe.th32ProcessID;
            p.name = QString::fromWCharArray(pe.szExeFile);

            if (HANDLE h = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, pe.th32ProcessID))
            {
                wchar_t path[MAX_PATH] = {0};
                DWORD sz = MAX_PATH;
                if (QueryFullProcessImageNameW(h, 0, path, &sz))
                    p.path = QString::fromWCharArray(path);
                CloseHandle(h);
            }
            out.append(p);
        } while (Process32NextW(snap, &pe));
    }
    CloseHandle(snap);
#endif
    return out;
}

QStringList SystemEnum::installedAntivirus()
{
    static QStringList s_cache;
    static bool s_cached = false;
    if (s_cached) return s_cache;

    QStringList products;
#ifdef _WIN32
    HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
    bool comInited = SUCCEEDED(hr);

    if (hr == RPC_E_CHANGED_MODE) {
        hr = S_OK;
        comInited = false;
    }

    if (SUCCEEDED(hr)) {
        do {
            IWbemLocator *loc = nullptr;
            if (FAILED(CoCreateInstance(CLSID_WbemLocator, nullptr,
                                        CLSCTX_INPROC_SERVER,
                                        IID_IWbemLocator, (LPVOID*)&loc)))
                break;

            IWbemServices *svc = nullptr;
            BSTR ns = SysAllocString(L"ROOT\\SecurityCenter2");
            hr = loc->ConnectServer(ns, nullptr, nullptr, nullptr,
                                    WBEM_FLAG_CONNECT_USE_MAX_WAIT,
                                    nullptr, nullptr, &svc);
            SysFreeString(ns);
            if (FAILED(hr) || !svc) { loc->Release(); break; }

            CoSetProxyBlanket(svc, RPC_C_AUTHN_WINNT, RPC_C_AUTHZ_NONE, nullptr,
                              RPC_C_AUTHN_LEVEL_CALL, RPC_C_IMP_LEVEL_IMPERSONATE,
                              nullptr, EOAC_NONE);

            BSTR lang  = SysAllocString(L"WQL");
            BSTR query = SysAllocString(L"SELECT displayName FROM AntivirusProduct");
            IEnumWbemClassObject *enumr = nullptr;
            hr = svc->ExecQuery(lang, query,
                                WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
                                nullptr, &enumr);
            SysFreeString(lang);
            SysFreeString(query);

            if (SUCCEEDED(hr) && enumr) {
                IWbemClassObject *obj = nullptr;
                ULONG ret = 0;
                for (int i = 0; i < 32; ++i) {
                    hr = enumr->Next(2000, 1, &obj, &ret);
                    if (hr != WBEM_S_NO_ERROR || !obj) break;
                    VARIANT v; VariantInit(&v);
                    if (SUCCEEDED(obj->Get(L"displayName", 0, &v, nullptr, nullptr))
                        && v.vt == VT_BSTR)
                    {
                        products << QString::fromWCharArray(v.bstrVal);
                    }
                    VariantClear(&v);
                    obj->Release();
                }
                enumr->Release();
            }
            svc->Release();
            loc->Release();
        } while (false);

        if (comInited) CoUninitialize();
    }
#endif
    if (products.isEmpty()) products << QObject::tr("None detected");
    s_cache  = products;
    s_cached = true;
    return products;
}

} // namespace verax
