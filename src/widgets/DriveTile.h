// DriveTile.h - selectable tile representing one drive/volume
// By Ali Sakkaf - https://alisakkaf.com
#pragma once
#include "SurfaceCard.h"
#include "../core/SystemEnum.h"
class QLabel;
class QCheckBox;

namespace verax {

class DriveTile : public SurfaceCard {
    Q_OBJECT
public:
    explicit DriveTile(const DriveInfo &d, QWidget *parent = nullptr);
    bool      isChecked() const;
    void      setChecked(bool v);
    QString   letter() const { return m_info.letter; }
    const DriveInfo& info() const { return m_info; }
    void      retranslate();

signals:
    void toggled(bool checked);

protected:
    void mousePressEvent(QMouseEvent *e) override;

private:
    DriveInfo  m_info;
    QLabel    *m_lblIcon    = nullptr;
    QLabel    *m_lblLetter  = nullptr;
    QLabel    *m_lblLabel   = nullptr;
    QLabel    *m_lblType    = nullptr;
    QLabel    *m_lblCapacity = nullptr;
    QCheckBox *m_check      = nullptr;
};

} // namespace verax
