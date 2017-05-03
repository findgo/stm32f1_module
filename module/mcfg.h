/**
  ******************************************************************************
  * @file   mcfg.h
  * @author  
  * @version 
  * @date    
  * @brief    cfg 模块用于裸奔程序动态储存配置，主要是flash擦，写时间过长，采用一定的管理，使程序不阻塞
  * @    依赖 mtimers.h para_cfg.h
  ******************************************************************************
  * @attention      v1.0   	jgb   2010503   未测试
  ******************************************************************************
  */


/* 使用说明 
 * 首先在para.c定义参数个数
 * 实现para.c的两个函数和一个定义 
 * 根据使用的flash 定义使用的扇区大小(一般为4K)， 使用多少个扇区作为配置,作为配置的起始地址
 */
#ifndef __MCFG_H_
#define __MCFG_H_

#include "app_cfg.h"


// cfg 扇区起始地址，默认(0)从flash 扇区头开始
#define CFG_FLASH_ADDRESS_START  (0)
// 使用的flash扇区大小
#define CFG_FLASH_SECTOR_SIZE  (4096)
// 给cfg使用的flash扇区个数
#define CFG_FLASH_SECTOR_USE_NUM  (8)


// 按扇区分配(扇大小 * CFG_FLASH_SECTOR_USE_NUM)
#define CFG_FLASH_USE_SIZE  (CFG_FLASH_SECTOR_SIZE * CFG_FLASH_SECTOR_USE_NUM)

/*
 * 下面宏是FLASH的都是页内的操作函数，不会出现跨页情况
 * 以下要求是无阻塞 实现
 */
#define CFGFLASH_READ(offset,pbuf,nbytes) 
#define CFGFLASH_WRITE(offset,pbuf,nbytes)
#define CFGFLASH_ERASE(sector_offset)
#define CFGFLASHSTATUSBUSY()


void mcfgLoadinit(void);
void mcfgUpdate(void);


#endif

