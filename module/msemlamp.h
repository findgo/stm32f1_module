/**
  ******************************************************************************
  * @file   msemlamp.h
  * @author  
  * @version 
  * @date    
  * @brief    信号灯模块
  ******************************************************************************
  * @attention      v1.0   	jgb		   20170508
  ******************************************************************************
  */
  /*
  *  信号灯，二值值号灯
  *  本模块主要解决裸奔机器处理通知功能
  */
#ifndef __MSEMLAMP_H_
#define __MSEMLAMP_H_

#include "app_cfg.h"

typedef struct{
    uint8_t se_val;
}msemlamp_t;

void msemlamp_init(msemlamp_t *sem,uint8_t origval);
void msemlamp_give(msemlamp_t *sem);
uint8_t msemlamp_take(msemlamp_t *sem);

#endif
