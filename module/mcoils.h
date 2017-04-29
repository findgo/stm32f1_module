/**
  ******************************************************************************
  * @file   mcoils.h
  * @author  
  * @version 
  * @date    
  * @brief    coilsģ�飬ֻ�п��͹���״̬�����Թ�����һ���
  ******************************************************************************
  * @attention    v1.1      jgb     �ع�            20151113 
  * @attention    v1.2      jgb     �򻯹���        20170428
  ******************************************************************************
  */
/*
;   ����coilsʹ�õĸ���mCOILSNUM��
;   ʵ��MTURN_COILS1_ON ����coils�Ĺ��ܺ�
;
*/
#ifndef __COILS_H_
#define __COILS_H_

#include "app_cfg.h"


//define Coils Numbers  Max 8
#define mCOILSNUM   1

/* COLIS - The COLIS number is the same as the bit position */
#define MCOILS_1        ((uint8_t)1 << 0)
#define MCOILS_2        ((uint8_t)1 << 1)
#define MCOILS_3        ((uint8_t)1 << 2)
#define MCOILS_4        ((uint8_t)1 << 3)
#define MCOILS_5        ((uint8_t)1 << 4)
#define MCOILS_6        ((uint8_t)1 << 5)
#define MCOILS_7        ((uint8_t)1 << 6)
#define MCOILS_8        ((uint8_t)1 << 7)
#define MCOILS_ALL      (uint8_t)((1 << mCOILSNUM) - 1)

/* Modes */
typedef enum 
{
    MCOILS_MODE_OFF     = 0x00,
    MCOILS_MODE_ON      = 0x01,
    MCOILS_MODE_TOGGLE  = 0x02,
}mCoils_Mode;

#define MTURN_COILS1_ON()   
#define MTURN_COILS1_OFF()  
#define MTURN_COILS2_ON()    
#define MTURN_COILS2_OFF()   
#define MTURN_COILS3_ON()
#define MTURN_COILS3_OFF()
#define MTURN_COILS4_ON()
#define MTURN_COILS4_OFF()
#define MTURN_COILS5_ON()   
#define MTURN_COILS5_OFF()   
#define MTURN_COILS6_ON()    
#define MTURN_COILS6_OFF()   
#define MTURN_COILS7_ON()
#define MTURN_COILS7_OFF()
#define MTURN_COILS8_ON()
#define MTURN_COILS8_OFF()

void mCoilsInit(void);
void mCoilsSet (uint8_t Coils, mCoils_Mode mode);
bool mCoilsGetStasus(uint8_t which);
uint8_t mCoilsGetMulStasus(uint8_t Coils);
// �ڲ�������������ʹ��
void mCoilsOnOff(uint8_t Coils, mCoils_Mode mode);

#endif

