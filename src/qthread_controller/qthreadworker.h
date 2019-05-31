#ifndef QTHREADWORKER_H
#define QTHREADWORKER_H
#include <QObject>
#include <QThread>

namespace QThreadController{
class QThreadWorker : public QObject
{
    Q_OBJECT

public slots:
    virtual void doWork(void* data1, void* data2);

signals:
    void ResultReady(void* result);
};
}
#endif // QTHREADWORKER_H
