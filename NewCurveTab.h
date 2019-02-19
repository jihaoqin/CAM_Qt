#ifndef NEWCURVETAB_H
#define NEWCURVETAB_H
#include <QWidget>
class QLabel;
class QTextEdit;
class QGroupBox;

class NewCurveTab: QWidget
{
    Q_OBJECT

public:
    NewCurveTab(QWidget* parent = nullptr);
private:
    QGroupBox* pointBox;
    QLabel* pointLabel;
    QTextEdit* pointText;
    QGroupBox* dirBox;
    QLabel* dirLabel;
    QTextEdit* dirText;
};

#endif // NEWCURVETAB_H
