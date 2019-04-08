#ifndef GENCURVEPROGRESSDIALOG_H
#define GENCURVEPROGRESSDIALOG_H

#include <QDialog>
#include <QLabel>

class GenCurveProgressDialog:public QDialog
{
    Q_OBJECT
public:
    GenCurveProgressDialog(int sum, int left, QWidget* parent = nullptr);
public slots:
    void setData(int sum, int left);
private:
    QLabel* sumLabel;
    QLabel* leftLabel;
};

#endif // GENCURVEPROGRESSWINDOW_H
