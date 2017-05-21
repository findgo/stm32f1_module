/**
  ******************************************************************************
  * @file   mod_clock.c
  * @author  
  * @version 
  * @date    
  * @brief   系统时基内置
  ******************************************************************************
  * @attention      v1.1    jgb     重构        			 20151112  
  * @attention      v1.2    jgb     重新修正函数规范          20170427
  ******************************************************************************
  */
#ifndef __MCLOCK_H_
#define __CLOCK_H_

#include "app_cfg.h"

// 定义了时间片的频率
#define CLOCKTICK_RATE_HZ    (1000)   // 1000hz
// 计算出时间片的时间ms
#define CLOCKTICK_RATE_MS   (1000 / CLOCKTICK_RATE_HZ)

uint32_t mcu_getCurSysctime(void);
uint32_t mcu_elapsedSystime(void);
void mcu_systime_isr_callback(void);

#endif

