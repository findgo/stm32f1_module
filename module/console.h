/**
  ******************************************************************************
  * @file   console.h
  * @author  
  * @version 
  * @date    
  * @brief    console模块
  ******************************************************************************
  * @attention 20170320     v1.1   	jgb	 修正BUG，测试OK
  * @attention 20170522     v1.2   	jgb	 修改外部接口API,减少用户介入，注册新指令更容易
  ******************************************************************************
  */
/*
 *  实现consol readbyte writebyte writestring  init的接口
 *  初始化 console_init
 *  大循环调用 console_task
 *  注册自定义命令 console_cmd_register
 *  
 */
#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "app_cfg.h"
#include "stdlib.h"
#include "mserial.h"


//! \brief argv max point Size
#define CONSOLE_ARGV_MAX_SIZE			(10)
//! \brief command Buffer max Size
#define CONSOLE_BUFFER_SIZE		        (20)
//! \brief histoty command number to save
#define CONSOLE_HIS_CMD_NUM    			(5u)

//! \name console dynamic command type define
typedef void CONSOLE_CMD_HANDLER(uint8_t argc, char *argv[], void *pcmd);

typedef struct {
	const char *pchCMD;
	CONSOLE_CMD_HANDLER *fncmd_handler;
	const char *pchHelp;
    void *next;
}console_command_t;


//!  serial in a character
#define console_readbyte(pchByte)	Serial_Read(COM0,pchByte, 1)
//! serial out a character
#define console_writebyte(chByte)  Serial_WriteByte(COM0,chByte)
//! serial out a string
#define console_writestring(pstring)	Serial_WriteStr(COM0,pstring)

#define console_extern_init()


void console_init(void);
bool console_cmd_register(console_command_t *ptCmd,   // 定义一个命令句柄
                            const char *cmd,                // 定义命令
                            CONSOLE_CMD_HANDLER *cmd_handler, // 定义命令解析回调函数
                            const char *help);                  // 定义命令帮助字符串
void console_task(void);

#endif

