# Changelog | سجل التغييرات

All notable changes to VeraxCore Antivirus will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2026-06-02

### 🎉 Initial Release

#### Detection Engines
- SHA-256 signature database with 65+ built-in signatures
- Byte-pattern scanner with wildcard support
- PE structural analysis engine
- Heuristic scoring engine with configurable thresholds
- Optional cloud hash lookup

#### PE Repair Engine (Clean Threat)
- Section removal: 25+ known virus section names
- RWX suspicious section cleanup
- Inflated section trimming (Floxif technique detection)
- Overlay virus body removal with Authenticode certificate preservation
- Smart Entry Point repair (3-case detection)
- CRT startup pattern scanning for real EP discovery
- DLL vs EXE aware prologue restoration
- PE checksum recalculation
- Backup before repair with auto-restore on failure

#### Supported Virus Families
- Floxif (.A–.H, EC!MTB), Sality, Ramnit, Virut, Neshta
- Mikcer, Parite, Expiro, Mabezat, Viking, Alman
- Generic CodeCave, TrojanDownloader, and more

#### Quarantine Vault
- AES-256-CBC encryption via Windows BCrypt API
- HWID-derived encryption key
- 3-pass secure delete (zeros → 0xFF → random)
- Full management: restore, permanent delete, view

#### Scan Types
- Quick Scan, Full Scan, Custom Scan, Folder Scan
- USB auto-scan on device insertion

#### User Interface
- Modern glassmorphism UI with dark/light themes
- Multi-language support (English + Arabic)
- System tray with notifications
- Real-time scan progress and threat details

#### Data Management
- Portable UserData directory (next to executable)
- SQLite database with JSON fallback
- Rotating log files (5MB, keep 5)
- JSON scan reports
- Online signature updates

#### Security Hardening
- ASLR, DEP, CFG enabled
- Administrator privileges via UAC manifest
- Encrypted quarantine vault
- Secure file deletion
