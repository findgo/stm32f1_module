

#include "mversion.h"

/*
* __DATE__ : May 12 2017  月日年
* __TIME__ : 15:26:26     时分秒
*
*/
const char *months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};

uint32_t mver_getver(void)
{
    uint8_t i,lengh;
		uint32_t ver;
    char *pdt = __DATE__;

    lengh = strlen(pdt);
    ver = pdt[lengh - 4] - 0x30;
    ver = ver * 10 + pdt[lengh - 3] - 0x30;
    ver = ver * 10 + pdt[lengh - 2] - 0x30;
    ver = ver * 10 + pdt[lengh - 1] - 0x30;
    	
    for(i = 0;i < sizeof(months)/sizeof(months[0]); i++){
        if(memcmp(months[i],pdt,3) == 0){
            ver = ver * 100 + i + 1;
            break;
        }
    }

    lengh = 4;
    ver = ver * 10 + (pdt[lengh] == ' ' ? 0 : (pdt[lengh] - 0x30));
    ver = ver * 10 + pdt[lengh + 1] - 0x30;

    pdt = __TIME__;
    ver = ver * 10 + (pdt[0] == ' ' ? 0 : (pdt[0] - 0x30));
    ver = ver * 10 + pdt[1] - 0x30;
    
    return ver;
}


#ifdef MVER_DEBUG
#include "debug.h"


void funcheckinit(void)
{
    dbg_set_dbg_level(7);
    DBG_VERBOS("%d\r\n",mver_getver());
    return;
}

void funchecktask(void)
{
    return;
}


#endif
