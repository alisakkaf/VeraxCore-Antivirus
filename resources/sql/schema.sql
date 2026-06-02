-- Verax SQLite schema
-- By Ali Sakkaf - https://alisakkaf.com

CREATE TABLE IF NOT EXISTS signatures (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    sha256          TEXT UNIQUE NOT NULL,
    name            TEXT NOT NULL,
    family          TEXT,
    severity        INTEGER DEFAULT 5,
    first_seen      INTEGER,
    source          TEXT,
    repairable      INTEGER DEFAULT 0,
    repair_method   TEXT DEFAULT '',
    byte_signatures TEXT DEFAULT '',
    entry_point_patch TEXT DEFAULT ''
);

CREATE INDEX IF NOT EXISTS idx_sigs_sha    ON signatures(sha256);
CREATE INDEX IF NOT EXISTS idx_sigs_family ON signatures(family);
CREATE INDEX IF NOT EXISTS idx_sigs_repairable ON signatures(repairable);

CREATE TABLE IF NOT EXISTS quarantine (
    id              INTEGER PRIMARY KEY AUTOINCREMENT,
    original_path   TEXT NOT NULL,
    vault_path      TEXT NOT NULL,
    sha256          TEXT,
    detection_name  TEXT,
    size            INTEGER,
    quarantined_at  INTEGER,
    restore_blocked INTEGER DEFAULT 0
);

CREATE TABLE IF NOT EXISTS settings (
    key   TEXT PRIMARY KEY,
    value TEXT
);

CREATE TABLE IF NOT EXISTS scan_history (
    id            INTEGER PRIMARY KEY AUTOINCREMENT,
    started_at    INTEGER,
    finished_at   INTEGER,
    files_scanned INTEGER,
    threats_found INTEGER,
    report_json   TEXT
);

CREATE INDEX IF NOT EXISTS idx_history_started ON scan_history(started_at);
