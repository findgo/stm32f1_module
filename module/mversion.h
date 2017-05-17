
/**
  ******************************************************************************
  * @file   mversion.h
  * @author  
  * @version 
  * @date    
  * @brief   根据软件编译时间生成次版本号 十进制，格式，年月日时
  ******************************************************************************
  * @attention 	  v1.0    jgb            20170502
  ******************************************************************************
  */

#ifndef __MVERSION_H_
#define __MVERSION_H_

#include "app_cfg.h"

// 定义主版本号
#define FIRMWARE_MAJOR_VER  (1)

/* 主版本号 */
uint8_t mver_getmajorver(void);
/* 次版本号 10进制年月日时 */
uint32_t mver_getminorver(void);

#endif /* __MVERSION_H_ */


