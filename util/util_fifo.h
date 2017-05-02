/**
  ******************************************************************************
  * @file   util_queue.h
  * @author  
  * @version 
  * @date    
  * @brief  	单字节数据队列
  ******************************************************************************
  * @attention      v1.1   	jgb		重构		        20151119 已测试
  * @attention      v1.02   	jgb		修改增加函数命名		20170420 已测试
  ******************************************************************************
  */
#ifndef __UTIL_QUEUE_H_
#define __UTIL_QUEUE_H_

#include "app_cfg.h"

typedef struct
{							
	uint8_t	*ptBuffer;   
	volatile uint16_t tHead; 
	volatile uint16_t tTail;
	volatile uint16_t tSize; 
	volatile uint16_t tCounter; 
}fifo_buf_t;	

bool fifo_init(fifo_buf_t *ptQ, uint8_t *ptBuffer, uint16_t tSize);
bool fifo_flush(fifo_buf_t *ptQ);
uint16_t fifo_put(fifo_buf_t *ptQ, uint8_t tObj);
uint16_t fifo_put_front(fifo_buf_t *ptQ, uint8_t tObj);
uint16_t fifo_pop(fifo_buf_t *ptQ, uint8_t *ptObj);
bool fifo_peek(fifo_buf_t *ptQ, uint8_t *ptObj);
uint16_t fifo_bufput(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length);
uint16_t fifo_bufpop(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length);
uint16_t fifo_count(fifo_buf_t *ptQ);
bool fifo_empty(fifo_buf_t *ptQ);
bool fifo_full(fifo_buf_t *ptQ);


#endif

