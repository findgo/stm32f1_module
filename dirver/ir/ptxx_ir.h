
/**
  ******************************************************************************
  * @file   ptxx.h
  * @author  
  * @version 
  * @date    
  * @brief    ��ģ��������PT2622��PT2624��ptxxϵ�б���оƬ�Ľ���
  ******************************************************************************
  * @attention      v1.0   	jgb		����                                    20170520
  ******************************************************************************
  */
/*
 * ��ģ�����ײ�ӿڣ����ý��ٵĺ� ��API����ptxx�ı���
 */

#ifndef __PTXX_IR__
#define __PTXX_IR__


#include "dev.h"			// ����������ͷ�ļ�
#include "global.h"
#include "includes.h"

/* �������ݺ͵�ַ��λ�� 
* ����λ����Ϊ0 - 6λ
*
*/
#define PTXX_DATA_BIT_NUM (4)
#define PTXX_ADDR_BIT_NUM (12 - PTXX_DATA_BIT_NUM)

// ����һ�����ڵ�ʱ����������� �����P4
#define PTXX_TIME_PULSE_CYCLE  (120)

void PTxx_Capture_Decode(u32 delta_time);
u8 PTxx_GetCode(u32 *addr,u8 *cmd);

#define IR_GetKeyCode(addr,cmd) PTxx_GetCode(addr,cmd)


#endif
