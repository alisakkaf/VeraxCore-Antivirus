<p align="center">
  <img src="docs/screenshots/logo.png" alt="VeraxCore Antivirus" width="120"/>
</p>

<h1 align="center">🛡️ VeraxCore Antivirus</h1>

<p align="center">
  <strong>حماية حقيقية. بدون ضجيج.</strong><br/>
  <em>مكافح فيروسات مجاني ومفتوح المصدر وذكي لنظام Windows</em>
</p>

<p align="center">
  <a href="#%EF%B8%8F-%D8%A5%D8%B5%D8%AF%D8%A7%D8%B1%D8%A7%D8%AA-windows-%D8%A7%D9%84%D9%85%D8%AF%D8%B9%D9%88%D9%85%D8%A9"><img src="https://img.shields.io/badge/Windows-7%20%7C%208%20%7C%2010%20%7C%2011-0078D6?style=for-the-badge&logo=windows&logoColor=white" alt="Platform"/></a>
  <a href="#%D8%AF%D8%B9%D9%85-%D8%A7%D9%84%D9%85%D8%B9%D9%85%D8%A7%D8%B1%D9%8A%D8%A7%D8%AA"><img src="https://img.shields.io/badge/Arch-x86%20%7C%20x64%20%7C%20ARM64-8B5CF6?style=for-the-badge&logo=windows-terminal&logoColor=white" alt="Architecture"/></a>
  <a href="https://github.com/alisakkaf/VeraxCore-Antivirus/releases"><img src="https://img.shields.io/badge/Version-1.0.0-10B981?style=for-the-badge&logo=semantic-release&logoColor=white" alt="Version"/></a>
  <a href="LICENSE"><img src="https://img.shields.io/badge/License-GPLv3-3B82F6?style=for-the-badge&logo=gnu&logoColor=white" alt="License"/></a>
</p>

<p align="center">
  <a href="#"><img src="https://img.shields.io/badge/C++-17-00599C?style=for-the-badge&logo=cplusplus&logoColor=white" alt="C++17"/></a>
  <a href="#"><img src="https://img.shields.io/badge/Qt-5.15+-41CD52?style=for-the-badge&logo=qt&logoColor=white" alt="Qt"/></a>
  <a href="#"><img src="https://img.shields.io/badge/SQLite-3-003B57?style=for-the-badge&logo=sqlite&logoColor=white" alt="SQLite"/></a>
  <a href="#"><img src="https://img.shields.io/badge/AES--256-Encryption-EF4444?style=for-the-badge&logo=letsencrypt&logoColor=white" alt="AES-256"/></a>
</p>

<p align="center">
  <a href="https://github.com/alisakkaf/VeraxCore-Antivirus/releases"><img src="https://img.shields.io/github/downloads/alisakkaf/VeraxCore-Antivirus/total?style=for-the-badge&logo=github&label=Downloads&color=22C55E" alt="Downloads"/></a>
  <a href="https://github.com/alisakkaf/VeraxCore-Antivirus/stargazers"><img src="https://img.shields.io/github/stars/alisakkaf/VeraxCore-Antivirus?style=for-the-badge&logo=github&label=Stars&color=F59E0B" alt="Stars"/></a>
  <a href="https://github.com/alisakkaf/VeraxCore-Antivirus/issues"><img src="https://img.shields.io/github/issues/alisakkaf/VeraxCore-Antivirus?style=for-the-badge&logo=github&label=Issues&color=6366F1" alt="Issues"/></a>
  <a href="README.md"><img src="https://img.shields.io/badge/English-README-F97316?style=for-the-badge&logo=googletranslate&logoColor=white" alt="English"/></a>
</p>

<p align="center">
  <a href="#-لقطات-الشاشة">لقطات الشاشة</a> •
  <a href="#-المميزات">المميزات</a> •
  <a href="#-البنية-المعمارية">البنية</a> •
  <a href="#-التثبيت">التثبيت</a> •
  <a href="#-البناء-من-المصدر">البناء</a> •
  <a href="#-الأسئلة-الشائعة">أسئلة شائعة</a>
</p>


---

## 📖 نبذة عن المشروع

**VeraxCore Antivirus** هو مكافح فيروسات **مجاني ومفتوح المصدر** لنظام Windows، مبني بلغة C++17 وإطار عمل Qt 5.15+. يجمع بين أربعة محركات كشف متقدمة — **مطابقة تواقيع SHA-256**، **فحص أنماط البايت**، **تحليل بنية PE**، و**التقييم السلوكي (Heuristic)** — لاكتشاف وإصلاح الملفات التنفيذية (EXE) والمكتبات (DLL) المصابة **دون تدميرها**.

على عكس معظم أدوات مكافحة الفيروسات التي تحذف الملفات المصابة ببساطة، يتميز VeraxCore بـ**محرك إصلاح PE متقدم** يستطيع إزالة كود الفيروس جراحياً من الملفات المصابة مع الحفاظ على وظائف البرنامج الأصلي — تماماً كما تفعل محركات مكافحة الفيروسات الاحترافية.

### 🎯 لماذا VeraxCore؟

| المشكلة | حل VeraxCore |
|---|---|
| المكافح يحذف ملفاتك المصابة | **يصلحها** — يزيل الفيروس ويحافظ على البرنامج |
| البرامج تتوقف بعد التنظيف (خطأ 0xc0000142) | **إصلاح ذكي لنقطة الدخول** — يكتشف هل الفيروس عدّل EP |
| حجم الملفات يتغير بعد الإصلاح | **جراحة PE دقيقة** — يحافظ على overlay والشهادات الرقمية |
| إنذارات كاذبة على برامج سليمة | **تحقق متعدد المحركات** — 4 طرق كشف يجب أن تتفق |
| مكافح ثقيل يستهلك الموارد | **خفيف** — أقل من 50 ميجابايت، بدون خدمات خلفية |
| المكافح يحتاج إنترنت | **يعمل بالكامل بدون إنترنت** — كل المحركات تعمل محلياً |
| مكافحات فيروسات تجارية مكلفة | **مجاني 100% ومفتوح المصدر** — GPLv3، مجاني للأبد |
| المكافح يكسر مكتبات DLL | **إصلاح واعي بالـ DLL** — استعادة prologue مختلفة للـ DLL |
| لا يمكن استرجاع الملفات المحجورة | **خزنة مشفرة بـ AES-256** — استعادة في أي وقت مع سلامة كاملة |
| لا شفافية في نتائج الفحص | **تقارير JSON مفصلة** — سجل تدقيق كامل لكل فحص |

---

## 🖥️ إصدارات Windows المدعومة

VeraxCore مصمم للعمل عبر نطاق واسع من إصدارات Windows والمعماريات:

| إصدار Windows | المعمارية | الحالة | ملاحظات |
|---|---|---|---|
| **Windows 11** (23H2, 24H2) | x64, ARM64 | ✅ مدعوم بالكامل | هدف التطوير الأساسي |
| **Windows 11** (21H2, 22H2) | x64, ARM64 | ✅ مدعوم بالكامل | |
| **Windows 10** (22H2) | x86, x64 | ✅ مدعوم بالكامل | الأكثر اختباراً |
| **Windows 10** (21H2, 21H1) | x86, x64 | ✅ مدعوم بالكامل | |
| **Windows 10** (1809–20H2) | x86, x64 | ✅ مدعوم | |
| **Windows 8.1** | x86, x64 | ⚠️ متوافق | غير مُختبر بشكل مكثف |
| **Windows 8** | x86, x64 | ⚠️ متوافق | اختبار محدود |
| **Windows 7** SP1 | x86, x64 | ⚠️ متوافق | يتطلب تحديثات KB |
| **Windows Server 2019/2022** | x64 | ✅ مدعوم | بيئات الخوادم |
| **Windows Server 2016** | x64 | ⚠️ متوافق | |

### دعم المعماريات

| المعمارية | مستوى الدعم | التفاصيل |
|---|---|---|
| **x64 (AMD64)** | ✅ كامل | الهدف الأساسي، جميع الميزات |
| **x86 (32-بت)** | ✅ كامل | وظائف كاملة |
| **ARM64** | ⚠️ عبر المحاكاة | يعمل عبر طبقة محاكاة x86/x64 في Windows |

### متطلبات النظام

| المكون | الحد الأدنى | الموصى به |
|---|---|---|
| **نظام التشغيل** | Windows 7 SP1 | Windows 10/11 |
| **المعالج** | 1 GHz نواة واحدة | 2 GHz نواتين |
| **الذاكرة** | 256 ميجابايت | 512 ميجابايت+ |
| **المساحة** | 50 ميجابايت (التطبيق) | 200 ميجابايت (التطبيق + UserData) |
| **الصلاحيات** | مسؤول (Administrator) | مسؤول (Administrator) |
| **الشاشة** | 1024×768 | 1280×720+ |
| **.NET** | غير مطلوب | — |
| **الإنترنت** | غير مطلوب | اختياري (للتحديثات) |

---

## 📸 لقطات الشاشة

<p align="center">
  <em>سيتم إضافة لقطات الشاشة هنا</em>
</p>

<!-- أضف لقطات الشاشة هكذا:
<p align="center">
  <img src="docs/screenshots/main_dashboard.png" alt="لوحة التحكم الرئيسية" width="700"/>
</p>
<p align="center">
  <img src="docs/screenshots/scan_results.png" alt="نتائج الفحص" width="700"/>
</p>
<p align="center">
  <img src="docs/screenshots/quarantine.png" alt="خزنة الحجر" width="700"/>
</p>
<p align="center">
  <img src="docs/screenshots/settings.png" alt="الإعدادات" width="700"/>
</p>
-->

---

## ✨ المميزات

### 🔍 كشف متعدد المحركات
- **قاعدة بيانات تواقيع SHA-256** — أكثر من 65 توقيع برمجيات خبيثة مع تحديثات أونلاين
- **ماسح أنماط البايت** — تواقيع بايت مع دعم wildcards (مثل: `E9??????????558BEC`)
- **تحليل بنية PE** — يكشف الأقسام المشبوهة، خطافات نقطة الدخول، وكهوف الكود
- **محرك سلوكي (Heuristic)** — تقييم سلوكي بعتبة قابلة للتعديل (0–100)
- **استعلام سحابي** — تحقق اختياري عبر الإنترنت من هاش الملفات

### 🔧 محرك إصلاح PE المتقدم (تنظيف التهديدات)
**الجوهرة الحقيقية** في VeraxCore — محرك إصلاح ملفات PE بمستوى احترافي:

- **إزالة الأقسام** — يزيل أكثر من 25 قسم فيروسي معروف (`.flx`، `.sality`، `.rmnet`، `.virut`، إلخ)
- **تنظيف أقسام RWX** — يكشف ويزيل الأقسام المشبوهة (قراءة-كتابة-تنفيذ)
- **قص الأقسام المنتفخة** — يكتشف كود الفيروس الملحق بالأقسام الموجودة (تقنية Floxif)
- **إزالة فيروس الـ Overlay** — يحذف جسم الفيروس من overlay الملف مع الحفاظ على شهادات Authenticode
- **إصلاح نقطة الدخول الذكي**:
  - يكتشف هل الفيروس **فعلاً** عدّل نقطة الدخول (JMP/CALL redirect)
  - **لا يلمس EP** إذا كان سليماً
  - يبحث في قسم .text عن أنماط CRT startup لإيجاد EP الحقيقي
  - يميّز بين DLL: `8BFF558BEC` (hotpatch) و EXE: `558BEC6AFF` (SEH)
- **الحفاظ على سلامة PE**:
  - إعادة حساب SizeOfImage بدقة
  - إعادة حساب PE checksum
  - حجم الملف بناءً على البايتات المحذوفة فعلياً (يحافظ على overlay)
  - نسخ احتياطي قبل أي عملية إصلاح

### 🛡️ دعم شامل لعائلات الفيروسات

| العائلة | تقنية الحقن | الكشف | الإصلاح |
|---|---|---|---|
| **Floxif** (.A–.H, EC) | خطف EP + إضافة overlay/section | ✅ | ✅ |
| **Sality** | إضافة section + تحويل EP | ✅ | ✅ |
| **Ramnit** | إضافة section + خطف EP | ✅ | ✅ |
| **Virut** | كهف كود + خطف EP | ✅ | ✅ |
| **Neshta** | إضافة section + عدوى ملفات | ✅ | ✅ |
| **Mikcer** | خطف EP + إضافة section | ✅ | ✅ |
| **Parite** | إضافة section متعددة الأشكال | ✅ | ✅ |
| **Expiro** | حقن كود | ✅ | ✅ |
| **Mabezat** | إضافة section | ✅ | ✅ |
| **Viking** | إضافة section | ✅ | ✅ |
| **Alman** | إضافة section | ✅ | ✅ |
| **Generic CodeCave** | حقن في كهوف الكود | ✅ | ✅ |
| **TrojanDownloader** | تلاعب متنوع بـ PE | ✅ | ✅ |

### 🏦 خزنة الحجر الصحي (Quarantine Vault)
- **تشفير AES-256-CBC** — الملفات مشفرة باستخدام Windows BCrypt API
- **مفتاح مشتق من الجهاز (HWID)** — مفتاح التشفير مستمد من MachineGuid الفريد
- **حذف آمن** — كتابة 3 مراحل (أصفار → 0xFF → عشوائي) قبل الحذف
- **إدارة كاملة** — استعادة، حذف نهائي، أو عرض العناصر المحجورة

### 📊 أنواع الفحص
- **فحص سريع** — المواقع الحساسة في النظام (Temp، التنزيلات، AppData، مجلدات بدء التشغيل)
- **فحص كامل** — فحص القرص بالكامل مع تتبع المجلدات الفرعية
- **فحص مخصص** — ملفات أو مجلدات يختارها المستخدم
- **فحص مجلد** — فحص مجلد واحد
- **فحص USB تلقائي** — فحص تلقائي عند إدخال أجهزة USB

### 🌐 مميزات إضافية
- **واجهة متعددة اللغات** — عربي + إنجليزي (قابلة للتوسيع عبر Qt i18n)
- **تحديث التواقيع أونلاين** — تحديث تلقائي من خادم JSON بعيد
- **سجل الفحوصات** — سجل تدقيق كامل مخزن في SQLite
- **تقارير JSON** — تقارير مفصلة لكل فحص
- **بدء مع Windows** — تسجيل اختياري في بدء التشغيل
- **شريط النظام (System Tray)** — تصغير للشريط مع إشعارات
- **سمة داكنة/فاتحة** — واجهة عصرية بتأثيرات glassmorphism
- **بيانات محمولة** — جميع البيانات مخزنة في `UserData/` بجوار الملف التنفيذي

---

## 🏗️ البنية المعمارية

```
VeraxShield/
├── main.cpp                    # نقطة دخول التطبيق
├── Version.h                   # المصدر الوحيد للإصدار والهوية
├── harden.h                    # تعزيز الأمان (ASLR, DEP, CFG)
├── manifest.xml                # ملف UAC لنظام Windows (صلاحيات المدير)
├── app.rc                      # ملف موارد Windows (أيقونة، معلومات الإصدار)
├── Verax.pro                   # ملف مشروع Qt
│
├── src/
│   ├── core/
│   │   ├── Scanner.cpp/.h      # 🔍 محرك الفحص الرئيسي (4000+ سطر)
│   │   ├── SignatureDb.cpp/.h   # 📦 قاعدة بيانات التواقيع (SQLite + JSON)
│   │   ├── Quarantine.cpp/.h   # 🏦 خزنة مشفرة بـ AES-256
│   │   ├── Settings.cpp/.h     # ⚙️ إعدادات مستمرة (مدعومة بالسجل)
│   │   └── Logger.cpp/.h       # 📝 مسجل أحداث دوّار
│   │
│   ├── ui/
│   │   └── MainWindow.cpp/.h   # 🖥️ نافذة التطبيق الرئيسية
│   │
│   ├── widgets/                # عناصر واجهة إضافية
│   └── utils/                  # دوال مساعدة
│
├── resources/
│   ├── signatures/
│   │   └── seed.json           # 📋 65+ توقيع برمجيات خبيثة مدمجة
│   ├── sql/schema.sql          # مخطط قاعدة البيانات
│   ├── icons/                  # أيقونات التطبيق
│   └── themes/                 # سمات الواجهة (QSS)
│
├── i18n/                       # 🌐 ملفات الترجمة (.ts/.qm)
├── third_party/                # مكتبات خارجية
├── docs/                       # التوثيق ولقطات الشاشة
└── build/                      # مخرجات البناء
```

### تدفق محرك الكشف

```
ملف الإدخال
    ↓
[1] البحث في قاعدة SHA-256 ← تطابق؟ → تهديد معروف
    ↓ لا
[2] فحص أنماط البايت ← تطابق؟ → تهديد معروف
    ↓ لا
[3] تحليل بنية PE ← مشبوه؟ → [4] التقييم السلوكي
    ↓ نظيف                         ↓
    ✓ آمن              نقاط ≥ العتبة → تهديد | نقاط < العتبة → آمن ✓
```

### خط أنابيب إصلاح PE

```
الخطوة A0: تحليل EP — كشف تحويل JMP/CALL
الخطوة A1: استعادة prologue الأصلي من قسم الفيروس
المرور 1: إزالة أقسام الفيروسات المسماة (.flx, .sality, إلخ)
المرور 2: إزالة أقسام RWX المشبوهة
المرور 3: قص الأقسام المنتفخة (SizeOfRawData مكبّر بالفيروس)
الخطوة C: إصلاح نقطة الدخول (3 حالات ذكية)
الخطوة D: تصفير كهوف الكود وأجسام الفيروس داخل الأقسام
الخطوة E: إصلاح SizeOfImage + إزالة فيروس overlay (مع الحفاظ على الشهادة)
الخطوة F: قص الملف للحجم الصحيح
الخطوة G: التحقق من سلامة PE
```

---

## 📥 التثبيت

### وضعان: تثبيت أو محمول (Portable)

VeraxCore يعمل **مباشرة بدون إعداد** بوضعين — أنت تختار عند أول تشغيل:

#### 🔹 الخيار أ: وضع التثبيت (موصى به)
عند أول تشغيل، VeraxCore **يسألك تلقائياً** إذا كنت تريد تثبيته:
1. حمّل أحدث إصدار من صفحة [Releases](https://github.com/alisakkaf/VeraxCore-Antivirus/releases)
2. شغّل `VeraxCore.exe` **كمسؤول (Administrator)**
3. عند أول تشغيل، يظهر حوار: **"هل تريد تثبيت VeraxCore؟"**
4. اضغط **نعم** ← VeraxCore يثبّت نفسه في `C:\Program Files\VeraxCore Antivirus\`
5. ينشئ اختصار في قائمة ابدأ وسطح المكتب وبدء التشغيل
6. يُفعّل **التحديثات التلقائية** — يتحقق من الإصدارات الجديدة ويحدّث نفسه
7. جاهز! VeraxCore يحمي نظامك الآن

#### 🔹 الخيار ب: الوضع المحمول (Portable)
لا تريد التثبيت؟ لا مشكلة — VeraxCore **محمول بالتصميم**:
- فقط فك الضغط وشغّل — بدون معالج تثبيت
- جميع البيانات مخزنة في `UserData/` بجوار الملف التنفيذي
- لا يلوّث سجل النظام (Registry) — إلا إدخال بدء التشغيل الاختياري
- انقل المجلد بالكامل لأي مكان أو فلاش USB أو قرص خارجي
- يعمل على أي جهاز Windows بدون تثبيت
- مثالي لفلاشات الإنقاذ USB وأدوات الفنيين

### نظام التحديث التلقائي
VeraxCore يتضمن نظام **تحديث تلقائي** مدمج:
- 🔄 **فحص الإصدار** — يتحقق تلقائياً من الإصدارات الجديدة عند بدء التشغيل
- 📥 **تحديث بضغطة واحدة** — حمّل وثبّت التحديثات من داخل التطبيق
- 📋 **تحديث التواقيع** — قاعدة بيانات تواقيع البرمجيات الخبيثة تُحدَّث بشكل مستقل
- 🔔 **إشعارات التحديث** — يُعلمك عند توفر إصدار جديد
- ⚙️ **قابل للتخصيص** — فعّل/عطّل التحديث التلقائي من الإعدادات

### أول تشغيل
عند أول تشغيل، VeraxCore سيقوم بـ:
1. يسألك هل تريد **التثبيت** أو التشغيل في **الوضع المحمول**
2. إنشاء هيكل مجلد `UserData/` تلقائياً
3. تهيئة قاعدة بيانات SQLite للتواقيع (أو احتياطي JSON)
4. استيراد 65+ توقيع برمجيات خبيثة مدمج من `seed.json`
5. التحقق من التحديثات المتاحة (إذا كان متصلاً بالإنترنت)
6. تسجيل إدخال بدء التشغيل الاختياري (إذا مُفعّل)
7. عرض لوحة التحكم الرئيسية — جاهز للفحص!

---

## 🔨 البناء من المصدر

### المتطلبات المسبقة
- [Qt 5.15+](https://www.qt.io/download) (MSVC 2019 أو MinGW)
- مُصرّف متوافق مع C++17
- Windows SDK 10.0+

### خطوات البناء

```bash
# استنساخ المستودع
git clone https://github.com/alisakkaf/VeraxCore-Antivirus.git
cd VeraxCore

# باستخدام Qt Creator
# 1. افتح Verax.pro في Qt Creator
# 2. اختر kit (MSVC 2019 أو MinGW)
# 3. Build → Build Project (Ctrl+B)

# باستخدام سطر الأوامر (MSVC)
qmake Verax.pro -spec win32-msvc
nmake release

# باستخدام سطر الأوامر (MinGW)
qmake Verax.pro -spec win32-g++
mingw32-make release
```

---

## 📁 تخزين البيانات

جميع بيانات المستخدم مخزنة في مجلد `UserData/` بجوار الملف التنفيذي:

```
VeraxCore Antivirus/
├── VeraxCore.exe
├── UserData/
│   ├── db/
│   │   ├── verax.sqlite          # قاعدة بيانات التواقيع
│   │   └── verax_signatures.json # احتياطي JSON
│   ├── Logs/
│   │   └── verax.log             # سجلات التطبيق (تدوير عند 5 ميجابايت)
│   ├── Vault/
│   │   └── *.qvault              # ملفات محجورة مشفرة
│   └── reports/
│       └── scan-*.json           # تقارير الفحص
```

---

## ❓ الأسئلة الشائعة

### س: هل سيحذف VeraxCore ملفاتي؟
**لا.** نهج VeraxCore الأساسي هو **إصلاح** الملفات المصابة. يزيل كود الفيروس جراحياً مع الحفاظ على البرنامج الأصلي. يمكنك أيضاً اختيار الحجر الصحي (نسخة مشفرة) أو الحذف.

### س: لماذا يحتاج صلاحيات المدير؟
لفحص ملفات النظام في `C:\Windows\`، `C:\Program Files\`، والمواقع المحمية الأخرى. بدون صلاحيات المدير، لا يمكن قراءة العديد من ملفات النظام.

### س: ماذا يحدث إذا فشل الإصلاح؟
VeraxCore ينشئ نسخة احتياطية `.bak` قبل أي إصلاح. إذا فشل الإصلاح أو لم يعمل الملف، يستعيد تلقائياً من النسخة الاحتياطية.

### س: هل يحل محل Windows Defender؟
**لا.** VeraxCore مصمم كأداة **مكمّلة**، مفيد بشكل خاص لـ**إصلاح** الملفات المصابة التي يحذفها Windows Defender ببساطة.

### س: ما هو خطأ 0xc0000142؟
هذا خطأ Windows يعني أن البرنامج فشل في التهيئة. محرك الإصلاح الذكي في VeraxCore مصمم خصيصاً لمنع هذا عن طريق استعادة نقاط الدخول وبنية PE بشكل صحيح.

### س: هل بياناتي آمنة في الحجر الصحي؟
**نعم.** الملفات المحجورة مشفرة بـ AES-256-CBC باستخدام مفتاح مشتق من معرّف الجهاز الفريد. لا يمكن فك تشفيرها على جهاز آخر.

### س: هل يعمل بدون إنترنت؟
**نعم.** جميع محركات الكشف تعمل بالكامل بدون اتصال. تحديثات التواقيع عبر الإنترنت اختيارية.

### س: ما هي أخطاء Windows الشائعة التي يحلها VeraxCore؟

| الخطأ | السبب | حل VeraxCore |
|---|---|---|
| `0xc0000142` | فشل تهيئة البرنامج بسبب EP تالف | إصلاح ذكي لنقطة الدخول |
| `0xc000007b` | DLL تالفة أو مفقودة | إصلاح بنية DLL |
| `Application Error` | ملف تنفيذي مصاب | إزالة الفيروس مع الحفاظ على البرنامج |
| `Startup Load Failed` | Entry Point محوّل | استعادة EP الحقيقي عبر CRT scan |
| حجم ملف متغير | فيروس أضاف بيانات | قص دقيق لـ overlay الفيروس |

### س: هل هو مجاني فعلاً؟ ما الخدعة؟

VeraxCore **مجاني 100% ومفتوح المصدر** بموجب GPLv3. لا خدعة:
- لا إعلانات
- لا تتبع أو جمع بيانات
- لا إصدار مدفوع/متميز
- لا قيود على الميزات
- لا فترة تجريبية
- مجاني للأبد

### س: هل يدعم ملفات 64-بت؟

**نعم.** VeraxCore يدعم كلاً من:
- **PE32** (32-بت) — ملفات تنفيذية ومكتبات DLL
- **PE32+** (64-بت) — ملفات تنفيذية ومكتبات DLL

المحرك يكتشف نوع PE تلقائياً ويطبّق استراتيجية الإصلاح الصحيحة.

### س: ما أنواع الملفات التي يفحصها؟

VeraxCore يركز على ملفات PE (Portable Executable):
- `.exe` — ملفات تنفيذية
- `.dll` — مكتبات الربط الديناميكي
- `.scr` — شاشات التوقف
- `.sys` — ملفات التعريف (كشف فقط)
- `.ocx` — عناصر تحكم ActiveX

كما يقوم بمطابقة هاش SHA-256 على **أي نوع ملف**.

### س: كيف أضيف تواقيع برمجيات خبيثة مخصصة؟

عدّل ملف `resources/signatures/seed.json` وأضف إدخالات:
```json
{
    "sha256": "هاش_64_حرف_هنا...",
    "name": "Virus:Win32/اسم_مخصص",
    "family": "اسم_العائلة",
    "severity": 8,
    "repairable": true,
    "repair_method": "PE.SectionWipe+EP.Restore",
    "byte_signatures": ["E9????????558BEC"]
}
```
أعد بناء التطبيق أو ضع seed.json المحدّث في المسار المناسب.

---

## 🔧 استكشاف الأخطاء وإصلاحها

### VeraxCore لا يعمل
1. تأكد أنك تشغّله **كمسؤول (Administrator)**
2. تحقق أن جميع مكتبات Qt DLL موجودة في نفس المجلد
3. تحقق من توافق إصدار Windows (Windows 7 SP1+)
4. راجع `UserData/Logs/verax.log` لتفاصيل الأخطاء

### الفحص بطيء
1. استثنِ ملفات الوسائط الكبيرة من نطاق الفحص
2. استخدم الفحص السريع بدلاً من الفحص الكامل للفحوصات الروتينية
3. أغلق التطبيقات الأخرى التي تستخدم القرص بشكل مكثف
4. فكّر في فحص مجلدات محددة بدلاً من محركات أقراص كاملة

### الإصلاح لم ينجح
1. راجع `UserData/Logs/verax.log` لتفاصيل الإصلاح
2. ملف النسخة الاحتياطية `.bak` يجب أن يكون بجوار الملف الأصلي
3. جرّب الحجر الصحي ← الاستعادة كبديل
4. بعض الملفات المعدّلة بشكل مكثف قد لا تكون قابلة للإصلاح

### تم اكتشاف إنذار كاذب
1. تحقق من درجة الخطورة — الدرجات المنخفضة قد تكون إنذارات كاذبة
2. عدّل عتبة المحرك السلوكي في الإعدادات
3. أبلغ عن الإنذارات الكاذبة عبر GitHub Issues
4. يمكن إضافة هاش SHA-256 للملف في القائمة البيضاء

### أخطاء قاعدة البيانات
1. احذف `UserData/db/verax.sqlite` — سيُعاد إنشاؤه
2. الاحتياطي JSON (`verax_signatures.json`) سيُفعّل تلقائياً
3. شغّل تحديث التواقيع بعد إعادة تعيين قاعدة البيانات

---

## 📊 إحصائيات المشروع

| المقياس | القيمة |
|---|---|
| **لغة البرمجة** | C++17 |
| **إطار العمل** | Qt 5.15+ |
| **المحرك الأساسي** | 4,000+ سطر |
| **التواقيع المدمجة** | 65+ توقيع |
| **عائلات الفيروسات** | 25+ مدعومة |
| **أقسام الفيروسات** | 25+ مكتشفة |
| **الأقسام القياسية المحمية** | 35+ محمية |
| **محركات الكشف** | 4 مستقلة |
| **التشفير** | AES-256-CBC |
| **الرخصة** | GPLv3 |

---

## 🔒 الأمان

يرجى الاطلاع على [SECURITY.md](SECURITY.md) لسياسة الأمان وكيفية الإبلاغ عن الثغرات.

---

## 📜 الرخصة

هذا المشروع مرخص بموجب **رخصة جنو العمومية الإصدار 3.0** — انظر ملف [LICENSE](LICENSE) للتفاصيل.

---

## ⚠️ إخلاء المسؤولية

يرجى الاطلاع على [DISCLAIMER.md](DISCLAIMER.md) لإخلاء المسؤولية الكامل.

**هذا البرنامج مقدم "كما هو" بدون أي ضمان من أي نوع.** المؤلفون غير مسؤولين عن أي ضرر أو فقدان بيانات أو عدم استقرار في النظام ناتج عن استخدام هذا البرنامج. احرص دائماً على الاحتفاظ بنسخ احتياطية من الملفات المهمة.

---

## 🤝 المساهمة

نرحب بالمساهمات! يرجى الاطلاع على [CONTRIBUTING.md](CONTRIBUTING.md) للإرشادات.

---

## 👨‍💻 المطوّر

<p align="center">
  <strong>علي السكاف</strong><br/>
  <a href="https://alisakkaf.com">🌐 الموقع</a> •
  <a href="https://github.com/alisakkaf">💻 GitHub</a> •
  <a href="https://www.facebook.com/AliSakkaf.Dev">📘 Facebook</a>
</p>

---

## 🌟 تاريخ النجوم

إذا وجدت VeraxCore مفيداً، يرجى إعطاؤه نجمة ⭐ على GitHub!

---

### 💡 Support the Developer

<div align="center">
  <i>If you find my tools and projects useful, consider supporting my work. Your support helps keep these projects completely free!</i>
</div>

<br>

<div align="center">

| Crypto Asset | Network | Wallet Address (Copy) | Quick Scan |
| :--- | :--- | :--- | :---: |
| ![USDT](https://img.shields.io/badge/USDT-Tether-26A17B?style=for-the-badge&logo=tether&logoColor=white) | **TRC20** | `TYLBeDA5aGNcc3WkVqf3xWPHXmsZzs2p28` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=TYLBeDA5aGNcc3WkVqf3xWPHXmsZzs2p28" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![USDT](https://img.shields.io/badge/USDT-Tether-26A17B?style=for-the-badge&logo=tether&logoColor=white) | **BEP20** | `0x67cf27f33c80479ea96372810f9e2ee4c3b095c5` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=0x67cf27f33c80479ea96372810f9e2ee4c3b095c5" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![BTC](https://img.shields.io/badge/BTC-Bitcoin-F7931A?style=for-the-badge&logo=bitcoin&logoColor=white) | **Bitcoin** | `bc1q97dr37h37npzarmmrv0tjz2nm50htqc7pfpzj6` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=bitcoin:bc1q97dr37h37npzarmmrv0tjz2nm50htqc7pfpzj6" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![ETH](https://img.shields.io/badge/ETH-Ethereum-3C3C3D?style=for-the-badge&logo=ethereum&logoColor=white) | **ERC20** | `0x67cf27f33c80479ea96372810F9e2EE4C3b095C5` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=ethereum:0x67cf27f33c80479ea96372810F9e2EE4C3b095C5" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![SOL](https://img.shields.io/badge/SOL-Solana-9945FF?style=for-the-badge&logo=solana&logoColor=white) | **Solana** | `Cbesgr4tvo4T1inNMFe46GSym2qMYjkmofbXFc77rDNK` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=solana:Cbesgr4tvo4T1inNMFe46GSym2qMYjkmofbXFc77rDNK" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![USDC](https://img.shields.io/badge/USDC-USD_Coin-2775CA?style=for-the-badge&logo=usd-coin&logoColor=white) | **ERC20** | `0x67cf27f33c80479ea96372810f9e2ee4c3b095c5` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=0x67cf27f33c80479ea96372810f9e2ee4c3b095c5" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |
| ![USDC](https://img.shields.io/badge/USDC-USD_Coin-2775CA?style=for-the-badge&logo=usd-coin&logoColor=white) | **SPL** | `Cbesgr4tvo4T1inNMFe46GSym2qMYjkmofbXFc77rDNK` | <a href="https://api.qrserver.com/v1/create-qr-code/?size=300x300&margin=10&data=solana:Cbesgr4tvo4T1inNMFe46GSym2qMYjkmofbXFc77rDNK" target="_blank"><img src="https://img.shields.io/badge/Show_QR-Click_Here-black?style=flat-square&logo=qr-code" alt="QR"></a> |

---

<p align="center">
  <img src="docs/screenshots/logo.png" alt="VeraxCore" width="60"/><br/>
  <strong>VeraxCore Antivirus</strong> — حماية حقيقية. بدون ضجيج.<br/>
  حقوق النشر © 2026 VeraxCore. جميع الحقوق محفوظة.<br/>
  صُنع بـ ❤️ بواسطة علي السكاف
</p>

