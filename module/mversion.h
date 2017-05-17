
/**
  ******************************************************************************
  * @file   mversion.h
  * @author  
  * @version 
  * @date    
  * @brief   �����������ʱ�����ɴΰ汾�� ʮ���ƣ���ʽ��������ʱ
  ******************************************************************************
  * @attention 	  v1.0    jgb            20170502
  ******************************************************************************
  */

#ifndef __MVERSION_H_
#define __MVERSION_H_

#include "app_cfg.h"

// �������汾��
#define FIRMWARE_MAJOR_VER  (1)

/* ���汾�� */
uint8_t mver_getmajorver(void);
/* �ΰ汾�� 10����������ʱ */
uint32_t mver_getminorver(void);

#endif /* __MVERSION_H_ */


