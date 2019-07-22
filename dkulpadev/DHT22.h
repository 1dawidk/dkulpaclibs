#ifndef MIOGIAPICCO_DHT22_H
#define MIOGIAPICCO_DHT22_H


#include <cstdint>
#include "dkulpaclibs/misc/Thread.h"
#include "iostream"

#define DHT11_READSTATE_BEFORELOW               0
#define DHT11_READSTATE_AFTERLOW                1
#define DHT11_READSTATE_INITSTATE_BEFORELOW     2
#define DHT11_READSTATE_INITSTATE_AFTERLOW      3
#define MAXTIMINGS	85

using namespace std;

class DHT22 : public Thread {
public:
    DHT22(uint8_t dataPin);

    float getTemperature();
    float getHumidity();

protected:
    void onStart() override;

    void onRun() override;

    void onStop() override;

private:
    uint8_t  dataPin;
    float temp;
    float hum;

    pthread_mutex_t readMutex;
};


#endif //MIOGIAPICCO_DHT11_H
