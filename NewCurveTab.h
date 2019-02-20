#ifndef NEWCURVETAB_H
#define NEWCURVETAB_H
#include <QWidget>
class QLabel;
class QTextEdit;
class QGroupBox;
class QPushButton;

class NewCurveTab:public QWidget
{
    Q_OBJECT

public:
    NewCurveTab(QWidget* parent = nullptr);
    virtual ~NewCurveTab();
private:
    QGroupBox* pointBox;
    QLabel* pointLabel;
    QTextEdit* pointText;

    QGroupBox* dirBox;
    QLabel* dirLabel;
    QTextEdit* dirText;

    QPushButton* ok;
    QPushButton* cancle;
};

#endif // NEWCURVETAB_H
