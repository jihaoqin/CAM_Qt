#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <mainwindow.h>
class TeeParameterDialog: public QDialog
{
    Q_OBJECT

public:
    TeeParameterDialog(MainWindow* parent);

public slots:
    void readData();
signals:
    void addTee(float, float, float, float);

private:
    bool isValid(float ,float ,float , float);
    QPushButton *okButton;
    QPushButton *cancelButton;

    QLineEdit *mainLengthLine;
    QLabel *mainLengthLabel;

    QLineEdit *branchLengthLine;
    QLabel *branchLengthLabel;

    QLineEdit *sideRLine;
    QLabel *sideRLabel;

    QLineEdit *RLine;
    QLabel *RLabel;

    QLabel *teePic;
};

