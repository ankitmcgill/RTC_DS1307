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

#ifndef _RTC_DS1307_H_
#define _RTC_DS1307_H_

#if defined(ESP8266)
  #include "osapi.h"
  #include "mem.h"
  #include "ets_sys.h"
  #include "ip_addr.h"
  #include "espconn.h"
  #include "os_type.h"

  #define PRINTF      os_printf
  #define PUTINFLASH  ICACHE_FLASH_ATTR
#endif

#define RTC_DS1307_ADDRESS  0xD0

//CUSTOM VARIABLE STRUCTURES/////////////////////////////
//END CUSTOM VARIABLE STRUCTURES/////////////////////////

//FUNCTION PROTOTYPES/////////////////////////////////////
//CONFIGURATION FUNCTIONS
void PUTINFLASH RTC_DS1307_SetDebug(uint8_t debug_on);
void PUTINFLASH RTC_DS1307_SetI2CFunctions(void (*i2c_init)(uint8_t),
                                            void (*i2c_writebyte)(uint8_t, uint8_t),
                                            void (*i2c_writebytemultiple)(uint8_t, uint8_t*, uint8_t),
                                            uint8_t (*i2c_readbyte)(uint8_t),
                                            void (*i2c_readbytemultiple)(uint8_t, uint8_t*, uint8_t)
                                          );
void PUTINFLASH RTC_DS1307_Initialize(void);


//GET PARAMETERS FUNCTIONS


//CONTROL FUNCTIONS

//INTERNAL FUNCTIONS

//END FUNCTION PROTOTYPES/////////////////////////////////
#endif
