#ifndef NEWCURVETAB_H
#define NEWCURVETAB_H
#include <QWidget>
class QLabel;
class QTextEdit;
class QGroupBox;
class QPushButton;
class GuiConnector;

class NewCurveTab:public QWidget
{
    Q_OBJECT

public:
    NewCurveTab(QWidget* parent = nullptr);
    virtual ~NewCurveTab();
    void setConnector(GuiConnector*);
    friend class GuiConnector;
protected:
    virtual bool eventFilter(QObject* target, QEvent* event) override;
private:
    QGroupBox* pointBox;
    QLabel* pointLabel;
    QTextEdit* pointText;

    QGroupBox* dirBox;
    QLabel* dirLabel;
    QTextEdit* dirText;

    QPushButton* ok;
    QPushButton* cancle;
    GuiConnector* connector;
};

#endif // NEWCURVETAB_H
