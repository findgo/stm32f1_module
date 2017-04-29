#include "mkey.h"

typedef struct{                                                                     
    uint8_t     tHead;
    uint8_t     tTail;
    uint8_t     tCounter;
    uint8_t     tSize;
    uint8_t    *ptBuffer;
}key_fifo_t;

//定义缓冲区
static uint8_t s_tKeyDetectorBuffer[KEY_QUEUE_MAX_SIZE];
//定义一个对象实列
static key_fifo_t s_tKeyDetectorQueue = {0, 0, 0, KEY_QUEUE_MAX_SIZE, s_tKeyDetectorBuffer};

static key_cfg_t *KeyHead = NULL;
static key_cfg_t *KeyTail = NULL;

static bool key_put(uint8_t Obj)  
{
    key_fifo_t *ptQ = (key_fifo_t *)&s_tKeyDetectorQueue;

    if (ptQ->tCounter != ptQ->tSize) {
        ptQ->ptBuffer[ptQ->tTail] = Obj;
        if (++ptQ->tTail >= ptQ->tSize){
            ptQ->tTail = 0;
        }       
        ptQ->tCounter++;
        return true;
    }
    
    return false;
}
static bool key_pop(uint8_t *ptObj)
{ 
    key_fifo_t *ptQ = (key_fifo_t *)&s_tKeyDetectorQueue;

    if(ptQ->tCounter != 0) {
        *ptObj = ptQ->ptBuffer[ptQ->tHead];
        if(++ptQ->tHead >= ptQ->tSize){ 
            ptQ->tHead = 0; 
        }       
        ptQ->tCounter--;
        return true;
    } 
    
    return false;
}
#define KEY_DECETOR_CHECK_START         0
#define KEY_DECETOR_CHECK_DOWN          1
#define KEY_DECETOR_LONG_PRESS          2   
#define KEY_DECETOR_REPEAT_PRESS        3 
#define KEY_DECETOR_IS_UP               4

void mkey_create(key_cfg_t *cfg,KeyDownFunc IsKeyDownFunc,
                    uint8_t KeyCodeDown,uint8_t KeyCodeLong,uint8_t KeyCodeUp,
                    uint8_t filterTime,uint16_t longTime,uint8_t repeatSpeed)
{
    cfg->IsKeyDownFunc = IsKeyDownFunc;
    if(!filterTime)
        filterTime = KEY_FILTER_TIME;
    cfg->filterTime = filterTime;
    cfg->longTime = longTime;
    cfg->repeatSpeed = repeatSpeed;
    cfg->KeyCodeDown = KeyCodeDown;
    cfg->KeyCodeLong = KeyCodeLong;
    cfg->KeyCodeUp = KeyCodeUp;
    cfg->filterCount = 0;
    cfg->longrepCount = 0;
    cfg->next = NULL;
    cfg->state = KEY_DECETOR_CHECK_START;
    if(KeyHead){
       KeyTail->next = (void *)cfg;
       KeyTail = cfg;
    }else{
        KeyHead = cfg;
        KeyTail= cfg;
    }
}

/**
  * @brief   按键分析程序，短击，长击，连击,抬键
  * @param  none
  * @note   
  * @retval  none
  * @note 
  */
void mkey_decetor_task(void)
{
    key_cfg_t *curcfg = KeyHead;    
    
    while(curcfg)
    {
        switch(curcfg->state){
            case KEY_DECETOR_CHECK_START:
                if(curcfg->IsKeyDownFunc()){
                    curcfg->state = KEY_DECETOR_CHECK_DOWN;
                }
                break;
                
            case KEY_DECETOR_CHECK_DOWN:    
                if(++curcfg->filterCount >= curcfg->filterTime){ // 滤波
                    curcfg->filterCount = 0;
                    if(curcfg->IsKeyDownFunc()){
                        if((curcfg ->longTime == 0) && (curcfg->repeatSpeed == 0)){ // 不支持长击和连击，直接到抬键状态
                            key_put(curcfg->KeyCodeDown);
                            curcfg->state = KEY_DECETOR_IS_UP;
                        }else{
                            curcfg->state = KEY_DECETOR_LONG_PRESS;
                        }
                    }else{
                        curcfg->state = KEY_DECETOR_CHECK_START;
                    }
                }
                break;
                
            case KEY_DECETOR_LONG_PRESS:
                if(curcfg->longTime > 0){ // 支持长按
                    if(curcfg->IsKeyDownFunc()){
                        if(++curcfg->longrepCount >= curcfg->longTime){// 长按
                            key_put(curcfg->KeyCodeLong);
                            if(curcfg->repeatSpeed == 0)// 不支持连击，直接到抬键
                                curcfg->state = KEY_DECETOR_IS_UP;
                            else
                                curcfg->state = KEY_DECETOR_REPEAT_PRESS;
                            curcfg->longrepCount = 0;  
                        }
                    }else{  //短按
                        key_put(curcfg->KeyCodeDown);
                        curcfg->state = KEY_DECETOR_IS_UP;
                        curcfg->longrepCount = 0;
                    }
                }else{  // 不支持长按
                    if(curcfg->repeatSpeed > 0){ //支持连击
                       if(curcfg->IsKeyDownFunc()){
                            if(++curcfg->longrepCount >= KEY_LONGTOREAP_TIME){
                                curcfg->state = KEY_DECETOR_REPEAT_PRESS;                        
                                curcfg->longrepCount = 0;
                            }
                       }else{
                            key_put(curcfg->KeyCodeDown);
                            curcfg->longrepCount = 0;
                            curcfg->state = KEY_DECETOR_IS_UP; 
                       }
                    }else{ // 不支持连击
                       key_put(curcfg->KeyCodeDown);
                       curcfg->state = KEY_DECETOR_IS_UP; 
                    }
                }
                break;
            case KEY_DECETOR_REPEAT_PRESS:
                if(curcfg->repeatSpeed > 0){ // 支持连击
                    if(curcfg->IsKeyDownFunc()){
                        if(++curcfg->longrepCount >= curcfg->repeatSpeed){
                            key_put(curcfg->KeyCodeDown);
                            curcfg->longrepCount = 0;
                        }
                    }else{
                         curcfg->longrepCount = 0;
                         curcfg->state = KEY_DECETOR_IS_UP;
                    }
                }else{ // 不支持连击            
                    curcfg->longrepCount = 0;
                    curcfg->state = KEY_DECETOR_IS_UP;
                }
                break;
                
            case KEY_DECETOR_IS_UP:         //抬键处理
                if(++curcfg->filterCount >= curcfg->filterTime){
                    if(!curcfg->IsKeyDownFunc()){
                        if(curcfg->KeyCodeUp != KEY_NULL)
                            key_put(curcfg->KeyCodeUp);
                        curcfg->state = KEY_DECETOR_CHECK_START;
                    }
                    curcfg->filterCount = 0;
                }
                break;
                
                default:
                    break;
        }
        curcfg = (key_cfg_t *)curcfg->next;
    }
}
/**
  * @brief  返回键值
  * @param  
  * @note   
  * @retval  none
  * @note 
  */
uint8_t mkey_getvalue(void)
{
    uint8_t keyvalue = KEY_NULL;
    
    (void)key_pop(&keyvalue);

    return keyvalue;
}


#ifdef DEBUG_MKEY

#include "debug.h"
#include "mtimers.h"

static mtimer_t testkey_time = MTIMER_INIT();
static key_cfg_t testKey;
static key_cfg_t testKey0;

static uint8_t TestKeyDown(void);
void  funcheckinit(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

	GPIO_Init(GPIOA, &GPIO_InitStruct);
    
    dbg_set_dbg_level(7);
    // 测试全功能
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_PRESS1_UP,2,100,20);
    // 测试短按  抬键  
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_PRESS1_UP,2,0,0);
    // 测试短按  长按    
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_NULL,2,100,0);
    // 测试短按  连击     
    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_NULL, KEY_NULL,2,0,20);
    mtimer_start(&testkey_time,tm_ms(10));
}


void funchecktask(void)
{
    uint8_t keyval;
    
    if(mtimer_expired(&testkey_time)){
        mkey_decetor_task();
        mtimer_start(&testkey_time,tm_ms(10));
    }
    keyval = mkey_getvalue();
    switch(keyval){
        case KEY_PRESS1_DOWN:
            DBG_VERBOS("KEY DOWN\r\n");
            break;
        case KEY_PRESS1_LONG:
            DBG_VERBOS("KEY LONG PRESS\r\n");
            break;
        case KEY_PRESS1_UP:
            DBG_VERBOS("KEY UP\r\n");
            break;

    }
}

static uint8_t TestKeyDown(void)
{
    return !GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
}

#endif
