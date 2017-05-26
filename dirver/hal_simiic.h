

/* 本模块未测试 */
#ifndef __HAL_SIMIIC_H_
#define __HAL_SIMIIC_H_

#include "app_cfg.h"


// 用于读内部寄存器?
#define IICDEV_NO_MEM_ADDR	0xff

void IIC_Init(void);
bool IICreadBuf(uint8_t devAddress, uint8_t memAddress, uint8_t length, uint8_t *data);
bool IICreadByte(uint8_t devAddress, uint8_t memAddress, uint8_t *data);
bool IICreadBit(uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t *data);
bool IICreadBits(uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t *data);
bool IICwriteBuf(uint8_t devAddress, uint8_t memAddress, uint8_t length, uint8_t* data);
bool IICwriteByte(uint8_t devAddress, uint8_t memAddress, uint8_t data);
bool IICwriteBits(uint8_t devAddress, uint8_t memAddress, uint8_t bitStart, uint8_t length, uint8_t data);
bool IICwriteBit(uint8_t devAddress, uint8_t memAddress, uint8_t bitNum, uint8_t data);


#endif

//------------------End of File----------------------------







