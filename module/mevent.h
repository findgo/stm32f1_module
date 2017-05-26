/**
  ******************************************************************************
  * @file   mevent.h
  * @author  
  * @version 
  * @date    
  * @brief    本模块为事件触发，或为时间事件触发模块 类OSAL事件触发方式
  ******************************************************************************
  * @attention    v1.1      jgb     重构            20170511
  ******************************************************************************
  */

/*
 * 使用说明: 先mEv_create 将时间事件的回调函数注册进去
 *  mEv_start_timevent : 启动一个时间事件，时间到后 调用指定的回调函数，只发生一次
 *  mEv_start_reload_timevent: 启动一个时间事件，时间到后 调用指定的回调函数，会重载重新发生
 *  mEv_set_event: 启动一个事件，立即发生。 只发生一次
 *  mEv_stop_timevent 和 mEv_clear_event 效果一样，清除一个不想发生的事件。
 *  mEvTimerUpdate 由主循环调用,ElapseTime 是执行此函数的流逝时间
 * 
 * NOTE: 回调函数的pmev, 是注册时的ev,在回调时会把此参数传入
 */

#ifndef __MEVENT_H_
#define __MEVENT_H_

#include "app_cfg.h"

typedef void (*mev_call_fn)(void *pmev);

typedef struct {
    uint16_t ev_flag;
    uint16_t timeout;
    uint16_t reloadtime;
    mev_call_fn fn;
    void *next;
}mEv_evnents_t;

void mEv_create(mEv_evnents_t *ev,mev_call_fn fn);
void mEv_start_timevent(mEv_evnents_t *ev,uint16_t timeout_value);
void mEv_start_reload_timevent(mEv_evnents_t *ev,uint16_t timeout_value);
void mEv_stop_timevent(mEv_evnents_t *ev);
void mEv_set_event(mEv_evnents_t *ev);
void mEv_clear_event(mEv_evnents_t *ev);
void mEvTimerUpdate( uint16_t ElapseTime );


#endif

