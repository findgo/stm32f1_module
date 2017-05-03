/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief    cfg ģ�������㱼����̬�������ã���Ҫ��flash����дʱ�����������һ���Ĺ���ʹ��������
  * @    ���� mtimers.h para_cfg.h
  ******************************************************************************
  * @attention      v1.0   	jgb   2010503   δ����
  ******************************************************************************
  */


/* ʹ��˵�� 
 * ������para.c�����������
 * ʵ��para.c������������һ������ 
 * ����ʹ�õ�flash ����ʹ�õ�������С(һ��Ϊ4K)�� ʹ�ö��ٸ�������Ϊ����,��Ϊ���õ���ʼ��ַ
 */
#ifndef __MCFG_H_
#define __MCFG_H_

#include "app_cfg.h"


// cfg ������ʼ��ַ��Ĭ��(0)��flash ����ͷ��ʼ
#define CFG_FLASH_ADDRESS_START  (0)
// ʹ�õ�flash������С
#define CFG_FLASH_SECTOR_SIZE  (4096)
// ��cfgʹ�õ�flash��������
#define CFG_FLASH_SECTOR_USE_NUM  (8)


// ����������(�ȴ�С * CFG_FLASH_SECTOR_USE_NUM)
#define CFG_FLASH_USE_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_SECTOR_USE_NUM)

/*
 * �������FLASH�Ķ���ҳ�ڵĲ���������������ֿ�ҳ���
 * ����Ҫ���������� ʵ��
 */
#define CFGFLASH_READ(offset,pbuf,nbytes) 
#define CFGFLASH_WRITE(offset,pbuf,nbytes)
#define CFGFLASH_ERASE(sector_offset)
#define CFGFLASHSTATUSBUSY()


void mcfgLoadinit(void);
void mcfgUpdate(void);


#endif

