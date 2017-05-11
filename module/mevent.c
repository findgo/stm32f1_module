



#include "mevent.h"

static mEv_evnents_t *mEv_head;
static mEv_evnents_t *mEv_tail;

void mEv_create(mEv_evnents_t *ev,mev_call_fn fn)
{

    ev->ev_flag = 0;
    ev->timeout = 0;
    ev->fn = fn;
    ev->next = NULL;
    
    if(mEv_head){
        mEv_tail->next = (void *)ev;
        mEv_tail = ev;
    }else{
        mEv_head = ev;
        mEv_tail= ev;
    }
}

void mEv_start_timevent(mEv_evnents_t *ev,uint16_t timeout_value)
{
    halIntState_t bintstate;
    
    ENTER_SAFE_ATOM_CODE(bintstate);
    ev->ev_flag = 1;
    ev->timeout = timeout_value;
    EXIT_SAFE_ATOM_CODE(bintstate);   
}

void mEv_start_reload_timevent(mEv_evnents_t *ev,uint16_t timeout_value)
{
    halIntState_t bintstate;
    
    ENTER_SAFE_ATOM_CODE(bintstate);
    ev->ev_flag = 1;
    ev->timeout = timeout_value;
    ev->reloadtime = timeout_value;
    EXIT_SAFE_ATOM_CODE(bintstate);   
}
void mEv_stop_timevent(mEv_evnents_t *ev)
{
    halIntState_t bintstate;
    
    ENTER_SAFE_ATOM_CODE(bintstate);
    ev->timeout = 0;
    ev->ev_flag = 0;    
    ev->reloadtime = 0;
    EXIT_SAFE_ATOM_CODE(bintstate);
}

void mEv_set_event(mEv_evnents_t *ev)
{
    halIntState_t bintstate;
    
    ENTER_SAFE_ATOM_CODE(bintstate);
    ev->timeout = 0;
    ev->ev_flag = 1;
    EXIT_SAFE_ATOM_CODE(bintstate);
}
void mEv_clear_event(mEv_evnents_t *ev)
{
    halIntState_t bintstate;
    
    ENTER_SAFE_ATOM_CODE(bintstate);
    ev->ev_flag = 0;
    EXIT_SAFE_ATOM_CODE(bintstate);
}
void mEvTimerUpdate( uint16_t ElapseTime )
{
    halIntState_t intState;
    mEv_evnents_t *srchEv;

    if(mEv_head == NULL){
        return;
    }
    srchEv = mEv_head;
    while(srchEv)
    {
        ENTER_SAFE_ATOM_CODE( intState );  // Hold off interrupts.
        if (srchEv->timeout <= ElapseTime){
            srchEv->timeout = 0;
        }else{
            srchEv->timeout = srchEv->timeout - ElapseTime;
        }

        //time out and have the event ,do it
        if((srchEv->timeout == 0) && (srchEv->ev_flag)){
            if(srchEv->reloadtime){
                srchEv->timeout = srchEv->reloadtime; // Reload the timer timeout value
            }else{
                srchEv->ev_flag = 0;
            } 
            EXIT_SAFE_ATOM_CODE( intState );  // Hold ON interrupts.
            srchEv->fn((void *)srchEv);
        }else{
            EXIT_SAFE_ATOM_CODE( intState );  // Hold ON interrupts.
        }
        srchEv = srchEv->next;
    }
}


#ifdef DEBUG_MEVENT

#include "debug.h"
#include "mclock.h"

static mEv_evnents_t mev_testreload;
static mEv_evnents_t mev_test;
static mEv_evnents_t mev_testev;

static void mEv_call_fn_reload(void *pmev);
static void mEv_call_fn(void *pmev);
static void mEv_call_fn_ev(void *pmev);

void  funcheckinit(void)
{
    dbg_set_dbg_level(7);
    mEv_create(&mev_testreload,mEv_call_fn_reload);
    mEv_create(&mev_test,mEv_call_fn);
    mEv_create(&mev_testev,mEv_call_fn_ev);

    mEv_start_reload_timevent(&mev_testreload,1000);
    mEv_start_timevent(&mev_test,2000);
    mEv_set_event(&mev_testev);
}


void funchecktask(void)
{
    uint32_t updatetime;

    updatetime= mcu_elapsedSystime();
    if(updatetime){
        mEvTimerUpdate(updatetime);
    }

}

static void mEv_call_fn_reload(void *pmev)
{
    static uint8_t count = 0;
    DBG_VERBOS("mEv_call_fn_reload happen\r\n");
    if(++count > 10){
        //mEv_stop_timevent(arg);
        mEv_clear_event(pmev);
    }
        
}
static void mEv_call_fn(void *pmev)
{
    DBG_VERBOS("mEv_call_fn happen\r\n");
}
static void mEv_call_fn_ev(void *pmev)
{
    DBG_VERBOS("mEv_call_fn_ev happen\r\n");
}


#endif
