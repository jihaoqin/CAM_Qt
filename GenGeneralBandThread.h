#ifndef GENGENERALBANDTHREAD_H
#define GENGENERALBANDTHREAD_H
#include <QThread>

class Controller;

class GenGeneralBandThread:public QThread
{
    Q_OBJECT
public:
    GenGeneralBandThread(QString, Controller*);
signals:
    void progress(int sum, int left);
    void calOver();
protected:
    virtual void run() override;
private:
    QString which;
    Controller* ctrl;
};

#endif // GENGENERALBANDTHREAD_H
