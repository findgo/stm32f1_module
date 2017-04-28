
#include "util_bits.h"
/*
 * @ �������������������ͷ���
 * @ ���򣬰�λ������Ϊ����BITSTART �������� �� __LENGTH �� 		��������λ��
 * @ ���򣬰�λ��������Ϊ����BITSTART ���������� __LENGTH ��	��������λ��
 */

#define WRITEBITS_UINTx( __VALUE, __BITSTART, __LENGTH, __DATA, __TYPE) \
{																		\
	__TYPE bytes;														\
	__TYPE mask;														\
																		\
	bytes = __VALUE;													\
	mask = ((1 << __LENGTH) - 1) << __BITSTART; 						\
	__DATA <<= __BITSTART; /* shift data into correct position*/ 				\
	__DATA &= mask; /* zero all non-important bits in data*/					\
	bytes &= ~(mask); /* zero all important bits in existing byte*/			\
	bytes |= __DATA; /* combine data with existing byte */					\
																		\
	__VALUE = bytes; 													\
}
#define WRITEBITS_INVUINTx( __VALUE, __BITSTART, __LENGTH, __DATA, __TYPE) 	\
{																		\
	__TYPE bytes;														\
	__TYPE mask;														\
																		\
	bytes = __VALUE;													\
	mask = ((1 << __LENGTH) - 1) << (__BITSTART - __LENGTH + 1); 			\
	__DATA <<= (__BITSTART - __LENGTH + 1); /* shift data into correct position*/ \
	__DATA &= mask; /* zero all non-important bits in data*/					\
	bytes &= ~(mask); /* zero all important bits in existing byte*/			\
	bytes |= __DATA; /* combine data with existing byte */					\
																		\
	__VALUE = bytes; 													\
}
#define READBITS_UINTx( __VALUE, __BITSTART, __LENGTH, __TYPE) 			\
{																		\
	__TYPE bytes;														\
	__TYPE mask;														\
																		\
	bytes = __VALUE;													\
	mask = ((1 << __LENGTH) - 1); 										\
	bytes >>= __BITSTART;												\
	bytes &= mask; /* zero all non-important bits in data*/					\
																		\
	return bytes;														\
}
#define READBITS_INVUINTx( __VALUE, __BITSTART, __LENGTH, __TYPE) 		\
{																		\
	__TYPE bytes;														\
	__TYPE mask;														\
																		\
	bytes = __VALUE;													\
	mask = ((1 << __LENGTH) - 1); 										\
	bytes >>= (__BITSTART - __LENGTH + 1);								\
	bytes &= mask; /* zero all non-important bits in data*/					\
																		\
	return bytes;														\
}


/**
  * @brief  ��value�ڴ������ bitstartλ���𳤶�length��λ������Ϊ����data 
  * @param  value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note   ����: ����       ���data���ݳ�������ᱻ�ض�
  * @retval none
  */
void writebits_U8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data)
{
	WRITEBITS_UINTx(*value, bitStart, length, data, uint8_t)
}
/**
  * @brief  
  * @param   value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note    ����: ����
  * @retval 
  */
void writebits_U16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data)
{
	WRITEBITS_UINTx(*value, bitStart, length, data, uint16_t)
}
/**
  * @brief  
  * @param   value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note    ����: ����
  * @retval 
  */
void writebits_U32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data)
{
	WRITEBITS_UINTx(*value, bitStart, length, data, uint32_t)
}

/**
  * @brief  ��value�ڴ������ bitstartλ���𳤶�length��λ������Ϊ����data 
  * @param  value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note    ����: ����       ���data���ݳ�������ᱻ�ض�
  * @retval none
  */
void writebits_invU8(uint8_t *value, uint8_t bitStart, uint8_t length, uint8_t data)
{
	WRITEBITS_INVUINTx(*value, bitStart, length, data, uint8_t)
}
/**
  * @brief  
  * @param   value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note    ����: ����
  * @retval 
  */
void writebits_invU16(uint16_t *value, uint8_t bitStart, uint8_t length, uint16_t data)
{
	WRITEBITS_INVUINTx(*value, bitStart, length, data, uint16_t)
}

/**
  * @brief  
  * @param   value Ҫ�ĵ����� bitStart ��ʼλ length ����λ data ����
  * @note    ����: ����
  * @retval 
  */
void writebits_invU32(uint32_t *value, uint8_t bitStart, uint8_t length, uint32_t data)
{
	WRITEBITS_INVUINTx(*value, bitStart, length, data, uint32_t)
}

/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint8_t readbits_U8(uint8_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_UINTx( value, bitStart, length, uint8_t)
}
/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint16_t readbits_U16(uint16_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_UINTx( value, bitStart, length, uint16_t)
}

/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint32_t readbits_U32(uint32_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_UINTx( value, bitStart, length, uint32_t)
}
/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint8_t readbits_invU8(uint8_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_INVUINTx( value, bitStart, length, uint8_t)
}
/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint16_t readbits_invU16(uint16_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_INVUINTx( value, bitStart, length, uint16_t)
}

/**
  * @brief  
  * @param   value Ҫ�������� bitStart ��ʼλ length ����λ
  * @note    ����: ����
  * @retval  ֵ
  */
uint32_t readbits_invU32(uint32_t value, uint8_t bitStart, uint8_t length)
{
	READBITS_INVUINTx( value, bitStart, length, uint32_t)
}
