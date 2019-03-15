#ifndef NEWCURVETAB_H
#define NEWCURVETAB_H
#include <QWidget>
class QSlider;
class QLabel;
class QTextEdit;
class QGroupBox;
class QPushButton;
class GuiConnector;
class QSpinBox;

class NewCurveTab:public QWidget
{
    Q_OBJECT

public:
    NewCurveTab(QWidget* parent = nullptr);
    virtual ~NewCurveTab();
    void setConnector(GuiConnector*);
    QString getPointText();
    void setPointText(QString);
    void setCurveId(QString);
    int getWindingAngle();
    bool isPointTextFocused();
    friend class GuiConnector;
public slots:
    void updateCurve(int);
protected:
    virtual bool eventFilter(QObject* target, QEvent* event) override;
private:
    QGroupBox* pointBox;
    QLabel* pointLabel;
    QTextEdit* pointText;

    QGroupBox* dirBox;
    QLabel* dirLabel;
    QSpinBox* dirSpinBox;
    QSlider* dirSlider;

    QPushButton* ok;
    QPushButton* cancle;

    GuiConnector* connector;

    bool focusOnPoint;
    QString curveId;
};

#endif // NEWCURVETAB_H
