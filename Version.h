// ═══════════════════════════════════════════════════════════════════════
//  VeraxCore Antivirus — single source of truth for identity, version,
//  paths, URLs. Edit ONLY the macros below.
//  By Ali Sakkaf  •  https://alisakkaf.com
// ═══════════════════════════════════════════════════════════════════════
#pragma once

// ─── Identity ──────────────────────────────────────────────────────────
#define APP_NAME            "VeraxCore Antivirus"
#define APP_NAME_SHORT      "VeraxCore"
#define APP_TAGLINE         "Real protection. Zero noise."
#define APP_VENDOR          "VeraxCore"
#define APP_DESCRIPTION     "VeraxCore Antivirus - intelligent PC integrity & threat protection."
#define APP_COPYRIGHT       "Copyright (c) 2026 VeraxCore"
#define APP_HOMEPAGE        "https://alisakkaf.com"
#define APP_AUTHOR_FB       "https://www.facebook.com/AliSakkaf.Dev"
#define APP_AUTHOR_GH       "https://github.com/alisakkaf"

// ─── Version (single source of truth) ─────────────────────────────────
#define APP_VERSION_MAJOR   1
#define APP_VERSION_MINOR   0
#define APP_VERSION_PATCH   0
#define APP_VERSION_BUILD   0

// Auto-derived — DO NOT EDIT BELOW
#define _VR_STR(x) #x
#define _VR_S(x) _VR_STR(x)
#define APP_VERSION_STR \
_VR_S(APP_VERSION_MAJOR) "." _VR_S(APP_VERSION_MINOR) "." \
    _VR_S(APP_VERSION_PATCH) "." _VR_S(APP_VERSION_BUILD)
#define APP_VERSION_RC \
    APP_VERSION_MAJOR,APP_VERSION_MINOR,APP_VERSION_PATCH,APP_VERSION_BUILD

// ─── Install paths (Removed custom author name subdirectories) ────────
#define APP_INSTALL_DIR     "C:\\Program Files\\VeraxCore"
#define APP_BIN_NAME        "VeraxCore.exe"
#define APP_REG_KEY         "SOFTWARE\\AliSakkaF\\VeraxCore\\Antivirus"
#define APP_VAULT_SUBDIR    "Vault"
#define APP_LOG_SUBDIR      "Logs"

// ─── Network endpoints ─────────────────────────────────────────────────
#define APP_UPDATE_URL          "https://gist.githubusercontent.com/alisakkaf/01eaea5312e4e583f993b891554666f3/raw/VeraxCore_Antivirus.json"
#define APP_VERSION_CHECK_URL   "https://pastebin.com/raw/fkhYWEf7"
#define APP_DOWNLOAD_URL        "https://alisakkaf.com/en/windows-software/download-veraxcore-free-open-source-antivirus-windows"

// ─── Theme ────────────────────────────────────────────────────────────
#define APP_THEME           "Daylight"
