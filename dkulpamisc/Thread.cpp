#include "Thread.h"


std::vector<Thread*> Thread::threads;

void Thread::start() {
    runMu= PTHREAD_MUTEX_INITIALIZER;
    this->finished=false;
    id=threads.size();
    threads.push_back(this);
    this->runThread=true;
    pthread_create(&threadHandle, nullptr, &Thread::threadExec, this);
}

void Thread::start(int id){
    runMu= PTHREAD_MUTEX_INITIALIZER;
    this->finished= false;
    this->id= id;
    threads.push_back(this);
    this->runThread=true;
    pthread_create(&threadHandle, nullptr, &Thread::threadExec, this);
}

void Thread::stop(){
    pthread_mutex_lock(&runMu);
    this->runThread=false;
    pthread_mutex_unlock(&runMu);
    pthread_join(threadHandle, nullptr);
}

bool Thread::isRunning() {
    bool run;
    pthread_mutex_lock(&runMu);
    run= runThread;
    pthread_mutex_unlock(&runMu);

    return run;
}

void* Thread::threadExec(void *context) {
    ((Thread*)context)->onStart();
    while( ((Thread*)context)->isRunning()) {
        ((Thread *) context)->onRun();
    }
    ((Thread*)context)->onStop();
    ((Thread*)context)->finished= true;

    return 0;
}

void Thread::kill() {
    pthread_kill(threadHandle,SIGKILL);
}

int Thread::getId() {
    return id;
}

Thread* Thread::findThreadById(int id) {
    for(int i=0; i<Thread::threads.size(); i++){
        if(Thread::threads[i]->getId()==id)
            return Thread::threads[i];
    }

    return nullptr;
}

int Thread::findThreadPosById(int id) {
    for(int i=0; i<Thread::threads.size(); i++){
        if(Thread::threads[i]->getId()==id)
            return Thread::threads[i]->getId();
    }

    return -1;
}

void Thread::pause(int ms) {
    struct timespec tp= {0, ms*1000};
    nanosleep(&tp, NULL);
}
