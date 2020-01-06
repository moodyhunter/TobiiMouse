#include "qthread_controller/qthreadworker.h"

void QThreadController::QThreadWorker::doWork(void *data1)
{
    Q_UNUSED(data1)
    throw "doWork is a virtual function.";
}
