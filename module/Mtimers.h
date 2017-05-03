/**
  ******************************************************************************
  * @file   mtimers.h
  * @author  
  * @version 
  * @date    
  * @brief     基于mclock.h
  ******************************************************************************
  * @attention 	  v1.1    jgb     jiangguobao        20151110
  * @attention      v1.2    jgb     重新修正函数规范          20170427
  ******************************************************************************
  */
/*
;   该定时器超时后会自我删除，如需启动，要重新调用start;
;   
;
;
*/


#ifndef __MTIMERS_H_
#define __MTIMERS_H_

#include "app_cfg.h"
#include "mclock.h"

//!< 
#define tm_ms(__x)      ((__x) / CLOCKTICK_RATE_MS)
#define tm_s(__x)       tm_ms((__x) * 1000L)
#define tm_min(__x)     tm_s((__x) * 60L)


typedef struct mtimer_s 
{
    uint32_t start;
    uint32_t timeout;
    bool enable;
}mtimer_t;

//静态初始化
#define MTIMER_INIT() { 0,0,false }  
//动态初始化
#define INIT_MTIMER(ptr)			do{\
                                (ptr)->start = 0; \
                                (ptr)->timeout = 0; \
                                (ptr)->enable = false;\
                            }while (0)
// 定义一个mtimer变量
#define DELCARE_MTIMER(t) mtimer_t t = MTIMER_INIT()

void mtimer_start(mtimer_t *t,uint32_t timeout);
void mtimer_modify(mtimer_t *t,uint32_t timeout);
void mtimer_del(mtimer_t *t);
bool mtimer_expired(mtimer_t *t);


#endif

