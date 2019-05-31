#include "qthread_controller/qthreadworker.h"

void QThreadController::QThreadWorker::doWork(void* data1, void* data2) {
    throw "doWork is a virtual function.";
}
