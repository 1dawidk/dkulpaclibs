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
    onStop();
    finished= true;
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

void Thread::pause(long ms) {
    int rs= (int)(ms/1000);
    int rms= (int)(ms%1000);

    struct timespec tp= {rs, rms*1000000};
    nanosleep(&tp, NULL);
}

void Thread::pauseUs(long us) {
    struct timespec tp= {0, us*1000};
    nanosleep(&tp, NULL);
}

void Thread::initMutex(Mutex &m) {
    m= PTHREAD_MUTEX_INITIALIZER;
}

void Thread::lockMutex(Mutex &m) {
    pthread_mutex_lock(&m);
}

void Thread::unlockMutex(Mutex &m) {
    pthread_mutex_unlock(&m);
}
