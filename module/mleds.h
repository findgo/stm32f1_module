﻿/**
  ******************************************************************************
  * @file   mleds.h
  * @author  
  * @version 
  * @date    
  * @brief    leds模块,可用于蜂鸣器
  ******************************************************************************
  * @attention      v1.1   	jgb		重构                                    20151110
  * @attention      v1.2   	jgb		抽像宏，减少用户介入                   20170429
  * @attention      v1.3   	jgb		修改一些宏,增加对事件添加的抽像层      20170522
  ******************************************************************************
  */
/*
;   定义coils使用的个数mLEDSNUM，
;   实现MTURN_LED1_ON 各个coils的on off功能宏
;   
;  实现自定义事件，首先在ledseq_eve_t 的枚举中定义自己的事件
;   调用 LEDSEQ_EVE_REGISTER(eve,todo,onduty,cycle) 在LEDSEQ_EVE_EXTERN_LSIT 添加自己的事件
;
;   LEDSEQ_EVE_REGISTER 参数说明:
;   eve: 自己在ledseq_eve_t 定义的事件
;   todo: 本事件要发生几次，如果是无限次发生，可使用宏 LED_SEQ_ALWAYS_TODO
;   onduty: 占空比 为0 - 100的整数
;   cycle:  周期 
;
;
;   ledseqset 调用API说明，
;   如果只是简单开,关,切换，调用函数ledseqmodeset
;   如果是事件,调用函数ledseqeveset
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

#define LED_SEQ_BLINK_TODO      		 1    //!< 默认眨现次数
#define LED_SEQ_BLINK_DUTY_CYCLE 		 5    //!< 默认眨现占空比
#define LED_SEQ_FLASH_QUARTER_DUTY_CYCLE 25   //!< 百分二十五占空比
#define LED_SEQ_FLASH_HALF_DUTY_CYCLE 	 50   //!< 百分五十占空比
#define LED_SEQ_FLASH_CYCLE_TIME       		 1000 //!< 默认周期，(以时基为准)

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
	LEDSEQ_EVE_FLASH_BLINK = 2,		// 无限眨现
	LEDSEQ_EVE_FLASH_QUARTER = 3,	// 闪烁占空比1/4
	LEDSEQ_EVE_FLASH_HALF = 4,      // 闪烁占空比1/4
// add you define event below




// add you define event above 
	LEDSEQ_EVE_MAX
}ledseq_eve_t;

#define LEDSEQ_EVE_REGISTER(eve,todo,onduty,cycle) {eve,todo,onduty,cycle,0},


#define LEDSEQ_EVE_EXTERN_LSIT //LEDSEQ_EVE_REGISTER(eve,todo,onduty,cycle)
                


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


