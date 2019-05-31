#include "qthread_controller/qthreadcontroller.h"
#include "qthread_controller/qthreadworker.h"

QThreadController::ThreadController::ThreadController(QThreadWorker* instance, HandleResultsCallback_t* _callback){
    worker = instance;
    callback = _callback;
    worker->moveToThread(&workerThread);
    //connect(&workerThread, &QThread::finished, worker, &QObject::deleteLater);
    connect(this, &ThreadController::Operate, worker, &QThreadWorker::doWork);
    connect(worker, &QThreadWorker::ResultReady, this, &ThreadController::HandleResults);
    workerThread.start();
}

QThreadController::ThreadController::~ThreadController() {
    workerThread.quit();
    workerThread.wait();
}

void QThreadController::ThreadController::StartOperate(void* data1, void* data2){
    emit Operate(data1, data2);
}

void QThreadController::ThreadController::HandleResults(void* data){
    callback(data);
}

void QThreadController::ThreadController::StopOperate(){
    workerThread.quit();
    workerThread.wait();
}
