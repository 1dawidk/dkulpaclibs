#ifndef MIOGIAPICCO_HD44780_H
#define MIOGIAPICCO_HD44780_H

#include <cstdint>
#include "bcm2835.h"
#include "string"
#include "../Peripherals/I2C.h"

using namespace std;

#define HD44780_BUS_GPIO    0
#define HD44780_BUS_I2C     1

#define HD44780_EN_BIT      0x04
#define HD44780_RW_BIT      0x02
#define HD44780_RS_BIT      0x01

#define HD44780_BL_ON       0x08
#define HDD4780_BL_OFF      0x00

class HD44780 {
public:
    HD44780(uint8_t h, uint8_t w, uint8_t bus, I2C *i2c_h=NULL, uint8_t adr=0);

    void init();
    void write(string s, uint8_t line);
    void writexy(uint8_t x, uint8_t y, string s);
    void clrscr();

private:
    void writeCmd(uint8_t cmd);
    void writeData(uint8_t data);

    void writeI2C(uint8_t data, uint8_t mode);
    void writeI2C_fourBits(uint8_t data);
    void writeI2C_strobe(uint8_t data);

    void writeGPIOCmd();
    void writeGPIOData();

    uint8_t busType;
    uint8_t adr;
    I2C *i2c_h;
    uint8_t h;
    uint8_t w;
    uint8_t blState;

};


#endif //MIOGIAPICCO_HD44780_H
