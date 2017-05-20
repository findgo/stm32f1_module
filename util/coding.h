
#ifndef __CODING_H_
#define __CODING_H_

#include "app_cfg.h"

// for coding 先低8位，再高8位
uint8_t encode_signed8(uint8_t *pdu, int8_t value);
uint8_t encode_signed16(uint8_t *pdu, int16_t value);
uint8_t encode_signed24(uint8_t *pdu, int32_t value);
uint8_t encode_signed32(uint8_t *pdu, int32_t value);

uint8_t encode_unsigned16(uint8_t *pdu, uint16_t value);
uint8_t encode_unsigned24(uint8_t *pdu, uint32_t value);
uint8_t encode_unsigned32(uint8_t *pdu, uint32_t value);

uint8_t decode_signed8(const uint8_t *pdu, int32_t *value);
uint8_t decode_signed16(const uint8_t *pdu, int32_t *value);
uint8_t decode_signed24(const uint8_t *pdu, int32_t *value);
uint8_t decode_signed32(const uint8_t *pdu, int32_t *value);

uint8_t decode_unsigned16(const uint8_t *pdu, uint16_t *value);
uint8_t decode_unsigned24(const uint8_t *pdu, uint32_t *value);
uint8_t decode_unsigned32(const uint8_t *pdu, uint32_t *value);


#endif //__CODING_H_

