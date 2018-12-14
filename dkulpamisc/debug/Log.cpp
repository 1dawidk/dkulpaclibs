#include <stdarg.h>
#include "Log.h"

vector<string> Log::msgs;
pthread_mutex_t Log::queueM;

void Log::init() {
    Log::queueM= PTHREAD_MUTEX_INITIALIZER;
    this->start();
}

void Log::write(const char* tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    char buff[512];
    vsprintf(buff, format, args);
    va_end(args);

    pthread_mutex_lock(&(Log::queueM));
    msgs.push_back(string(tag)+": "+string(buff));
    pthread_mutex_unlock(&(Log::queueM));
}

void Log::onStart() {

}

void Log::onRun() {
    pthread_mutex_lock(&(Log::queueM));
    if(!(Log::msgs).empty()){
        cout << msgs[msgs.size()-1] << endl;
        msgs.pop_back();
    }
    pthread_mutex_unlock(&(Log::queueM));
    Thread::pause(2);
}

void Log::onStop() {
    Log::msgs.clear();
}
