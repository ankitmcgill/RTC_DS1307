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
  #include "mem.h"

  #define PRINTF      os_printf
  #define PUTINFLASH  ICACHE_FLASH_ATTR
#endif

#define RTC_DS1307_I2C_ADDRESS        0x68
#define RTC_DS1307_REGISTER_NVRAM_LEN 56

//DS1307 REGISTER MAP
#define RTC_DS1307_REGISTER_SECONDS   0x00
#define RTC_DS1307_REGISTER_MINUTES   0x01
#define RTC_DS1307_REGISTER_HOURS     0x02
#define RTC_DS1307_REGISTER_DAY       0x03
#define RTC_DS1307_REGISTER_DATE      0x04
#define RTC_DS1307_REGISTER_MONTH     0x05
#define RTC_DS1307_REGISTER_YEAR      0x06
#define RTC_DS1307_REGISTER_CONTROL   0x07
#define RTC_DS1307_REGISTER_NVRAM     0x08

//DS1307 CONTROL REGISTER OPTIONS DEFINE
#define RTC_DS1307_CONTROL_OUT              (1 << 7)
#define RTC_DS1307_CONTROL_SQWE             (1 << 4)
#define RTC_DS1307_CONTROL_SW_FREQ_1HZ      (0)
#define RTC_DS1307_CONTROL_SW_FREQ_4096HZ   (1 << 0)
#define RTC_DS1307_CONTROL_SW_FREQ_8192HZ   (1 << 1)
#define RTC_DS1307_CONTROL_SW_FREQ_32768HZ  ((1 << 0) | (1 << 1))

//DS1307 TIME REGISTER OPERATIONS
#define RTC_DS1307_SECONDS_GET_CH_BIT(x)    ((x & 0x80) >> 7)
#define RTC_DS1307_SECONDS_SET_CH_BIT(x)    (x | 0x80)
#define RTC_DS1307_SECONDS_GET_DIG0(x)      (x & 0x0F)
#define RTC_DS1307_SECONDS_GET_DIG1(x)      ((x & 0x70) >> 4)
#define RTC_DS1307_SECONDS_SET_DIG0(x,y)    (x | y)
#define RTC_DS1307_SECONDS_SET_DIG1(x,y)    (x | (y << 4))

#define RTC_DS1307_MINUTES_GET_DIG0(x)      (x & 0x0F)
#define RTC_DS1307_MINUTES_GET_DIG1(x)      ((x & 0xF0) >> 4)
#define RTC_DS1307_MINUTES_SET_DIG0(x,y)      (x | y)
#define RTC_DS1307_MINUTES_SET_DIG1(x,y)      (x | (y << 4))

#define RTC_DS1307_HOURS_GET_1224_BIT(x)    ((x & 0x40) >> 6)
#define RTC_DS1307_HOURS_SET12(x)           (x | 0x40)
#define RTC_DS1307_HOURS_SET24(x)           (x & ~(1 << 6))
#define RTC_DS1307_HOURS_GET_AMPM_BIT(x)    ((x & 0x20) >> 5)
#define RTC_DS1307_HOURS_SETAM(x)           (x & ~(1 << 5))
#define RTC_DS1307_HOURS_SETPM(x)           (x | (1 << 5))
#define RTC_DS1307_HOURS_12_GET_DIG0(x)     (x & 0x0F)
#define RTC_DS1307_HOURS_12_GET_DIG1(x)     ((x & 0x10) >> 4)
#define RTC_DS1307_HOURS_24_GET_DIG0(x)     (x & 0x0F)
#define RTC_DS1307_HOURS_24_GET_DIG1(x)     ((x & 0x30) >> 4)
#define RTC_DS1307_HOURS_SET_DIG0(x, y)     (x | y)
#define RTC_DS1307_HOURS_SET_DIG1(x, y)     (x | (y << 4))

#define RTC_DS1307_DAY_GET(x)               (x & 0x07)
#define RTC_DS1307_DAY_SET(x,y)             (x | y)

#define RTC_DS1307_DATE_GET_DIG0(x)         (x & 0x0F)
#define RTC_DS1307_DATE_GET_DIG1(x)         ((x & 0x30) >> 4)
#define RTC_DS1307_DATE_SET_DIG0(x,y)       (x | y)
#define RTC_DS1307_DATE_SET_DIG1(x,y)       (x | (y << 4))

#define RTC_DS1307_MONTH_GET_DIG0(x)        (x & 0x0F)
#define RTC_DS1307_MONTH_GET_DIG1(x)        ((x & 0x10) >> 4)
#define RTC_DS1307_MONTH_SET_DIG0(x,y)      (x | y)
#define RTC_DS1307_MONTH_SET_DIG1(x,y)      (x | (y << 4))

#define RTC_DS1307_YEAR_GET_DIG0(x)         (x & 0x0F)
#define RTC_DS1307_YEAR_GET_DIG1(x)         ((x & 0xF0) >> 4)
#define RTC_DS1307_YEAR_SET_DIG0(x,y)       (x | y)
#define RTC_DS1307_YEAR_SET_DIG1(x,y)       (x | (y << 4))

#define RTC_DS1307_CONTROL_GET_OUT(x)       ((x & 0x80) >> 7)
#define RTC_DS1307_CONTROL_SET_OUT(x)       (x | 0x80)
#define RTC_DS1307_CONTROL_GET_SQWE(x)      ((x & 0x10) >> 4)
#define RTC_DS1307_CONTROL_SET_SQWE(x)      (x | 0x10)
#define RTC_DS1307_CONTROL_GET_RS(x)        (x & 0x03)
#define RTC_DS1307_CONTROL_SET_RS(x,y)      (x | y)

//CUSTOM VARIABLE STRUCTURES/////////////////////////////
typedef enum
{
    sqw_1hz = 0,
    sqw_4096hz,
    sqw_8192hz,
    sqw_32768hz
} RTC_DS1307_SQW_FREQ;

typedef struct
{
    uint8_t out;
    uint8_t sqwe;
    RTC_DS1307_SQW_FREQ freq;
} RTC_DS1307_CONTROL_CONFIG;

typedef enum
{
    timeformat_24 = 0,
    timeformat_12
} RTC_DS1307_TIMEFORMAT_CONFIG;

typedef enum
{
    timeformat_am = 0,
    timeformat_pm
} RTC_DS1307_TIMEFORMAT_AMPM;

typedef enum
{
    sunday = 0,
    monday,
    tuesday,
    wednesday,
    thrursday,
    friday,
    saturday
}RTC_DS1307_DAYFORMAT_CONFIG;

typedef struct
{
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  RTC_DS1307_TIMEFORMAT_CONFIG hour_format;
  RTC_DS1307_TIMEFORMAT_AMPM am_pm;
  RTC_DS1307_DAYFORMAT_CONFIG day;
  uint8_t date;
  uint8_t month;
  uint16_t year;
} RTC_DS1307_TIME;
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


//GET PARAMETERS FUNCTIONS
uint8_t PUTINFLASH RTC_DS1307_GetControlRegister(void);
RTC_DS1307_TIME*  PUTINFLASH RTC_DS1307_GetTime(void);

//SET PARAMETERS FUNCTIONS
void PUTINFLASH RTC_DS1307_SetControlRegister(RTC_DS1307_CONTROL_CONFIG control);
void PUTINFLASH RTC_DS1307_SetTimeFormat(RTC_DS1307_TIMEFORMAT_CONFIG format);
void PUTINFLASH RTC_DS1307_SetTime(RTC_DS1307_TIME time);

//CONTROL FUNCTIONS
void PUTINFLASH RTC_DS1307_StartOcillator(void);
void PUTINFLASH RTC_DS1307_StopOscillator(void);
void PUTINFLASH RTC_DS1307_Initialize(void);

//INTERNAL FUNCTIONS
uint8_t PUTINFLASH _rtc_ds1307_verify_ch_bit(void);
//END FUNCTION PROTOTYPES/////////////////////////////////
#endif
