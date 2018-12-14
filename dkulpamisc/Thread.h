#ifndef MISC_THREAD_H
#define MISC_THREAD_H

#include <pthread.h>
#include <sys/signal.h>
#include <vector>
#include "Clock.h"

class Thread {
public:
    virtual void start();
    virtual void start(int id);
    bool isRunning();
    void stop();
    void kill();
    int getId();

protected:
    virtual void onStart()=0;
    virtual void onRun()=0;
    virtual void onStop()=0;

    bool runThread;
    pthread_t threadHandle;
    pthread_mutex_t runMu;

    static void pause(int ms);
private:
    bool finished;
    int id;

    static void* threadExec(void *context);
    static std::vector<Thread*> threads;
    static Thread* findThreadById(int id);
    static int findThreadPosById(int id);
};


#endif //MISC_THREAD_H
