
#include "Clock.h"

long Clock::GetTimeMs() {
    milliseconds now=
            duration_cast<milliseconds>(system_clock::now().time_since_epoch());

       return now.count();
}

long Clock::GetTimeS() {
    seconds now=
            duration_cast<seconds>(system_clock::now().time_since_epoch());

    return now.count();
}

long Clock::GetTimeM() {
    minutes now=
            duration_cast<minutes>(system_clock::now().time_since_epoch());

    return now.count();
}

int Clock::GetDayMinutes() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_hour*60 + aTime->tm_min;
}

int Clock::getYear() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_year + 1900;
}

int Clock::getMonth() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_mon+1;
}

int Clock::getDay() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_mday;
}

int Clock::getHour() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_hour;
}

int Clock::getMinutes() {
    time_t theTime= time(NULL);
    struct tm *aTime= localtime(&theTime);

    return aTime->tm_min;
}
