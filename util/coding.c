#include "coding.h"


uint8_t encode_signed8(uint8_t *pdu, int8_t value)
{
    pdu[0] = (uint8_t)value;

    return 1;
}

uint8_t encode_signed16(uint8_t *pdu, int16_t value)
{
    pdu[0] = (uint8_t)(value & 0x00ff);
    pdu[1] = (uint8_t)((value & 0xff00) >> 8);

    return 2;
}

uint8_t encode_signed24(uint8_t *pdu, int32_t value)
{
    pdu[0] = (uint8_t)(value & 0x0000ff);
    pdu[1] = (uint8_t)((value & 0x00ff00) >> 8);
    pdu[2] = (uint8_t)((value & 0xff0000) >> 16);
    
    return 3;
}

uint8_t encode_signed32(uint8_t *pdu, int32_t value)
{
    pdu[0] = (uint8_t)(value & 0x000000ff);
    pdu[1] = (uint8_t)((value & 0x0000ff00) >> 8);
    pdu[2] = (uint8_t)((value & 0x00ff0000) >> 16);
    pdu[3] = (uint8_t)((value & 0xff000000) >> 24);
    
    return 4;
}

uint8_t encode_unsigned16(uint8_t * apdu,uint16_t value)
{
    apdu[0] = (uint8_t) (value & 0x00ff);
    apdu[1] = (uint8_t) ((value & 0xff00) >> 8);
    
    return 2;
}

uint8_t encode_unsigned24(uint8_t *pdu, uint32_t value)
{
    pdu[0] = (uint8_t)(value & 0x0000ff);
    pdu[1] = (uint8_t)((value & 0x00ff00) >> 8);
    pdu[2] = (uint8_t)((value & 0xff0000) >> 16);
    
    return 3;
}

uint8_t encode_unsigned32(uint8_t * apdu,uint32_t value)
{
    apdu[0] = (uint8_t) (value & 0x000000ff);
    apdu[1] = (uint8_t) ((value & 0x0000ff00) >> 8);
    apdu[2] = (uint8_t) ((value & 0x00ff0000) >> 16);
    apdu[3] = (uint8_t) ((value & 0xff000000) >> 24);

    return 4;
}

uint8_t decode_signed8(const uint8_t *pdu, int32_t *value)
{
    if (value) {
        if (pdu[0] & 0x80) {                /* negative - bit 7 is set */
            *value = 0xFFFFFF00;
        } else {
            *value = 0;
        }
        
        *value |= ((int32_t)(((int32_t)pdu[0]) & 0x000000ff));
    }

    return 1;
}

uint8_t decode_signed16(const uint8_t *pdu, int32_t *value)
{
    if (value) {
        if (pdu[0] & 0x80) {                /* negative - bit 7 is set */
            *value = 0xFFFF0000;
        } else {
            *value = 0;
        }
        
        *value |= ((int32_t)(((int32_t)pdu[0]) & 0x000000ff));  
        *value |= ((int32_t)((((int32_t)pdu[1]) << 8) & 0x0000ff00));
    }

    return 2;
}

uint8_t decode_signed24(const uint8_t *pdu, int32_t *value)
{
    if (value) {
        if (pdu[0] & 0x80) {                /* negative - bit 7 is set */
            *value = 0xFF000000;
        } else {
            *value = 0;
        }
        *value |= ((int32_t)(((int32_t)pdu[0]) & 0x000000ff));        
        *value |= ((int32_t)((((int32_t)pdu[1]) << 8) & 0x0000ff00));        
        *value |= ((int32_t)((((int32_t)pdu[2]) << 16) & 0x00ff0000));
    }

    return 3;
}

uint8_t decode_signed32(const uint8_t *pdu, int32_t *value)
{
    if (value) {
        *value = ((int32_t)(((int32_t)pdu[0]) & 0x000000ff));
        *value |= ((int32_t)((((int32_t)pdu[1]) << 8) & 0x0000ff00));
        *value |= ((int32_t)((((int32_t)pdu[2]) << 16) & 0x00ff0000));
        *value |= ((int32_t)((((int32_t)pdu[3]) << 24) & 0xff000000));
    }

    return 4;
}

uint8_t decode_unsigned16(const uint8_t * apdu,uint16_t * value)
{
    if (value) {   
        *value = ((uint16_t) (((uint16_t) apdu[0]) & 0x00ff));
        *value |= (uint16_t) ((((uint16_t) apdu[1]) << 8) & 0xff00);
    }

    return 2;
}

uint8_t decode_unsigned24(const uint8_t *pdu, uint32_t *value)
{
    if (value) {
        *value = ((uint32_t)(((uint32_t)pdu[2]) & 0x000000ff));
        *value |= (uint32_t)((((uint32_t)pdu[1]) << 8) & 0x0000ff00);
        *value |= ((uint32_t)((((uint32_t)pdu[0]) << 16) & 0x00ff0000));
    }

    return 3;
}

uint8_t decode_unsigned32(const uint8_t * apdu,uint32_t * value)
{
    if(value) {
        *value = ((uint32_t) (((uint32_t) apdu[0]) & 0x000000ff));
        *value |= ((uint32_t) ((((uint32_t) apdu[1]) << 8) & 0x0000ff00));
        *value |= ((uint32_t) ((((uint32_t) apdu[2]) << 16) & 0x00ff0000));  
        *value |= ((uint32_t) ((((uint32_t) apdu[3]) << 24) & 0xff000000));
    }

    return 4;
}

