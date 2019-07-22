#ifndef MISC_CLOCK_H
#define MISC_CLOCK_H

#include <chrono>
#include <time.h>

using namespace std::chrono;

class Clock {
public:
    static long GetTimeMs();
    static long GetTimeS();
    static long GetTimeM();

    static int GetDayMinutes();

    static int getYear();
    static int getMonth();
    static int getDay();
    static int getHour();
    static int getMinutes();

};


#endif //MISC_CLOCK_H
