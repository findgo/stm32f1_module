/**
  ******************************************************************************
  * @file   mleds.h
  * @author  
  * @version 
  * @date    
  * @brief    leds模块,可用于蜂鸣器
  ******************************************************************************
  * @attention      v1.1   	jgb		重构   20151110
  * @attention      v1.2   	jgb		        20170429
  ******************************************************************************
  */
/*
;   定义coils使用的个数mLEDSNUM，
;   实现MTURN_LED1_ON 各个coils的on of功能宏
;   实现LEDSEQ_EVE_EXTERN_LSIT 外部宏
;
;   eve     todo        onpct    period   next	
;  事件   发生次数      占空比    周期    设为0
;
;   ledseqset 调用说明，如果只是简单开,关,切换，将eve参数设为LEDSEQ_EVE_NONE,可调用函数ledseqmodeset
;   如果是事件,将mode 设为LEDSEQ_MODE_NONE，可调用函数ledseqeveset
;
*/

#ifndef __MLEDS_H_
#define __MLEDS_H_

#include "app_cfg.h"

#include "hal_led.h"

/* Defaults */
#define mLEDSNUM   1

/* leds - The leds number is the same as the bit position */
#define MLED_1     	((uint8_t)1 << 0)
#define MLED_2    	((uint8_t)1 << 1)
#define MLED_3     	((uint8_t)1 << 2)
#define MLED_4     	((uint8_t)1 << 3)
#define MLED_5     	((uint8_t)1 << 4)
#define MLED_6    	((uint8_t)1 << 5)
#define MLED_7     	((uint8_t)1 << 6)
#define MLED_8     	((uint8_t)1 << 7)
#define MLED_ALL   	(uint8_t)((1 << mLEDSNUM) - 1)

#define LED_SEQ_ALWAYS_TODO						 0xff

#define LED_SEQ_DEFAULT_BLINK_COUNT      		 1    //!< 默认眨现次数
#define LED_SEQ_DEFAULT_BLINK_DUTY_CYCLE 		 5    //!< 默认眨现占空比
#define LED_SEQ_DEFAULT_FLASH_QUARTER_DUTY_CYCLE 25   //!< 百分二十五占空比
#define LED_SEQ_DEFAULT_FLASH_HALF_DUTY_CYCLE 	 50   //!< 百分五十占空比
#define LED_SEQ_DEFAULT_FLASH_TIME       		 1000 //!< 默认周期，(以时基为准)

/* Modes */
typedef enum 
{
	LEDSEQ_MODE_OFF 		= 0x00,
	LEDSEQ_MODE_ON			= 0x01,
	LEDSEQ_MODE_TOGGLE  	= 0x02,
	LEDSEQ_MODE_NONE		= 0x04
}ledseq_mode_t;

/* event */
typedef enum 
{
	LEDSEQ_EVE_NONE = 0,
	LEDSEQ_EVE_BLINK_ONCE = 1,		// 眨现一次
	LEDSEQ_EVE_FLASH_BLINK = 2,		//无限眨现
	LEDSEQ_EVE_FLASH_QUARTER = 3,	// 闪烁占空比1/4
	LEDSEQ_EVE_FLASH_HALF = 4,      // 闪烁占空比1/4
	LEDSEQ_EVE_MAX = LEDSEQ_EVE_FLASH_HALF + 1
}ledseq_eve_t;

/* 外部灯事件可以登记在这里 */
/*	eve  todo  onpct    period   next	*/
//{LEDSEQ_EVE_FLASH_BLINK, LED_SEQ_ALWAYS_TODO, LED_SEQ_DEFAULT_BLINK_DUTY_CYCLE, LED_SEQ_DEFAULT_FLASH_TIME, 0},
#define LEDSEQ_EVE_EXTERN_LSIT 


#define MTURN_LED1_ON()		st(GPIO_ResetBits(HAL_REDLED_PORT,HAL_REDLED_PIN);)
#define MTURN_LED1_OFF()	st(GPIO_SetBits(HAL_REDLED_PORT,HAL_REDLED_PIN);)
#define MTURN_LED2_ON()	 	st(;)
#define MTURN_LED2_OFF()	st(;)
#define MTURN_LED3_ON()		st(;)
#define MTURN_LED3_OFF()	st(;)
#define MTURN_LED4_ON()		st(;)
#define MTURN_LED4_OFF()	st(;)
#define MTURN_LED5_ON()		st(;)
#define MTURN_LED5_OFF()	st(;)
#define MTURN_LED6_ON()		st(;) 
#define MTURN_LED6_OFF()	st(;)
#define MTURN_LED7_ON()		st(;)
#define MTURN_LED7_OFF()	st(;)
#define MTURN_LED8_ON()		st(;)
#define MTURN_LED8_OFF()	st(;)

void ledseqInit(void);
void ledseqset(uint8_t leds, ledseq_eve_t eve, ledseq_mode_t mode);
void ledsequpdate (uint16_t ElapseTime);

#define ledseqmodeset(leds,mode)  ledseqset(leds,LEDSEQ_EVE_NONE,mode)
#define ledseqeveset(leds,eve)   ledseqset(leds,eve,LEDSEQ_MODE_NONE)


// 内部函数，尽量少用
void ledseqOnOff (uint8_t leds, bool val);

#endif


