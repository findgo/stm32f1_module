/**
  ******************************************************************************
  * @file   util_bits.h
  * @author  
  * @version 
  * @date    
  * @brief  	 λ����������λ������ֵ
  ******************************************************************************
  * @attention 	20161110     v1.1   	jgb		�Ѳ���20161111
  ******************************************************************************
  */
#ifndef __UTIL_BITS_H_
#define __UTIL_BITS_H_

#include "app_cfg.h"

/*
 * @ �������������������ͷ���
 * @ ���򣬰�λ������Ϊ����BITSTART �������� �� __LENGTH �� 		��������λ��
 * @ ���򣬰�λ��������Ϊ����BITSTART ���������� __LENGTH ��	��������λ��
 */

void writebits_U8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data);
void writebits_U16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data);
void writebits_U32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data);
void writebits_invU8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data);
void writebits_invU16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data);
void writebits_invU32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data);

uint8_t readbits_U8(uint8_t value, uint8_t bitStart, uint8_t length);
uint16_t readbits_U16(uint16_t value, uint8_t bitStart, uint8_t length);
uint32_t readbits_U32(uint32_t value, uint8_t bitStart, uint8_t length);
uint8_t readbits_invU8(uint8_t value, uint8_t bitStart, uint8_t length);
uint16_t readbits_invU16(uint16_t value, uint8_t bitStart, uint8_t length);
uint32_t readbits_invU32(uint32_t value, uint8_t bitStart, uint8_t length);

#endif


