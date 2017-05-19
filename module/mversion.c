

#include "mversion.h"


const char *months[] = {"Jan","Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};


uint8_t mver_getmajorver(void)
{
    return FIRMWARE_MAJOR_VER;
}

/*
* __DATE__ : May 12 2017  月日年
* __TIME__ : 15:26:26     时分秒
*
*/
uint32_t mver_getminorver(void)
{
    uint8_t i,lengh;
		uint32_t ver;
    char *pstr = __DATE__;

    lengh = strlen(pstr);
    ver = pstr[lengh - 4] - 0x30;
    ver = ver * 10 + pstr[lengh - 3] - 0x30;
    ver = ver * 10 + pstr[lengh - 2] - 0x30;
    ver = ver * 10 + pstr[lengh - 1] - 0x30;
    	
    for(i = 0;i < sizeof(months)/sizeof(months[0]); i++){
        if(memcmp(months[i],pstr,3) == 0){
            ver = ver * 100 + i + 1;
            break;
        }
    }

    lengh = 4;
    ver = ver * 10 + (pstr[lengh] == ' ' ? 0 : (pstr[lengh] - 0x30));
    ver = ver * 10 + pstr[lengh + 1] - 0x30;

    pstr = __TIME__;
    ver = ver * 10 + (pstr[0] == ' ' ? 0 : (pstr[0] - 0x30));
    ver = ver * 10 + pstr[1] - 0x30;
    
    return ver;
}


#ifdef MVER_DEBUG
#include "debug.h"


void FunDebuginit(void)
{
    dbg_set_dbg_level(7);
    DBG_VERBOS("%d.%d\r\n",mver_getmajorver(),mver_getminorver());
    return;
}

void FunDebugtask(void)
{
    return;
}


#endif
