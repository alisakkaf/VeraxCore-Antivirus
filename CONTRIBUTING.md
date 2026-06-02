# Contributing to VeraxCore Antivirus | المساهمة في VeraxCore

Thank you for your interest in contributing to VeraxCore Antivirus! 🎉

## 📋 Table of Contents

- [Code of Conduct](#code-of-conduct)
- [How to Contribute](#how-to-contribute)
- [Development Setup](#development-setup)
- [Coding Standards](#coding-standards)
- [Pull Request Process](#pull-request-process)
- [Reporting Issues](#reporting-issues)

## Code of Conduct

This project adheres to a code of conduct. By participating, you are expected to uphold this code. Please see [CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md).

## How to Contribute

### 🐛 Bug Reports

1. Check existing [Issues](https://github.com/alisakkaf/VeraxCore-Antivirus/issues) to avoid duplicates
2. Use the bug report template
3. Include:
   - OS version (Windows 10/11, build number)
   - VeraxCore version
   - Steps to reproduce
   - Expected vs actual behavior
   - Log files from `UserData/Logs/`
   - Screenshots if applicable

### 💡 Feature Requests

1. Check existing issues for similar requests
2. Describe the feature clearly
3. Explain why it would be useful
4. Suggest implementation approach if possible

### 🔧 Code Contributions

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test thoroughly
5. Commit with clear messages (`git commit -m 'Add: amazing feature'`)
6. Push to your branch (`git push origin feature/amazing-feature`)
7. Open a Pull Request

### 📋 Signature Contributions

Help expand our malware signature database:
1. Add entries to `resources/signatures/seed.json`
2. Each entry must include valid SHA-256 hash (64 chars)
3. Include family name, severity, and source
4. Test detection with the application

## Development Setup

### Prerequisites
- Qt 5.15+ (MSVC 2019 or MinGW)
- C++17 compatible compiler
- Windows SDK 10.0+
- Git

### Steps
```bash
git clone https://github.com/alisakkaf/VeraxCore-Antivirus.git
cd VeraxCore
# Open Verax.pro in Qt Creator
```

## Coding Standards

### C++ Style
- Use C++17 features where appropriate
- Follow Qt naming conventions:
  - Classes: `PascalCase`
  - Functions/methods: `camelCase`
  - Member variables: `m_camelCase`
  - Constants: `kCamelCase`
- Use `QStringLiteral()` for string literals
- Use `auto` where type is obvious
- Comment complex logic in English

### Commit Messages
```
Type: Short description

Types: Add, Fix, Update, Remove, Refactor, Docs
```

### File Organization
- Core engine code → `src/core/`
- UI code → `src/ui/`
- Widgets → `src/widgets/`
- Utilities → `src/utils/`

## Pull Request Process

1. Ensure your code compiles without warnings
2. Test with both infected and clean files
3. Update documentation if needed
4. Ensure brace balance in modified files
5. Do not include build artifacts
6. Reference any related issues

## Reporting Issues

### ⚠️ Security Vulnerabilities

**DO NOT** open public issues for security vulnerabilities. See [SECURITY.md](SECURITY.md).

### General Issues

For general bugs, use the GitHub Issues tracker with:
- Clear title
- Detailed description
- Reproduction steps
- Environment details

---

## 📜 License

By contributing, you agree that your contributions will be licensed under the [GPLv3 License](LICENSE).

---

Thank you for helping make VeraxCore better! 🛡️
