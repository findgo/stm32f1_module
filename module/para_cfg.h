


#ifndef __PARA_CFG_H_
#define __PARA_CFG_H_


#include "app_cfg.h"


// cfg number you have 
// cfg type is 2 byte
// ÿ������������uint16_t��
#define CFG_SAVE_PARA_NUM    (1)




void para_setbybuf(const uint16_t *buf);
void para_gettobuf(uint16_t *buf);



#endif

