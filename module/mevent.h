/**
  ******************************************************************************
  * @file   mevent.h
  * @author  
  * @version 
  * @date    
  * @brief    ��ģ��Ϊ�¼���������Ϊʱ���¼�����ģ�� ��OSAL�¼�������ʽ
  ******************************************************************************
  * @attention    v1.1      jgb     �ع�            20170511
  ******************************************************************************
  */

/*
 * ʹ��˵��: ��mEv_create ��ʱ���¼��Ļص�����ע���ȥ
 *  mEv_start_timevent : ����һ��ʱ���¼���ʱ�䵽�� ����ָ���Ļص�������ֻ����һ��
 *  mEv_start_reload_timevent: ����һ��ʱ���¼���ʱ�䵽�� ����ָ���Ļص����������������·���
 *  mEv_set_event: ����һ���¼������������� ֻ����һ��
 *  mEv_stop_timevent �� mEv_clear_event Ч��һ�������һ�����뷢�����¼���
 *  mEvTimerUpdate ����ѭ������,ElapseTime ��ִ�д˺���������ʱ��
 * 
 * NOTE: �ص�������pmev, ��ע��ʱ��ev,�ڻص�ʱ��Ѵ˲�������
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

