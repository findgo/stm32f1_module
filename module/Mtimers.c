

#include "mtimers.h"

#define MTIMER_START(t,timeout)	do {\
                            t->start = mcu_getCurSysctime(); \
                            t->timeout = timeout; \
                            t->enable = true; \
                        }while(0)


void mtimer_start(mtimer_t *t,uint32_t timeout)
{
    MTIMER_START(t,timeout);
}

void mtimer_seconds_start(mtimer_t *t,uint32_t seconds)
{
    uint32_t timeout = (seconds * 1000L) / CLOCKTICK_RATE_MS;

    MTIMER_START(t,timeout); 
}
void mtimer_minutes_start(mtimer_t *t,uint32_t minutes)
{    
    uint32_t timeout = (minutes * 60L * 1000L) / CLOCKTICK_RATE_MS;

    MTIMER_START(t,timeout); 
}

void mtimer_del(mtimer_t *t)
{
    t->enable = false;
}
bool mtimer_expired(mtimer_t *t)
{
    uint32_t now = mcu_getCurSysctime();

    if(t->enable && timer_after_eq(now, t->start + t->timeout)){
		t->enable = false;
      	return true;  
	}
    
    return false;
}



#ifdef DEBUG_MTIMER
#include "debug.h"

static mtimer_t atest_mtimer = MTIMER_INIT();
static mtimer_t btest_mtimer ;
DELCARE_MTIMER(ctest_mtimer);

void  funcheckinit(void)
 {
 	dbg_set_dbg_level(7);
	INIT_MTIMER(&atest_mtimer);
	mtimer_start(&test_mtimer,tm_s(1));
	mtimer_start(&test1_mtimer,tm_s(15));
	mtimer_start(&test2_mtimer,tm_s(5));
	mtimer_del(&test2_mtimer);
}


void  funchecktask(void)
{
	if(mtimer_expired(&atest_mtimer)){
		DBG_VERBOS("%ld ms is comming!\r\n",tm_s(1));
		mtimer_start(&atest_mtimer,tm_s(1));
	}
	if(mtimer_expired(&btest_mtimer)){
		DBG_VERBOS("%ld ms is comming!\r\n",tm_s(15));
		mtimer_start(&btest_mtimer,tm_s(15));
	}
	if(mtimer_expired(&ctest_mtimer)){
		DBG_VERBOS("%ld ms is comming!\r\n",tm_s(5));
		mtimer_start(&ctest_mtimer,tm_s(5));
	}

}


#endif

