#ifndef MISC_THREAD_H
#define MISC_THREAD_H

#include <pthread.h>
#include <sys/signal.h>
#include <vector>
#include "Clock.h"


typedef pthread_mutex_t Mutex;

class Thread {
public:
    virtual void start();
    virtual void start(int id);
    bool isRunning();
    void stop();
    void kill();
    int getId();

    static void initMutex(Mutex &m);
    static void lockMutex(Mutex &m);
    static void unlockMutex(Mutex &m);

protected:
    virtual void onStart()=0;
    virtual void onRun()=0;
    virtual void onStop()=0;

    bool runThread;
    pthread_t threadHandle;
    pthread_mutex_t runMu;

    static void pause(long ms);
    static void pauseUs(long us);

private:
    bool finished;
    int id;

    static void* threadExec(void *context);
    static std::vector<Thread*> threads;
    static Thread* findThreadById(int id);
    static int findThreadPosById(int id);
};




#endif //MISC_THREAD_H
