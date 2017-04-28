/**
  ******************************************************************************
  * @file   mod_console.h
  * @author  
  * @version 
  * @date    
  * @brief    consoleÄ£¿é
  ******************************************************************************
  * @attention 20170320     v1.1   	jgb	 ÐÞÕýBUG£¬²âÊÔOK
  ******************************************************************************
  */

#ifndef __CONSOLE_H_
#define __CONSOLE_H_

#include "app_cfg.h"
#include "stdlib.h"

#include "mserial.h"


#ifdef DEBUG_MSG
extern void cmd_debug_control(uint8_t argc, char *argv[], void *pcmd);
#define CMD_DEBUG_ADDTOLIST {"debug", &cmd_debug_control, "\tdebug\t-- debug level control"},
#else
#define CMD_DEBUG_ADDTOLIST 
#endif

// extend command
extern void cmd_debug_control(uint8_t argc, char *argv[], void *pcmd);


//! \brief argv max point Size
#define CONSOLE_ARGV_MAX_SIZE			(10)
//! \brief Buffer Size
#define CONSOLE_BUFFER_SIZE		        (20)
//! \brief histoty command number
#define CONSOLE_HIS_CMD_NUM    			(5u)

//! \name console dynamic command type define
typedef void CONSOLE_CMD_HANDLER(uint8_t argc, char *argv[], void *pcmd);

typedef struct {
	char *pchCMD;
	CONSOLE_CMD_HANDLER *fncmd_handler;
	char *pchHelp;
}console_command_t;


//!  serial in a character
#define console_readbyte(pchByte)	Serial_Read(pchByte, 1)
//! serial out a character
#define console_writebyte(chByte)  Serial_WriteByte(chByte)
//! serial out a string
#define console_writestring(pstring)	Serial_WriteStr(pstring)

#define console_extern_init()

//! \brief Default Static Command : "help" "clear" and '?' you can add command at here
#ifndef CMD_SET_EXTERN_LIST
#define CMD_SET_EXTERN_LIST CMD_DEBUG_ADDTOLIST
#endif

bool console_str_cpy(char *dst, const char *src);
bool console_str_cmp(char *pchString1, char *pchString2);
uint8_t console_str_len(char *pchString);

void console_init(void);
bool console_cmd_register(const console_command_t *ptCmdList, uint8_t CmdListCount);
void console_task(void);

#endif

