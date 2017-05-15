/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief    cfg 模块用于裸奔程序动态储存配置，主要是使用spi flash存少量参数，对付擦，写时间过长，采用一定的管理，使程序不阻塞
  * @       依赖 mtimers.h mcfg_defaultpara.c
  ******************************************************************************
  * @attention      v1.0   	jgb   20170503   未测试
  ******************************************************************************
  */


/* 
 * NOTE:    由于SPI flash有寿命会出现坏块，本工程实现坏块跳过处理。
 * 
 * 使用说明 
 * 首先定义参数结构体mcfg_para_t的参数
 * 实现mcfg_defaultpara.c中根据mcfg_para_t 配置默认参数
 *
 * 实现flash的扇区擦除, 页内写，任意地址读,忙状态检测  四个函数
 * 根据使用的spi flash 定义使用的扇区大小(一般为4K),并设定配置的起始地址
 * spi FLASH采用的是扇区擦除, 页内可随意编写0 - 256 字节
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

// 给CFG所有的存储总量(sector size * sector number)
#define CFG_FLASH_USE_TOTAL_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_USE_SECTOR_NUM)

// 配置改变和刷新时间，5秒钟
#define CFG_FLUSH_TIME          ((uint32_t)5000)

// 定义CRC32计算的初始值
#define CFG_CRC32_INIT_VALUE    0xffffffff
/*
 * 以下要求是无阻塞 实现
 */
#define CFGFLASH_Read(offset,pbuf,nbytes) 
#define CFGFLASH_WritePage(offset,pbuf,nbytes)
#define CFGFLASH_Erase(sector_address)
#define CFGFLASHStatusBusy()    0




// cfg type is 2 byte
// 每个参数类型是uint16_t型
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

