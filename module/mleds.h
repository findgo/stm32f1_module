/**
  ******************************************************************************
  * @file   mleds.h
  * @author  
  * @version 
  * @date    
  * @brief    ledsģ��,�����ڷ�����
  ******************************************************************************
  * @attention      v1.1   	jgb		�ع�   20151110
  * @attention      v1.2   	jgb		        20170429
  ******************************************************************************
  */
/*
;   ����coilsʹ�õĸ���mLEDSNUM��
;   ʵ��MTURN_LED1_ON ����coils��on of���ܺ�
;   ʵ��LEDSEQ_EVE_EXTERN_LSIT �ⲿ��
;
;   eve     todo        onpct    period   next	
;  �¼�   ��������      ռ�ձ�    ����    ��Ϊ0
;
;   ledseqset ����˵�������ֻ�Ǽ򵥿�,��,�л�����eve������ΪLEDSEQ_EVE_NONE,�ɵ��ú���ledseqmodeset
;   ������¼�,��mode ��ΪLEDSEQ_MODE_NONE���ɵ��ú���ledseqeveset
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

#define LED_SEQ_DEFAULT_BLINK_COUNT      		 1    //!< Ĭ��գ�ִ���
#define LED_SEQ_DEFAULT_BLINK_DUTY_CYCLE 		 5    //!< Ĭ��գ��ռ�ձ�
#define LED_SEQ_DEFAULT_FLASH_QUARTER_DUTY_CYCLE 25   //!< �ٷֶ�ʮ��ռ�ձ�
#define LED_SEQ_DEFAULT_FLASH_HALF_DUTY_CYCLE 	 50   //!< �ٷ���ʮռ�ձ�
#define LED_SEQ_DEFAULT_FLASH_TIME       		 1000 //!< Ĭ�����ڣ�(��ʱ��Ϊ׼)

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
	LEDSEQ_EVE_BLINK_ONCE = 1,		// գ��һ��
	LEDSEQ_EVE_FLASH_BLINK = 2,		//����գ��
	LEDSEQ_EVE_FLASH_QUARTER = 3,	// ��˸ռ�ձ�1/4
	LEDSEQ_EVE_FLASH_HALF = 4,      // ��˸ռ�ձ�1/4
	LEDSEQ_EVE_MAX = LEDSEQ_EVE_FLASH_HALF + 1
}ledseq_eve_t;

/* �ⲿ���¼����ԵǼ������� */
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


// �ڲ���������������
void ledseqOnOff (uint8_t leds, bool val);

#endif


