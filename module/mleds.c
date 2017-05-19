
#include "mleds.h"


typedef struct ledseq_t{
    uint8_t     eve;
    uint8_t     todo;       /* 眨现次数，如果设置为0xff，表明无限闪烁*/
    uint8_t     onPct;      /* 占空比*/
    uint16_t    period; /*开/关总周期 */    
    uint16_t    next;    /* 下一次改变的时间/与流逝时间相关 ，与时基绑定*/
}ledseq_t;

static uint8_t PreledsOnOffstatus;
static ledseq_t ledseqControl[mLEDSNUM];

const ledseq_t ledseqeveList[LEDSEQ_EVE_MAX]= {
    {LEDSEQ_EVE_NONE, 0, 0, 0, 0},
    {LEDSEQ_EVE_BLINK_ONCE,     1,                   LED_SEQ_DEFAULT_BLINK_DUTY_CYCLE,         LED_SEQ_DEFAULT_FLASH_TIME, 0},
    {LEDSEQ_EVE_FLASH_BLINK,    LED_SEQ_ALWAYS_TODO, LED_SEQ_DEFAULT_BLINK_DUTY_CYCLE,         LED_SEQ_DEFAULT_FLASH_TIME, 0},
    {LEDSEQ_EVE_FLASH_QUARTER,  LED_SEQ_ALWAYS_TODO, LED_SEQ_DEFAULT_FLASH_QUARTER_DUTY_CYCLE, LED_SEQ_DEFAULT_FLASH_TIME, 0},
    {LEDSEQ_EVE_FLASH_HALF,     LED_SEQ_ALWAYS_TODO, LED_SEQ_DEFAULT_FLASH_HALF_DUTY_CYCLE,    LED_SEQ_DEFAULT_FLASH_TIME, 0},
    LEDSEQ_EVE_EXTERN_LSIT
};

/**
  * @brief  初始化
  * @param  None
  * @note   
  * @retval  None
  */
void ledseqInit(void)
{
    uint8_t i;
    
    for(i = 0; i < mLEDSNUM; i++){
        ledseqControl[i].eve = LEDSEQ_EVE_NONE;
        ledseqControl[i].onPct = 0;
        ledseqControl[i].todo = 0;
        ledseqControl[i].period = LED_SEQ_DEFAULT_FLASH_TIME;
        ledseqControl[i].next = 0;
    }
    ledseqOnOff( MLED_ALL, false);// 所有灯关
}

/**
  * @brief  
  * @param  leds 哪个灯，位域法，可同时设置几个灯   
  * @note       eve 为 LEDSEQ_EVE_NONE，灯状态有mode 决定(当设置 LEDSEQ_MODE_NONE )
  * @note       eve 不为 LEDSEQ_EVE_NONE，忽略参数mode，一般设置为 LEDSEQ_MODE_NONE
  * @retval  None
  */

void ledseqset(uint8_t leds, ledseq_eve_t eve, ledseq_mode_t mode)
{
    uint8_t led;
    ledseq_t *sts;

    led = MLED_1;           //从第一个灯开始查
    leds &= MLED_ALL;       //屏蔽不在驱动内的灯
    sts = &ledseqControl[0];
    while (leds)    //遍历找出想要设置的灯
    {
        if (leds & led){
            if (eve == LEDSEQ_EVE_NONE){
                if(mode != LEDSEQ_MODE_TOGGLE){
                    if(PreledsOnOffstatus & led){
                        if(mode == LEDSEQ_MODE_OFF){
                            ledseqOnOff (led, false); //动作
                        }
                    }else{
                        if(mode == LEDSEQ_MODE_ON){
                            ledseqOnOff (led, true); //动作
                        }
                    }
                 }else{
                    ledseqOnOff(led, (PreledsOnOffstatus ^ led) & led);// 取反 do it
                 }
                 sts->eve = LEDSEQ_EVE_NONE;
            }else{
                if(eve != sts->eve){//diferent from current
                    sts->eve = eve;
                    sts->onPct = ledseqeveList[eve].onPct;
                    sts->todo = ledseqeveList[eve].todo;
                    sts->period = ledseqeveList[eve].period;
                    sts->next = ledseqeveList[eve].next;

                    ledseqOnOff(led, false); 
                }
            }
            leds ^= led;//这个参数已设置完,清除对应位掩码
        }
        led <<= 1;  //下一个灯
        sts++;      //下一个灯
    }
}


/**
  * @brief  led服务程序，大循环调用
  * @param  ElapseTime :流逝时间 (以时基为准)
  * @note   
  * @retval  None
  */
void ledsequpdate (uint16_t ElapseTime)
{
    uint8_t pct;
    uint8_t led;
    uint8_t leds;
    ledseq_t *sts;

    led  = MLED_1;
    leds = MLED_ALL;
    sts = &ledseqControl[0];//前端初始化
    while (leds)//遍历所有灯
    {
        if (leds & led){
            if(sts->eve != LEDSEQ_EVE_NONE){
                if(ElapseTime >= sts->next){//超时
                    if(PreledsOnOffstatus & led){
                        pct = 100 - sts->onPct;         /* Percentage of cycle for off */
                        ledseqOnOff(led, false);        /* Turn it off */
                        
                        if (sts->todo != LED_SEQ_ALWAYS_TODO){  //不是无限闪烁，减少闪次数
                            sts->todo--;                        /* Not continuous, reduce count */
                        }
                    }else{
                        pct = sts->onPct;           /* Percentage of cycle for on */
                        ledseqOnOff (led, true);    /* Turn it on */
                    }
                    
                    if( !sts->todo){//次数到
                        sts->eve = LEDSEQ_EVE_NONE; //转到停止
                    }
                    sts->next = (uint16_t)(((uint32_t)pct * (uint32_t)sts->period) / 100);//取出下一次时间
                }else{//未超时，减去流逝时间
                    sts->next -= ElapseTime;
                }
            }
            leds ^= led;//已处理完，对应位清零
        }
        led <<= 1;  //下一个灯
        sts++;      //下一个灯
    }
}

/**
  * @brief  leds开关控制 抽象低层
  * @param  leds: bitmask ,detaial for MLED_XX ,bool
  * @note   
  * @note    
  * @note   
  * @retval 
  */
void ledseqOnOff (uint8_t leds, bool val)
{
#if (mLEDSNUM >= 1)
    if (leds & MLED_1){
        if(val){         
            PreledsOnOffstatus |= MLED_1; 
            MTURN_LED1_ON();
        }else{
            PreledsOnOffstatus &= ~MLED_1;
            MTURN_LED1_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 2)
    if (leds & MLED_2){
        if(val){
            PreledsOnOffstatus |= MLED_2; 
            MTURN_LED2_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_2;
            MTURN_LED2_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 3) 
    if (leds & MLED_3){
        if(val){         
            PreledsOnOffstatus |= MLED_3; 
            MTURN_LED3_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_3;
            MTURN_LED3_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 4) 
    if (leds & MLED_4){
        if(val){         
            PreledsOnOffstatus |= MLED_4; 
            MTURN_LED4_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_4;
            MTURN_LED4_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 5) 
    if (leds & MLED_5){
        if(val){         
            PreledsOnOffstatus |= MLED_5; 
            MTURN_LED5_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_5;
            MTURN_LED5_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 6) 
    if (leds & MLED_6){
        if(val){         
            PreledsOnOffstatus |= MLED_6; 
            MTURN_LED6_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_6;
            MTURN_LED6_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 7) 
    if (leds & MLED_7){
        if(val){         
            PreledsOnOffstatus |= MLED_7; 
            MTURN_LED7_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_7;
            MTURN_LED7_OFF();
        }
    }
#endif
#if (mLEDSNUM >= 8)
    if (leds & MLED_8){
        if(val){         
            PreledsOnOffstatus |= MLED_8; 
            MTURN_LED8_ON();
        }else{  
            PreledsOnOffstatus &= ~MLED_8;
            MTURN_LED8_OFF();
        }
   }
#endif
}


#ifdef DEBUG_MLEDS
#include "debug.h"
#include "mclock.h"
#include "hal_led.h"

void  FunDebuginit(void)
{
    dbg_set_dbg_level(7);
    halledInit();
    
//    ledseqeveset(MLED_1,LEDSEQ_EVE_BLINK_ONCE);  
    ledseqeveset(MLED_1,LEDSEQ_EVE_FLASH_BLINK);  
//    ledseqeveset(MLED_1,LEDSEQ_EVE_FLASH_QUARTER);
//    ledseqeveset(MLED_1,LEDSEQ_EVE_FLASH_HALF);
}


void FunDebugtask(void)
{
    uint32_t elapsetime;
    if(elapsetime = mcu_elapsedSystime(),elapsetime){
        ledsequpdate(elapsetime);
    }
}


#endif

