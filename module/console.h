/**
  ******************************************************************************
  * @file   console.h
  * @author  
  * @version 
  * @date    
  * @brief    consoleģ��
  ******************************************************************************
  * @attention 20170320     v1.1   	jgb	 ����BUG������OK
  * @attention 20170522     v1.2   	jgb	 �޸��ⲿ�ӿ�API,�����û����룬ע����ָ�������
  ******************************************************************************
  */
/*
 *  ʵ��consol readbyte writebyte writestring  init�Ľӿ�
 *  ��ʼ�� console_init
 *  ��ѭ������ console_task
 *  ע���Զ������� console_cmd_register
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
bool console_cmd_register(console_command_t *ptCmd,   // ����һ��������
                            const char *cmd,                // ��������
                            CONSOLE_CMD_HANDLER *cmd_handler, // ������������ص�����
                            const char *help);                  // ������������ַ���
void console_task(void);

#endif

