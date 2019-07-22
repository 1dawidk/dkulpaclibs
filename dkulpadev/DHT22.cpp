#include <bcm2835.h>
#include "DHT22.h"

DHT22::DHT22(uint8_t dataPin) {
    this->dataPin= dataPin;
}

void DHT22::onStart() {
    readMutex= PTHREAD_MUTEX_INITIALIZER;
    bcm2835_gpio_fsel(dataPin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(dataPin, HIGH);
}

void DHT22::onRun() {
    int dht11_dat[5] = { 0, 0, 0, 0, 0 };

    uint8_t laststate	= HIGH;
    uint8_t counter		= 0;
    uint8_t j		= 0, i;
    float	f; /* fahrenheit */

    dht11_dat[0] = dht11_dat[1] = dht11_dat[2] = dht11_dat[3] = dht11_dat[4] = 0;

    /* pull pin down for 18 milliseconds */
    bcm2835_gpio_fsel(dataPin, BCM2835_GPIO_FSEL_OUTP);
    bcm2835_gpio_write(dataPin, LOW);
    delay( 18 );
    /* then pull it up for 40 microseconds */
    bcm2835_gpio_write(dataPin, HIGH);
    delayMicroseconds( 40 );
    /* prepare to read the pin */
    bcm2835_gpio_fsel(dataPin, BCM2835_GPIO_FSEL_INPT);
    bcm2835_gpio_set_pud(dataPin, BCM2835_GPIO_PUD_OFF);


    /* detect change and read data */
    for ( i = 0; i < MAXTIMINGS; i++ )
    {
        counter = 0;
        while ( bcm2835_gpio_lev(dataPin) == laststate )
        {
            counter++;
            delayMicroseconds( 1 );
            if ( counter == 255 )
            {
                break;
            }
        }
        laststate = bcm2835_gpio_lev(dataPin);

        if ( counter == 255 )
            break;

        /* ignore first 3 transitions */
        if ( (i >= 4) && (i % 2 == 0) )
        {
            /* shove each bit into the storage bytes */
            dht11_dat[j / 8] <<= 1;
            if ( counter > 30 )
                dht11_dat[j / 8] |= 1;
            j++;
        }
    }

    /*
     * check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
     * print it out if data is good
     */

    if ( (j >= 40) && (dht11_dat[4] == ( (dht11_dat[0] + dht11_dat[1] + dht11_dat[2] + dht11_dat[3]) & 0xFF) ) )
    {
        uint16_t humTmp= ((uint16_t)dht11_dat[0]<<8) | (uint16_t)dht11_dat[1];
        uint16_t tempTmp= ((uint16_t)dht11_dat[2]<<8) | (uint16_t)dht11_dat[3];

        pthread_mutex_lock(&readMutex);
        hum= (float)humTmp/10;
        temp= (float)tempTmp/10;
        pthread_mutex_unlock(&readMutex);

    }else  {
    }

    Thread::pause(5000);

}

void DHT22::onStop() {

}

float DHT22::getTemperature() {
    float t;

    pthread_mutex_lock(&readMutex);
    t= temp;
    pthread_mutex_unlock(&readMutex);

    return t;
}

float DHT22::getHumidity(){
    float h;

    pthread_mutex_lock(&readMutex);
    h= hum;
    pthread_mutex_unlock(&readMutex);

    return h;
}
