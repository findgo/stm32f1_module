/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief  提供对flash的管理，实现对配置的保存，无阻塞状态机实现,并具有跳过坏块的处理，以免产生配置丢失
  * @       依赖 mtimers.h mcfg_defaultpara.c
  ******************************************************************************
  * @attention      v1.0   	jgb   20170503     测试
  *                 v1.1    jgb    20170516    解决SN回绕问题，方法参考linux time_after  解决载入时，band index索引值不对的问题
  *                 
  ******************************************************************************
  */
/*
 * 本模块 提供的SPI flash的上层应用，用于裸机保存参数,用状态机实现,仅提供少量宏,底层接口和外部接口就可以使用
 *  扇区 4K byte 页面 256 byte
 *  仅要求提供四个无阻塞底层接口 CFGFLASH_Read() 读flash数据
 *                               CFGFLASH_WritePageSequeue()   启动一个页写序列， 0 - 256字节  
 *                               CFGFLASH_EraseSectorSequeue() 启动一个擦扇区序列
 *                               CFGFLASHStatusBusy()      芯片擦写和写忙状态读
 *  
 *  实现原理:  请参考印象笔记:  SPI FLASH 配置参数管理模块
 */

/* 
 * NOTE:    由于SPI flash有寿命会出现坏块，本工程实现坏块跳过处理,但不对坏块进行标记。
 * 
 * 使用说明 
 * 首先定义参数结构体mcfg_para_t结构体里的参数
 * 实现mcfg_defaultpara.c中根据mcfg_para_t 配置默认参数
 *
 * 根据使用的spi flash 
 * 定义扇区大小(一般为4K byte)，
 * 定义页大小(一般为256 byte)
 * 定义要使用配置的flash起始地址
 * 定义使用的要使用的扇区个数 
 * 定义刷新的时间
 * 定义CRC32校验的初始值
 * 实现四个底层驱动函数
 * spi FLASH采用的是扇区擦除, 页内可随意编写0 - 256 字节
 * NOTE: 如果跨页一次写，超过256字节的数据将写在本页头
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

// 给CFG所有的存储总量(sector size * sector number)
#define CFG_FLASH_USE_TOTAL_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_USE_SECTOR_NUM)

// 配置改变和刷新时间，5秒钟
#define CFG_FLUSH_TIME          ((uint32_t)5000)

// 定义CRC32计算的初始值
#define CFG_CRC32_INIT_VALUE    0xffffffff
/*
 * 以下要求是无阻塞 实现
 */
#define CFGFLASH_Read(offset,pbuf,nbytes)               sf_ReadBuffer(offset,pbuf,nbytes)
#define CFGFLASH_WritePageSequeue(offset,pbuf,nbytes)   sf_StartWritePageSequeue(offset,pbuf,nbytes)
#define CFGFLASH_EraseSectorSequeue(sector_address)     sf_StartEraseSectoreSequeue(sector_address)
#define CFGFLASHStatusBusy()                            sf_StatusBusy()

// cfg type is 2 byte
// 每个参数类型是uint16_t型
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

