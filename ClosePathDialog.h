#ifndef CLOSEPATHDIALOG_H
#define CLOSEPATHDIALOG_H
#include <QDialog>
class QLabel;
class ClosePathDialog: public QDialog
{
    Q_OBJECT
public:
    ClosePathDialog(QWidget* parent = nullptr);
public slots:
    void setData(int, int, int, int, int, int);
private:
    QLabel* leftTotal;
    QLabel* leftLeft;

    QLabel* upTotal;
    QLabel* upLeft;

    QLabel* rightTotal;
    QLabel* rightLeft;
};

#endif // CLOSEPATHDIALOG_H
