#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <mainwindow.h>
struct TeePara{
    float mainLength;
    float branchLength;
    float R;
    float sideR;
};

class TeeParameterDialog: public QDialog
{
    Q_OBJECT

public:
    TeeParameterDialog(MainWindow* parent);
    TeePara getTeePara();
public slots:
    void readData();

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

    TeePara para;
};

