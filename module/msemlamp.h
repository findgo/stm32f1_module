/**
  ******************************************************************************
  * @file   msemlamp.h
  * @author  
  * @version 
  * @date    
  * @brief    �źŵ�ģ��
  ******************************************************************************
  * @attention      v1.0   	jgb		   20170508
  ******************************************************************************
  */
  /*
  *  �źŵƣ���ֵֵ�ŵ�
  *  ��ģ����Ҫ����㱼��������֪ͨ����
  */
#ifndef __MSEMLAMP_H_
#define __MSEMLAMP_H_

#include "app_cfg.h"

typedef struct{
    uint8_t se_val;
}msemlamp_t;

void msemlamp_init(msemlamp_t *sem,uint8_t origval);
void msemlamp_post(msemlamp_t *sem);
uint8_t msemlamp_get(msemlamp_t *sem);

#endif
