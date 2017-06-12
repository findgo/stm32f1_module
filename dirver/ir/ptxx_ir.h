
/**
  ******************************************************************************
  * @file   ptxx.h
  * @author  
  * @version 
  * @date    
  * @brief    本模块是用于PT2622和PT2624等ptxx系列编码芯片的解码
  ******************************************************************************
  * @attention      v1.0   	jgb		创建                                    20170520
  ******************************************************************************
  */
/*
 * 本模块抽象底层接口，仅用较少的宏 和API解码ptxx的编码
 */

#ifndef __PTXX_IR__
#define __PTXX_IR__


#include "dev.h"			// 包含驱动层头文件
#include "global.h"
#include "includes.h"

/* 定义数据和地址的位数 
* 数据位可以为0 - 6位
*
*/
#define PTXX_DATA_BIT_NUM (4)
#define PTXX_ADDR_BIT_NUM (12 - PTXX_DATA_BIT_NUM)

// 定义一个周期的时间计数数个数 规格书P4
#define PTXX_TIME_PULSE_CYCLE  (120)

void PTxx_Capture_Decode(u32 delta_time);
u8 PTxx_GetCode(u32 *addr,u8 *cmd);

#define IR_GetKeyCode(addr,cmd) PTxx_GetCode(addr,cmd)


#endif
