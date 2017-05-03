/**
  ******************************************************************************
  * @file   mkey.h
  * @author   jgb
  * @version 
  * @date    
  * @brief    key模块
  ******************************************************************************
  * @attention      v1.1   	jgb		20151117重构 		
  			      v1.2        		20170306修改框架，直接返回键值
  			      v1.3             20170429再次重构，避免更多用户修改介入       
  ******************************************************************************
  */
/*
; 关于按键功能说明
; 支持短按长按功能时，触发长按时，不会触发短按效果
; 支持短按连发，触发连发时，是不会触发短按状态，虽然连发和短按键值一样，另外这种情况，连发前有个默认长延时
;
*/
#ifndef __MKEY_H_
#define __MKEY_H_

#include "app_cfg.h"

//缓冲区大小
#define KEY_QUEUE_MAX_SIZE		3
//默认滤波时间,根据key_decetor_task
#define KEY_FILTER_TIME     	2   // 滤波时间
//如果不支持长按，但支持连击，有个长延时到连击的默认时间
//如果长按和连击都不支持，直接到抬键
#define KEY_LONGTOREAP_TIME     50

typedef uint8_t (*KeyDownFunc)(void);

typedef struct {
    KeyDownFunc IsKeyDownFunc;  // 按键按下的判断函数,1表示按下 
    void *next;
    
    uint8_t filterCount;             // 滤波器计时器
    uint8_t filterTime;              // 滤波时间(如果设为0，则默认为20ms)
    uint16_t longrepCount;           // 长按连击共用计数器 
    uint16_t longTime;               // 按键按下持续时间, 0表示不检测长按 
    uint8_t repeatSpeed;             // 连击时间

    uint8_t KeyCodeUp;               // 起键键值
    uint8_t KeyCodeDown;             // 短按键值
    uint8_t KeyCodeLong;             // 长按键值
    uint8_t state;                   // 状态机
}key_cfg_t;

/*按键值 用户修改*/
enum
{
	KEY_NULL = 0,			/* 0 表示无按键事件 */	

	KEY_PRESS1_DOWN,			
	KEY_PRESS1_LONG, 		
	KEY_PRESS1_UP
};

void mkey_create(key_cfg_t *cfg,
                KeyDownFunc IsKeyDownFunc,      // 定义该函数 按键下 返回1
                uint8_t KeyCodeDown,    
                uint8_t KeyCodeLong,
                uint8_t KeyCodeUp,              // 为 KEY_NULL表示不支持抬键
                uint8_t filterTime,             // 为0 将使用默认滤波时间
                uint16_t longTime,              // 为0 表示不支持长按
                uint8_t repeatSpeed);           // 为 0 表示不支持连击

//每隔一定周期做一次键扫 一般为10ms
void mkey_decetor_task(void);
uint8_t mkey_getvalue(void);

#endif

