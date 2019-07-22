#include "HD44780.h"

HD44780::HD44780(uint8_t h, uint8_t w, uint8_t bus, I2C *i2c_h, uint8_t adr) {
    this->busType= bus;
    this->w= w;
    this->h= h;
    this->i2c_h= i2c_h;
    this->adr= adr;
}

void HD44780::init() {

    blState= HD44780_BL_ON;

    writeCmd(0x03);     //Init
    writeCmd(0x03);     //Init
    writeCmd(0x03);     //Init
    writeCmd(0x02);     //Init

    writeCmd(0x28);     //Function_set | 2 lines + 5x8 dots + 4 bit mode
    writeCmd(0x0C);     //Display control | display on
    writeCmd(0x01);     //Clear display
    writeCmd(0x06);     //Entry mode | entry left

    delay(200);
}

void HD44780::write(string s, uint8_t line) {
    if(line==0)
        writeCmd(0x80);
    else
        writeCmd(0xC0);

    for(int i=0; i<s.length(); i++){
        writeData((uint8_t)s[i]);
    }
}

void HD44780::writexy(uint8_t x, uint8_t y, string s) {
    if(y==0)
        writeCmd((uint8_t)0x80+x);
    else
        writeCmd((uint8_t)0xC0+x);

    for(int i=0; i<s.length(); i++){
        writeData((uint8_t)s[i]);
    }
}

void HD44780::clrscr() {
    writeCmd(0x01);
}

void HD44780::writeCmd(uint8_t cmd) {
    if(busType==HD44780_BUS_GPIO)
        writeGPIOCmd();
    else
        writeI2C(cmd, 0);
}

void HD44780::writeData(uint8_t data) {
    if(busType==HD44780_BUS_GPIO)
        writeGPIOData();
    else
        writeI2C((uint8_t )data, HD44780_RS_BIT);
}

void HD44780::writeI2C(uint8_t data, uint8_t mode) {
    uint8_t bitsHigh= (uint8_t)(data&0xF0);
    uint8_t bitsLow= (uint8_t)( (data&0x0F)<<4 );

    writeI2C_fourBits(mode | bitsHigh);
    writeI2C_fourBits(mode | bitsLow);
}

void HD44780::writeI2C_fourBits(uint8_t data) {
    i2c_h->write(adr, data | blState);
    writeI2C_strobe(data | blState);
}

void HD44780::writeI2C_strobe(uint8_t data) {
    delayMicroseconds(200);
    i2c_h->write(adr, (uint8_t)(data | HD44780_EN_BIT));
    delayMicroseconds(200);
    i2c_h->write(adr, (uint8_t)(data & ~HD44780_EN_BIT));
    delayMicroseconds(200);
}

void HD44780::writeGPIOCmd() {

}

void HD44780::writeGPIOData() {

}