/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief  �ṩ��flash�Ĺ���ʵ�ֶ����õı��棬������״̬��ʵ��,��������������Ĵ�������������ö�ʧ
  * @       ���� mtimers.h mcfg_defaultpara.c
  ******************************************************************************
  * @attention      v1.0   	jgb   20170503     ����
  *                 v1.1    jgb    20170516    ���SN�������⣬�����ο�linux time_after  �������ʱ��band index����ֵ���Ե�����
  *                 
  ******************************************************************************
  */
/*
 * ��ģ�� �ṩ��SPI flash���ϲ�Ӧ�ã���������������,��״̬��ʵ��,���ṩ������,�ײ�ӿں��ⲿ�ӿھͿ���ʹ��
 *  ���� 4K byte ҳ�� 256 byte
 *  ��Ҫ���ṩ�ĸ��������ײ�ӿ� CFGFLASH_Read() ��flash����
 *                               CFGFLASH_WritePageSequeue()   ����һ��ҳд���У� 0 - 256�ֽ�  
 *                               CFGFLASH_EraseSectorSequeue() ����һ������������
 *                               CFGFLASHStatusBusy()      оƬ��д��дæ״̬��
 *  
 *  ʵ��ԭ��:  ��ο�ӡ��ʼ�:  SPI FLASH ���ò�������ģ��
 */

/* 
 * NOTE:    ����SPI flash����������ֻ��飬������ʵ�ֻ�����������,�����Ի�����б�ǡ�
 * 
 * ʹ��˵�� 
 * ���ȶ�������ṹ��mcfg_para_t�ṹ����Ĳ���
 * ʵ��mcfg_defaultpara.c�и���mcfg_para_t ����Ĭ�ϲ���
 *
 * ����ʹ�õ�spi flash 
 * ����������С(һ��Ϊ4K byte)��
 * ����ҳ��С(һ��Ϊ256 byte)
 * ����Ҫʹ�����õ�flash��ʼ��ַ
 * ����ʹ�õ�Ҫʹ�õ��������� 
 * ����ˢ�µ�ʱ��
 * ����CRC32У��ĳ�ʼֵ
 * ʵ���ĸ��ײ���������
 * spi FLASH���õ�����������, ҳ�ڿ������д0 - 256 �ֽ�
 * NOTE: �����ҳһ��д������256�ֽڵ����ݽ�д�ڱ�ҳͷ
 */
#ifndef __MCFG_H_
#define __MCFG_H_

#include "app_cfg.h"
#include "bsp_spi_flash.h"

// cfg use flash start address
#define CFG_FLASH_SECTOR_BASE_ADDRESS  ((uint32_t)0x00000000)
// flash Page size = 256 byte */
#define CFG_FLASH_PAGE_SIZE     ((uint32_t)0x100)
// flash Sector size = 4K byte */
#define CFG_FLASH_SECTOR_SIZE  ((uint32_t)0x1000) 
// cfg use flash sector number
#define CFG_FLASH_USE_SECTOR_NUM  (4)

// ��CFG���еĴ洢����(sector size * sector number)
#define CFG_FLASH_USE_TOTAL_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_USE_SECTOR_NUM)

// ���øı��ˢ��ʱ�䣬5����
#define CFG_FLUSH_TIME          ((uint32_t)5000)

// ����CRC32����ĳ�ʼֵ
#define CFG_CRC32_INIT_VALUE    0xffffffff
/*
 * ����Ҫ���������� ʵ��
 */
#define CFGFLASH_Read(offset,pbuf,nbytes)               sf_ReadBuffer(offset,pbuf,nbytes)
#define CFGFLASH_WritePageSequeue(offset,pbuf,nbytes)   sf_StartWritePageSequeue(offset,pbuf,nbytes)
#define CFGFLASH_EraseSectorSequeue(sector_address)     sf_StartEraseSectoreSequeue(sector_address)
#define CFGFLASHStatusBusy()                            sf_StatusBusy()

// cfg type is 2 byte
// ÿ������������uint16_t��
typedef struct 
{
// add your para
    uint16_t deviceID;
}mcfg_para_t;


mcfg_para_t *mcfgPara(void);
void mcfgLoadinit(void);
void mcfgFlush(void);
void mcfgUpdate(void);

#endif

