#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <QThread>
#include <QMutex>

#include <dbmanager.h>

#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlQueryModel>

class mythread : public QThread
{
    Q_OBJECT

public:
    explicit mythread(QObject *parent = 0, bool b = false);
    void run();

    // if Stop = true, the thread will break
    // out of the loop, and will be disposed
    bool Stop;


signals:
    // To communicate with Gui Thread
    // we need to emit a signal
    void valueChanged();

public slots:

};

#endif // MYTHREAD_H
