/****************************************************************
* DS1307 RTC LIBRARY
* I2C BASED
*
* I2C READ ADDRESS  : 0xD1
* I2C WRITE ADDRESS : 0xD0
*
* AUGUST 18 2017
*
* ANKIT BHATNAGAR
* ANKIT.BHATNAGARINDIA@GMAIL.COM
*
* REFERENCES
*
****************************************************************/

#include "RTC_DS1307.h"

//LOCAL LIBRARY VARIABLES/////////////////////////////////////
//DEBUG RELATRED
static uint8_t _rtc_ds1307_debug;

//I2C FUNCTION POINTERS
static void (*_rtc_ds1307_i2c_init)(uint8_t);
static void (*_rtc_ds1307_i2c_writebyte)(uint8_t, uint8_t);
static void (*_rtc_ds1307_i2c_writebyte_multiple)(uint8_t, uint8_t*, uint8_t);
static uint8_t (*_rtc_ds1307_i2c_readbyte)(uint8_t);
static void (*_rtc_ds1307_i2c_readbyte_multiple)(uint8_t, uint8_t*, uint8_t);

void PUTINFLASH RTC_DS1307_SetDebug(uint8_t debug_on)
{
    //SET DEBUG PRINTF ON(1) OR OFF(0)

    _rtc_ds1307_debug = debug_on;
}

void PUTINFLASH RTC_DS1307_SetI2CFunctions(void (*i2c_init)(uint8_t),
                                            void (*i2c_writebyte)(uint8_t, uint8_t),
                                            void (*i2c_writebytemultiple)(uint8_t, uint8_t*, uint8_t),
                                            uint8_t (*i2c_readbyte)(uint8_t),
                                            void (*i2c_readbytemultiple)(uint8_t, uint8_t*, uint8_t)
                                          )
{
    //SET I2C DATA TRANSFER FUNCTIONS POINTERS

    _rtc_ds1307_i2c_init = i2c_init;
    _rtc_ds1307_i2c_writebyte = i2c_writebyte;
    _rtc_ds1307_i2c_writebyte_multiple = i2c_writebytemultiple;
    _rtc_ds1307_i2c_readbyte = i2c_readbyte;
    _rtc_ds1307_i2c_readbyte_multiple = i2c_readbytemultiple;

    if(_rtc_ds1307_debug)
    {
        PRINTF("RTC : DS1307 : i2c operation functions set\n");
    }
}

void PUTINFLASH RTC_DS1307_Initialize(void)
{
    //INTIALIZE DS1307 RTC


}
