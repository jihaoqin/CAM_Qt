#ifndef CLOSEPATHTHREAD_H
#define CLOSEPATHTHREAD_H
#include <QThread>

class Controller;

class ClosePathThread:public QThread
{
    Q_OBJECT
public:
    ClosePathThread(Controller* c);
signals:
    void progress(int, int, int, int, int, int);
    void calOver();
protected:
    virtual void run() override;
private:
    Controller* ctrl;
};

#endif // CLOSEPATHTHREAD_H
