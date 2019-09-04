#ifndef THREAD_GET_FS_VALUE_H
#define THREAD_GET_FS_VALUE_H

#include <QThread>
#include "global_functions.h"


class thread_get_fs_value : public QThread
{
    Q_OBJECT

public:
    explicit thread_get_fs_value(QObject *parent = 0, bool b = false);
    void run();

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;


signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void have_fsVal(float ,
                    float,
                    float ,
                    double ,
                    double ,
                    short,
                    qint8,
                    int,
                    float,
                    double,
                    short,
                    short,
                    short,
                    short,
                    qint8 ,
                    qint8 ,
                    qint8,
                    qint8,
                    qint8,
                    short,
                    short,
                    int,
                    int,
                    float,
                    float,
                    double,
                    double,
                    double,
                    double );

public slots:

private:

    global_function *g_function;


};

#endif // THREAD_GET_FS_VALUE_H
