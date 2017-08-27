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
static void (*_rtc_ds1307_i2c_init)(void);
static void (*_rtc_ds1307_i2c_writebyte)(uint8_t, uint32_t, uint8_t, uint8_t);
static void (*_rtc_ds1307_i2c_writebyte_multiple)(uint8_t, uint32_t, uint8_t, uint8_t*, uint8_t);
static uint8_t (*_rtc_ds1307_i2c_readbyte)(uint8_t, uint32_t, uint8_t);
static void (*_rtc_ds1307_i2c_readbyte_multiple)(uint8_t, uint32_t, uint8_t, uint8_t*, uint8_t);

void PUTINFLASH RTC_DS1307_SetDebug(uint8_t debug_on)
{
    //SET DEBUG PRINTF ON(1) OR OFF(0)

    _rtc_ds1307_debug = debug_on;
}

void PUTINFLASH RTC_DS1307_SetI2CFunctions(void (*i2c_init)(void),
                                            void (*i2c_writebyte)(uint8_t, uint32_t, uint8_t, uint8_t),
                                            void (*i2c_writebytemultiple)(uint8_t, uint32_t, uint8_t, uint8_t*, uint8_t),
                                            uint8_t (*i2c_readbyte)(uint8_t, uint32_t, uint8_t),
                                            void (*i2c_readbytemultiple)(uint8_t, uint32_t, uint8_t, uint8_t*, uint8_t)
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

uint8_t PUTINFLASH RTC_DS1307_GetControlRegister(void)
{
    //DS1307 RETURN CONTROL REGISTER VALUE

    return (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_CONTROL, 1);
}

void PUTINFLASH RTC_DS1307_GetTime(RTC_DS1307_TIME* time)
{
    //DS1307 GET CURRENT TIME

    uint8_t byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_SECONDS, 1);
    time->second = (RTC_DS1307_SECONDS_GET_DIG1(byte) * 10) + RTC_DS1307_SECONDS_GET_DIG0(byte);

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_MINUTES, 1);
    time->minute = (RTC_DS1307_MINUTES_GET_DIG1(byte) * 10) + RTC_DS1307_MINUTES_GET_DIG0(byte);

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_HOURS, 1);
    time->hour_format = RTC_DS1307_HOURS_GET_1224_BIT(byte);
    if(time->hour_format == timeformat_12)
    {
        //12 HR FORMAT
        time->am_pm = RTC_DS1307_HOURS_GET_AMPM_BIT(byte);
        time->hour = (RTC_DS1307_HOURS_12_GET_DIG1(byte) * 10) + RTC_DS1307_HOURS_12_GET_DIG0(byte);
    }
    else
    {
          time->hour = (RTC_DS1307_HOURS_24_GET_DIG1(byte) * 10) + RTC_DS1307_HOURS_24_GET_DIG0(byte);
    }

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_DAY, 1);
    time->day = RTC_DS1307_DAY_GET(byte);

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_DATE, 1);
    time->date = (RTC_DS1307_DATE_GET_DIG1(byte) * 10) + RTC_DS1307_DATE_GET_DIG0(byte);

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_MONTH, 1);
    time->month = (RTC_DS1307_MONTH_GET_DIG1(byte) * 10) + RTC_DS1307_MONTH_GET_DIG0(byte);

    byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_YEAR, 1);
    time->year = 2000 + (RTC_DS1307_YEAR_GET_DIG1(byte) * 10) + RTC_DS1307_YEAR_GET_DIG0(byte);
}

void PUTINFLASH RTC_DS1307_SetControlRegister(RTC_DS1307_CONTROL_CONFIG control)
{
    //DS1307 SET CONTROL REGISTER OPTIONS

    uint8_t byte = 0;
    if(control.out)
    {
        byte = RTC_DS1307_CONTROL_SET_OUT(byte);
    }

    if(control.sqwe)
    {
        byte = RTC_DS1307_CONTROL_SET_SQWE(byte);
        byte = RTC_DS1307_CONTROL_SET_RS(byte, control.freq);
    }

    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_CONTROL, 1, byte);

    if(_rtc_ds1307_debug)
    {
        PRINTF("RTC : DS1307 : Control register written with %x\n", byte);
    }
}

void PUTINFLASH RTC_DS1307_SetTimeFormat(RTC_DS1307_TIMEFORMAT_CONFIG format)
{
    //SET DS1307 TIMEFORMAT (12/24 HRS)

    uint8_t byte = 0;
    if(format == timeformat_12)
    {
        //12 HRS
        byte = RTC_DS1307_HOURS_SET12(byte);
        if(_rtc_ds1307_debug)
        {
            PRINTF("RTC : DS1307 : Time format set to 12 hrs. byte = %x\n", byte);
        }
    }
    else
    {
        //24 HRS
        RTC_DS1307_HOURS_SET24(byte);
        if(_rtc_ds1307_debug)
        {
            PRINTF("RTC : DS1307 : Time format set to 24 hrs. byte = %x\n", byte);
        }
    }
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_HOURS, 1, byte);
}

void PUTINFLASH RTC_DS1307_SetTime(RTC_DS1307_TIME time)
{
    //SET THE SPECIFIED TIME

    //SECOND
    uint8_t byte = 0;
    byte = RTC_DS1307_SECONDS_SET_DIG1(byte, (time.second/10)) | RTC_DS1307_SECONDS_SET_DIG0(byte, (time.second % 10));
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_SECONDS, 1, byte);

    //MINUTE
    byte = 0;
    byte = RTC_DS1307_MINUTES_SET_DIG1(byte, (time.minute/10)) | RTC_DS1307_MINUTES_SET_DIG0(byte, (time.minute % 10));
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_MINUTES, 1, byte);

    //HOUR
    byte = 0;
    if(time.hour_format == timeformat_24)
    {
        //24 HR FORMAT
        byte = RTC_DS1307_HOURS_SET24(byte);
        byte = RTC_DS1307_HOURS_SET_DIG1(byte, (time.hour/10)) | RTC_DS1307_HOURS_SET_DIG0(byte, (time.hour % 10));
    }
    else
    {
        //12 HR FORMAT
        byte = RTC_DS1307_HOURS_SET12(byte);
        if(time.am_pm == timeformat_am)
            byte = RTC_DS1307_HOURS_SETAM(byte);
        else
            byte = RTC_DS1307_HOURS_SETPM(byte);
            byte = RTC_DS1307_HOURS_SET_DIG1(byte, (time.hour/10)) | RTC_DS1307_HOURS_SET_DIG0(byte, (time.hour % 10));
    }
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_HOURS, 1, byte);

    //DAY
    byte = 0;
    byte = RTC_DS1307_DAY_SET(byte, time.day);
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_DAY, 1, byte);

    //DATE
    byte = 0;
    byte = RTC_DS1307_DATE_SET_DIG1(byte, (time.date/10)) | RTC_DS1307_DATE_SET_DIG0(byte, (time.date % 10));
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_DATE, 1, byte);

    //MONTH
    byte = 0;
    byte = RTC_DS1307_MONTH_SET_DIG1(byte, (time.month/10)) | RTC_DS1307_MONTH_SET_DIG0(byte, (time.month % 10));
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_MONTH, 1, byte);

    //YEAR
    byte = 0;
    byte = RTC_DS1307_YEAR_SET_DIG1(byte, ((time.year - 2000)/10)) | RTC_DS1307_YEAR_SET_DIG0(byte, ((time.year - 2000) % 10));
    (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_YEAR, 1, byte);

    if(_rtc_ds1307_debug)
    {
        PRINTF("RTC : DS1307 : Time set to user specified !\n");
    }
}

void PUTINFLASH RTC_DS1307_Initialize(void)
{
    //INTIALIZE DS1307 RTC
    //CHECK FOR CH BIT, IF DISABLED, ENABLE IT

    //INIT BACKEND I2C
    #if defined(ESP8266)
        ESP8266_I2C_SetDebug(_rtc_ds1307_debug);
        ESP8266_I2C_Init();
    #endif

    //VERIFY CH BIT
    _rtc_ds1307_verify_ch_bit();

    if(_rtc_ds1307_debug)
    {
        PRINTF("RTC : DS1307 : Initialized\n");
    }
}

RTC_DS1307_DAYFORMAT_CONFIG PUTINFLASH RTC_DS1307_ConvertDayStringToValue(char* day_str)
{
    //USER HELPER FUNCTION TO CONVERT DAY STRING TO DAY VALUE

    if(strcmp(day_str, "Sun") == 0)
      return sunday;
    else if(strcmp(day_str, "Mon") == 0)
      return monday;
    else if(strcmp(day_str, "Tue") == 0)
      return tuesday;
    else if(strcmp(day_str, "Wed") == 0)
      return wednesday;
    else if(strcmp(day_str, "Thu") == 0)
      return thursday;
    else if(strcmp(day_str, "Fri") == 0)
      return friday;
    else if(strcmp(day_str, "Sat") == 0)
      return saturday;
}

RTC_DS1307_MONTHFORMAT_CONFIG PUTINFLASH RTC_DS1307_ConvertMonthStringToValue(char* month_str)
{
    //USER HELPER FUNCTION TO CONVERT DAY STRING TO DAY VALUE

    if(strcmp(month_str, "Jan") == 0)
      return january;
    else if(strcmp(month_str, "Feb") == 0)
      return february;
    else if(strcmp(month_str, "Mar") == 0)
      return march;
    else if(strcmp(month_str, "Apr") == 0)
      return april;
    else if(strcmp(month_str, "May") == 0)
      return may;
    else if(strcmp(month_str, "Jun") == 0)
      return june;
    else if(strcmp(month_str, "Jul") == 0)
      return july;
    else if(strcmp(month_str, "Aug") == 0)
      return august;
    else if(strcmp(month_str, "Sep") == 0)
      return sepetmber;
    else if(strcmp(month_str, "Oct") == 0)
      return october;
    else if(strcmp(month_str, "Nov") == 0)
      return november;
    else if(strcmp(month_str, "Dec") == 0)
      return december;
}

uint8_t PUTINFLASH _rtc_ds1307_verify_ch_bit(void)
{
    //CHECK THE CH BIT AND TURN ON OSCILLATOR IF OFF

    //READ CH BIT
    uint8_t byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_SECONDS, 1);

    if(RTC_DS1307_SECONDS_GET_CH_BIT(byte))
    {
      if(_rtc_ds1307_debug)
      {
          PRINTF("RTC : DS1307 : CH bit disabled. Enabling it\n");
      }

      byte = 0;
      (*_rtc_ds1307_i2c_writebyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_SECONDS, 1, byte);

      //CONFIRM CH WRITTEN
      byte = (*_rtc_ds1307_i2c_readbyte)(RTC_DS1307_I2C_ADDRESS, RTC_DS1307_REGISTER_SECONDS, 1);

      if(RTC_DS1307_SECONDS_GET_CH_BIT(byte))
      {
        if(_rtc_ds1307_debug)
        {
            PRINTF("RTC : DS1307 : Error writing CH bit\n");
        }
      }
    }
}
