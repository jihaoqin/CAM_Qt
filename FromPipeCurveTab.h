#ifndef FROMPIPECURVETAB_H
#define FROMPIPECURVETAB_H
#include <QWidget>

class QGroupBox;
class QLabel;
class QTextEdit;
class QSpinBox;
class QSlider;
class QDoubleSpinBox;
class QPushButton;
class TabBackground;

class FromPipeCurveTab:public QWidget
{
    Q_OBJECT
public:
    friend class GuiConnector;
    FromPipeCurveTab(TabBackground*, GuiConnector*, QWidget* parent = nullptr);
    QString getPointText();
    bool isPointTextFocused();
    void setPointText(QString);
    float getWindingAngle();
    void setBandId(QString);
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
    QSpinBox* dirSpinBox;
    QSlider* dirSlider;
    QLabel* lamLabel;
    QDoubleSpinBox* lamSpinBox;

    QPushButton* ok;
    QPushButton* cancle;

    bool focusOnPoint;
    QString bandId;

    GuiConnector* connector;
    TabBackground* back;
};

#endif // FROMPIPECURVETAB_H
