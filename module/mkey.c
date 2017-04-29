#include "mkey.h"

typedef struct{                                                                     
    uint8_t     tHead;
    uint8_t     tTail;
    uint8_t     tCounter;
    uint8_t     tSize;
    uint8_t    *ptBuffer;
}key_fifo_t;

//���建����
static uint8_t s_tKeyDetectorBuffer[KEY_QUEUE_MAX_SIZE];
//����һ������ʵ��
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
  * @brief   �����������򣬶̻�������������,̧��
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
                if(++curcfg->filterCount >= curcfg->filterTime){ // �˲�
                    curcfg->filterCount = 0;
                    if(curcfg->IsKeyDownFunc()){
                        if((curcfg ->longTime == 0) && (curcfg->repeatSpeed == 0)){ // ��֧�ֳ�����������ֱ�ӵ�̧��״̬
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
                if(curcfg->longTime > 0){ // ֧�ֳ���
                    if(curcfg->IsKeyDownFunc()){
                        if(++curcfg->longrepCount >= curcfg->longTime){// ����
                            key_put(curcfg->KeyCodeLong);
                            if(curcfg->repeatSpeed == 0)// ��֧��������ֱ�ӵ�̧��
                                curcfg->state = KEY_DECETOR_IS_UP;
                            else
                                curcfg->state = KEY_DECETOR_REPEAT_PRESS;
                            curcfg->longrepCount = 0;  
                        }
                    }else{  //�̰�
                        key_put(curcfg->KeyCodeDown);
                        curcfg->state = KEY_DECETOR_IS_UP;
                        curcfg->longrepCount = 0;
                    }
                }else{  // ��֧�ֳ���
                    if(curcfg->repeatSpeed > 0){ //֧������
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
                    }else{ // ��֧������
                       key_put(curcfg->KeyCodeDown);
                       curcfg->state = KEY_DECETOR_IS_UP; 
                    }
                }
                break;
            case KEY_DECETOR_REPEAT_PRESS:
                if(curcfg->repeatSpeed > 0){ // ֧������
                    if(curcfg->IsKeyDownFunc()){
                        if(++curcfg->longrepCount >= curcfg->repeatSpeed){
                            key_put(curcfg->KeyCodeDown);
                            curcfg->longrepCount = 0;
                        }
                    }else{
                         curcfg->longrepCount = 0;
                         curcfg->state = KEY_DECETOR_IS_UP;
                    }
                }else{ // ��֧������            
                    curcfg->longrepCount = 0;
                    curcfg->state = KEY_DECETOR_IS_UP;
                }
                break;
                
            case KEY_DECETOR_IS_UP:         //̧������
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
  * @brief  ���ؼ�ֵ
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
    // ����ȫ����
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_PRESS1_UP,2,100,20);
    // ���Զ̰�  ̧��  
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_PRESS1_UP,2,0,0);
    // ���Զ̰�  ����    
//    mkey_create(&testKey,TestKeyDown,KEY_PRESS1_DOWN, KEY_PRESS1_LONG, KEY_NULL,2,100,0);
    // ���Զ̰�  ����     
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
