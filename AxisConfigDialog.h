#ifndef AXISCONFIGDIALOG_H
#define AXISCONFIGDIALOG_H

#include <QDialog>
#include "AxisIni.h"

class AxisIni;
namespace Ui {
class AxisConfigDialog;
}

class AxisConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AxisConfigDialog(AxisIni*, QWidget *parent = nullptr);
    void updateUI();
    ~AxisConfigDialog();
public slots:
    void spindleLeftCheckbox(int);
    void xLeftCheckbox(int);
    void zDownCheckbox(int);
    void flipDownCheckbox(int);
    void yawLeftCheckbox(int);
    void setAxisConfig(int);
    void saveIni();
    void updateAxis();
private:
    void initial();
private:
    Ui::AxisConfigDialog *ui;
    AxisIni* axisData;
    AxisIni* axis;
};

#endif // AXISCONFIGDIALOG_H
