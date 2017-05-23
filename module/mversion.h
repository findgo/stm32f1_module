
/**
  ******************************************************************************
  * @file   mversion.h
  * @author  
  * @version 
  * @date    
  * @brief   根据软件编译时间生成次版本号 十进制，格式，年月日时
  ******************************************************************************
  * @attention 	  v1.0    jgb                                       20170502
  * @attention 	  v1.1    jgb      增加批次版本号 用于发布          20170502
  ******************************************************************************
  */
/*
 *  关于版本号管理  
 * 主版本为 无大规模修改时定义的版本  
 * 批次号为 发布生成的批次版本号   NOTE:   0 : 强制定义为内部测试版本
 * 次版号为 该软件编译生成时间,用于区分同版本同批次软件的区别
 */


#ifndef __MVERSION_H_
#define __MVERSION_H_

#include "app_cfg.h"

// 定义主版本号
#define FIRMWARE_MAJOR_VER   (1)
// 定义批次号
#define FIRMWARE_BATCH_VER   (0)


/* 主版本号 批次号 高4位主版号,底4位次版本号*/
#define mver_getmajorver()  ((FIRMWARE_MAJOR_VER << 4) | FIRMWARE_BATCH_VER)

/* 次版本号 10进制年月日时 ,例2017050213 -- 2017年 5 月 2日 13时 编译*/
uint32_t mver_getminorver(void);




#endif /* __MVERSION_H_ */


