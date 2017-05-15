/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief    cfg ģ�������㱼����̬�������ã���Ҫ��ʹ��spi flash�������������Ը�����дʱ�����������һ���Ĺ���ʹ��������
  * @       ���� mtimers.h mcfg_defaultpara.c
  ******************************************************************************
  * @attention      v1.0   	jgb   20170503   δ����
  ******************************************************************************
  */


/* 
 * NOTE:    ����SPI flash����������ֻ��飬������ʵ�ֻ�����������
 * 
 * ʹ��˵�� 
 * ���ȶ�������ṹ��mcfg_para_t�Ĳ���
 * ʵ��mcfg_defaultpara.c�и���mcfg_para_t ����Ĭ�ϲ���
 *
 * ʵ��flash����������, ҳ��д�������ַ��,æ״̬���  �ĸ�����
 * ����ʹ�õ�spi flash ����ʹ�õ�������С(һ��Ϊ4K),���趨���õ���ʼ��ַ
 * spi FLASH���õ�����������, ҳ�ڿ������д0 - 256 �ֽ�
 */
#ifndef __MCFG_H_
#define __MCFG_H_

#include "app_cfg.h"

// cfg use flash start address
#define CFG_FLASH_BASE_ADDRESS  ((uint32_t)0x0801f000)
// flash Page size = 256 byte */
#define CFG_FLASH_PAGE_SIZE     ((uint32_t)0x100)
// flash Sector size = 4K byte */
#define CFG_FLASH_SECTOR_SIZE  ((uint32_t)0x1000) 
// cfg use flash sector number
#define CFG_FLASH_USE_SECTOR_NUM  (2)

// ��CFG���еĴ洢����(sector size * sector number)
#define CFG_FLASH_USE_TOTAL_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_USE_SECTOR_NUM)

// ���øı��ˢ��ʱ�䣬5����
#define CFG_FLUSH_TIME          ((uint32_t)5000)

// ����CRC32����ĳ�ʼֵ
#define CFG_CRC32_INIT_VALUE    0xffffffff
/*
 * ����Ҫ���������� ʵ��
 */
#define CFGFLASH_Read(offset,pbuf,nbytes) 
#define CFGFLASH_WritePage(offset,pbuf,nbytes)
#define CFGFLASH_Erase(sector_address)
#define CFGFLASHStatusBusy()    0




// cfg type is 2 byte
// ÿ������������uint16_t��
typedef struct 
{
// add your para
    uint16_t deviceID;  

}mcfg_para_t;


void mcfgLoadinit(void);
mcfg_para_t *mcfgPara(void);
void mcfgFlush(void);
void mcfgUpdate(void);

#endif

