#pragma once

#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QLabel>
#include <mainwindow.h>
#include "TeePara.h"

class TeeParameterDialog: public QDialog
{
    Q_OBJECT

public:
    TeeParameterDialog(MainWindow* parent);
    TeePara getTeePara();
    float bandWidth();
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


    QLineEdit *widthLine;
    QLabel *widthLabel;

    QLabel *teePic;

    TeePara para;
    float m_width;
};

