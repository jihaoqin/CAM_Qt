#ifndef FROMPIPECURVETAB_H
#define FROMPIPECURVETAB_H
#include <QWidget>
#include "NameDeclare.h"
class QGroupBox;
class QLabel;
class QTextEdit;
class QSpinBox;
class QSlider;
class QDoubleSpinBox;
class QPushButton;
class TabBackground;
class QCheckBox;

class FromPipeCurveTab:public QWidget
{
    Q_OBJECT
public:
    friend class GuiConnector;
    FromPipeCurveTab(TabBackground*, GuiConnector*, QWidget* parent = nullptr);
    QString getPointText();
    QString getCBandId();
    QString getTCurveId();
    QString getTPointId();
    bool isPointTextFocused();
    void setPointText(QString);
    float getWindingAngle();
    void setCBandId(QString);
    void setTCurveId(QString);
    void setTPointId(QString);
    float getSlip();
public slots:
    void okPressed();
    void canclePressed();
    void updateBand();
protected:
    virtual bool eventFilter(QObject* target, QEvent* event) override;
private:
    void initial();
    QGroupBox* pointBox;
    QLabel* pointLabel;
    QTextEdit* pointText;

    QGroupBox* dirBox;
    QLabel* dirLabel;
    QCheckBox* oppositeBox;
    QSpinBox* dirSpinBox;
    QSlider* dirSlider;
    QLabel* lamLabel;
    QDoubleSpinBox* lamSpinBox;

    QPushButton* ok;
    QPushButton* cancle;

    bool focusOnPoint;
    QString cBandId;
    QString tCurveId;
    QString tPointId;

    GuiConnector* connector;
    TabBackground* back;
};

#endif // FROMPIPECURVETAB_H
