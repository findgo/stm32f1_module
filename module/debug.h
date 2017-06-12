/**
  ******************************************************************************
  * @file   debug.h
  * @author  
  * @version 
  * @date    
  * @brief  提供对程序运行间调试接口
  * @       依赖 外部串口映射
  ******************************************************************************
  * @attention      v1.0   	jgb   20160503     测试
  *                 v1.1    jgb   20170516    提供重映射接口，提供DBG_VERBOSBUF接口
  *                 
  ******************************************************************************
  */
/*
 * debug 调试模块，使用宏需要开启宏DEBUG_MSG
 * 配置重定义fput,直接可在头文件 配置REDIR_FPUT(ch)的宏
 * 默认只开启 err, 而warn，verbos不开启。
 * 
 */


#ifndef __DEBUG_H__
#define __DEBUG_H__

#include "app_cfg.h"
#include "mserial.h"

#define DEBUG_LEVEL_VERBOS  0x01
#define DEBUG_LEVEL_WARN    0x02
#define DEBUG_LEVEL_ERROR   0x04

#ifdef DEBUG_MSG

//重定向fput  用户定义
#define REDIR_FPUT(ch)   (void)Serial_WriteByte(COM0,ch);
// 重定向输出buf
#define REDIR_FPUTBUF(buf,len) (void)Serial_WriteBuf(COM0,buf,len)

extern bool dbg_verbos;
extern bool dbg_warn;
extern bool dbg_err;

#define DBG_VERBOS(fmt, args...)                    \
	do {                                                \
	    if (dbg_verbos) {                           \
	        printf(fmt, ##args);                        \
	    }                                               \
	} while (0)

#define DBG_WARN(fmt, args...)                      \
	do {                                                \
	    if (dbg_warn) {                             \
	        printf(fmt, ##args);                        \
	    }                                               \
	} while (0)

#define DBG_ERR(fmt, args...)                     \
    do {                                                \
        if (dbg_err) {                              \
            printf(fmt, ##args);                        \
        }                                               \
    } while (0)

#define DBG_VERBOSBUF(buf,len)						\
	do {                                                \
	    if (dbg_verbos) {                           \
			REDIR_FPUTBUF(buf,len);                     \
	    }                                               \
	} while (0)
#else
#define DBG_VERBOS(fmt, args...)
#define DBG_WARN(fmt, args...)
#define DBG_ERR(fmt, args...)
#define DBG_VERBOSBUF(buf,len)
#endif

void dbg_set_dbg_level(uint8_t level);
void dbg_print_dbg_level(void);
void  FunDebuginit(void);
void  FunDebugtask(void);

#endif

