#ifndef OPENBANDTHREAD_H
#define OPENBANDTHREAD_H
#include <QThread>

class Controller;

class OpenBandThread:public QThread
{
    Q_OBJECT
public:
    OpenBandThread(QString p, Controller* c);
signals:
    void progress(int sum, int left);
    void calOver();
protected:
    virtual void run() override;
private:
    Controller* ctrl;
    QString path;
};

#endif // WORKINGTHREAD_H
