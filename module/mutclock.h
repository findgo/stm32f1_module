/**
  ******************************************************************************
  * @file   mutc.h
  * @author  
  * @version 
  * @date    
  * @brief     utc时间，基于clock
  ******************************************************************************
  * @attention 	  v1.0    jgb     jiangguobao        20170503
  ******************************************************************************
  */
#ifndef __MUTC_CLOCK_H_
#define __MUTC_CLOCK_H_

#include "app_cfg.h"


#define	BEGYEAR  2000     //  UTC started at 00:00:00 January 1, 2000
// number of seconds since 0 hrs, 0 minutes, 0 seconds, on the
// 1st of January 2000 UTC
typedef uint32_t UTCTime;

typedef struct
{
  uint8_t seconds;  // 0-59
  uint8_t minutes;  // 0-59
  uint8_t hour;     // 0-23
  uint8_t day;      // 0-30
  uint8_t month;    // 0-11
  uint16_t year;    // 2000+
} UTCTimeStruct;

void mutc_ClockUpdate( uint16_t elapsedMSec );
void mutc_setClock( UTCTime newTime );
UTCTime mutc_getClock( void );
void mutc_ConvertUTCTime( UTCTimeStruct *tm, UTCTime secTime );
UTCTime mutc_ConvertUTCSecs( UTCTimeStruct *tm );

#endif

