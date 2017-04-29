/**
  ******************************************************************************
  * @file   mcheck.h
  * @author  
  * @version 
  * @date    
  * @brief    check模块  用于边缘触发，一次性，支持下降沿和上升沿触发回调，双边缘滤波
  ******************************************************************************
  * @attention 20161110     v1.1   	jgb		重构 已测试 20161113
  ******************************************************************************
  */

#ifndef __MCHECK_H_
#define __MCHECK_H_

#include "app_cfg.h"


typedef uint8_t (*IsDownFunc)(void);
typedef void (*CheckCallBackFunc)(void);

typedef struct check_mem_s
{
	uint8_t state;
	uint8_t filtertime;
	uint8_t filtercount;
	IsDownFunc pfuc;
	CheckCallBackFunc pdowncallfuc;
	CheckCallBackFunc pUpcallfuc;
	void *next;	
}cks_mem_t;


void checkregister(cks_mem_t *pcks, uint8_t filtetime, IsDownFunc pfuc, CheckCallBackFunc pdowncallback, CheckCallBackFunc pUpcallfuc);
void checkupdate(uint16_t ElapseTime);
#endif


