#ifndef QTHREADWORKER_CONTROLLER_H
#define QTHREADWORKER_CONTROLLER_H
#include <QObject>
#include <QThread>
#include "qthread_controller/qthreadworker.h"

namespace QThreadController{

typedef void ( HandleResultsCallback_t )( void* user_data );

class ThreadController : public QObject
{
    Q_OBJECT
    QThread workerThread;
    HandleResultsCallback_t* callback;
public:
    ThreadController(QThreadWorker* instance, HandleResultsCallback_t* callback);
    ~ThreadController();
    void StartOperate(void* data);
public slots:
    void HandleResults(void* result);
signals:
    void Operate(void* data);
private:
    QThreadWorker* worker;
};
}

#endif // QTHREADWORKER_CONTROLLER_H
