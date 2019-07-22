#ifndef KOPYTKO_LOG_H
#define KOPYTKO_LOG_H

#include <iostream>
#include <vector>
#include <string>
#include "../Thread.h"

using namespace std;

class Log : Thread {
public:
    void init();
    static void write(const char* tag, const char *format, ...);
private:
    void onStart() override;
    void onRun() override;
    void onStop() override;

    static vector<string> msgs;
    static pthread_mutex_t queueM;
};


#endif //KOPYTKO_LOG_H
