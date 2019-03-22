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
class QDoubleSpinBox;
class TabBackground;

class NewCurveTab:public QWidget
{
    Q_OBJECT

public:
    friend class GuiConnector;
    NewCurveTab(QWidget* parent = nullptr);
    virtual ~NewCurveTab();
    void setConnector(GuiConnector*);
    void setTabBack(TabBackground*);
    QString getPointText();
    void setPointText(QString);
    void setCurveId(QString);
    int getWindingAngle();
    bool isPointTextFocused();
public slots:
    void updateCurve();
    void okPressed();
    void canclePressed();
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
    QLabel* lamLabel;
    QDoubleSpinBox* lamSpinBox;

    QPushButton* ok;
    QPushButton* cancle;


    GuiConnector* connector;
    TabBackground* back;

    bool focusOnPoint;
    QString curveId;
};

#endif // NEWCURVETAB_H
