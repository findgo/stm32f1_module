
#include "mleds.h"


typedef struct ledseq_t{
    uint8_t     eve;
    uint8_t     todo;       /* գ�ִ������������Ϊ0xff������������˸*/
    uint8_t     onPct;      /* ռ�ձ�*/
    uint16_t    period; /*��/�������� */    
    uint16_t    next;    /* ��һ�θı��ʱ��/������ʱ����� ����ʱ����*/
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
  * @brief  ��ʼ��
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
    ledseqOnOff( MLED_ALL, false);// ���еƹ�
}

/**
  * @brief  
  * @param  leds �ĸ��ƣ�λ�򷨣���ͬʱ���ü�����   
  * @note       eve Ϊ LEDSEQ_EVE_NONE����״̬��mode ����(������ LEDSEQ_MODE_NONE )
  * @note       eve ��Ϊ LEDSEQ_EVE_NONE�����Բ���mode��һ������Ϊ LEDSEQ_MODE_NONE
  * @retval  None
  */

void ledseqset(uint8_t leds, ledseq_eve_t eve, ledseq_mode_t mode)
{
    uint8_t led;
    ledseq_t *sts;

    led = MLED_1;           //�ӵ�һ���ƿ�ʼ��
    leds &= MLED_ALL;       //���β��������ڵĵ�
    sts = &ledseqControl[0];
    while (leds)    //�����ҳ���Ҫ���õĵ�
    {
        if (leds & led){
            if (eve == LEDSEQ_EVE_NONE){
                if(mode != LEDSEQ_MODE_TOGGLE){
                    if(PreledsOnOffstatus & led){
                        if(mode == LEDSEQ_MODE_OFF){
                            ledseqOnOff (led, false); //����
                        }
                    }else{
                        if(mode == LEDSEQ_MODE_ON){
                            ledseqOnOff (led, true); //����
                        }
                    }
                 }else{
                    ledseqOnOff(led, (PreledsOnOffstatus ^ led) & led);// ȡ�� do it
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
            leds ^= led;//���������������,�����Ӧλ����
        }
        led <<= 1;  //��һ����
        sts++;      //��һ����
    }
}


/**
  * @brief  led������򣬴�ѭ������
  * @param  ElapseTime :����ʱ�� (��ʱ��Ϊ׼)
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
    sts = &ledseqControl[0];//ǰ�˳�ʼ��
    while (leds)//�������е�
    {
        if (leds & led){
            if(sts->eve != LEDSEQ_EVE_NONE){
                if(ElapseTime >= sts->next){//��ʱ
                    if(PreledsOnOffstatus & led){
                        pct = 100 - sts->onPct;         /* Percentage of cycle for off */
                        ledseqOnOff(led, false);        /* Turn it off */
                        
                        if (sts->todo != LED_SEQ_ALWAYS_TODO){  //����������˸������������
                            sts->todo--;                        /* Not continuous, reduce count */
                        }
                    }else{
                        pct = sts->onPct;           /* Percentage of cycle for on */
                        ledseqOnOff (led, true);    /* Turn it on */
                    }
                    
                    if( !sts->todo){//������
                        sts->eve = LEDSEQ_EVE_NONE; //ת��ֹͣ
                    }
                    sts->next = (uint16_t)(((uint32_t)pct * (uint32_t)sts->period) / 100);//ȡ����һ��ʱ��
                }else{//δ��ʱ����ȥ����ʱ��
                    sts->next -= ElapseTime;
                }
            }
            leds ^= led;//�Ѵ����꣬��Ӧλ����
        }
        led <<= 1;  //��һ����
        sts++;      //��һ����
    }
}

/**
  * @brief  leds���ؿ��� ����Ͳ�
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

