#ifndef ENVELOPINIDIALOG_H
#define ENVELOPINIDIALOG_H

#include <QDialog>
#include "EnvelopData.h"

namespace Ui {
class EnvelopIniDialog;
}

class GuiConnector;

class EnvelopIniDialog : public QDialog
{
    Q_OBJECT

public:
    explicit EnvelopIniDialog(GuiConnector* , QWidget *parent = nullptr);
    ~EnvelopIniDialog();
public slots:
    void okPressed();
private:
    Ui::EnvelopIniDialog *ui;
    GuiConnector* connector;
    EnvelopData tempIni;
};

#endif // ENVELOPINIDIALOG_H
