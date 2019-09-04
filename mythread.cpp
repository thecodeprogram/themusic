#include "mythread.h"
#include <QDebug>

mythread::mythread(QObject *parent, bool b) :
    QThread(parent), Stop(b)
{
}

// run() will be called when a thread starts
void mythread::run()
{

     while(1)
    {


        emit valueChanged( );



        // slowdown the count change, msec
        this->msleep(30);
    }
}
