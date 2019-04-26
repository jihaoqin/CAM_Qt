#ifndef AXISCONFIGDIALOG_H
#define AXISCONFIGDIALOG_H

#include <QDialog>

namespace Ui {
class AxisConfigDialog;
}

class AxisConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AxisConfigDialog(QWidget *parent = nullptr);
    ~AxisConfigDialog();
private:
    void initial();

private:
    Ui::AxisConfigDialog *ui;
};

#endif // AXISCONFIGDIALOG_H
