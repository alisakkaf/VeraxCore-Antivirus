# Security Policy | سياسة الأمان

## Supported Versions | الإصدارات المدعومة

| Version | Supported |
|---|---|
| 1.0.x | ✅ Active |
| < 1.0 | ❌ Not supported |

## Reporting a Vulnerability | الإبلاغ عن ثغرة أمنية

### English

We take the security of VeraxCore Antivirus seriously. If you discover a security vulnerability, please report it responsibly.

**DO NOT** open a public GitHub issue for security vulnerabilities.

#### How to Report

1. **Email**: Send a detailed report to the project maintainer through the contact form at [alisakkaf.com](https://alisakkaf.com)
2. **Include**:
   - Description of the vulnerability
   - Steps to reproduce
   - Potential impact
   - Suggested fix (if any)

#### Response Timeline

- **Acknowledgment**: Within 48 hours
- **Initial Assessment**: Within 7 days
- **Fix Release**: Within 30 days for critical vulnerabilities

#### What We Consider Security Issues

- Remote code execution through crafted PE files
- Bypass of quarantine encryption (AES-256-CBC)
- Privilege escalation through the application
- Signature database tampering
- Path traversal in scan or quarantine operations
- Memory corruption in the PE repair engine

#### What We Do NOT Consider Security Issues

- Denial of service through very large files
- Issues requiring physical access to the machine
- Social engineering attacks
- Issues in third-party dependencies (report to them directly)

#### Safe Harbor

We support responsible disclosure. If you follow this policy, we will:
- Not take legal action against you
- Work with you to understand and resolve the issue
- Credit you in the security advisory (unless you prefer anonymity)

---

### العربية

نأخذ أمان VeraxCore Antivirus على محمل الجد. إذا اكتشفت ثغرة أمنية، يرجى الإبلاغ عنها بشكل مسؤول.

**لا تفتح** مشكلة (issue) عامة على GitHub للثغرات الأمنية.

#### كيفية الإبلاغ

1. **البريد الإلكتروني**: أرسل تقريراً مفصلاً عبر نموذج التواصل في [alisakkaf.com](https://alisakkaf.com)
2. **يجب أن يتضمن**:
   - وصف الثغرة
   - خطوات إعادة الإنتاج
   - التأثير المحتمل
   - الإصلاح المقترح (إن وُجد)

#### جدول الاستجابة

- **التأكيد**: خلال 48 ساعة
- **التقييم الأولي**: خلال 7 أيام
- **إصدار الإصلاح**: خلال 30 يوماً للثغرات الحرجة
