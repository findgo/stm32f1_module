
#include "mcoils.h"

/* 用于记录coil当前的工作状态 */
static uint8_t PreCoilOnOffStatus = 0x00;

#define PreCoilStatus(Coil) (PreCoilOnOffStatus & (Coil))

/***********************************************************
* 函数名称:     mCoilsInit(void)
* 函数功能:     外部调用，初始化
* 入口参数:     None
* 出口参数:     无      
* 返回参数:     无
* 功能描述: Coils 采用位掩码
* 备 注: 
************************************************************/
void mCoilsInit(void)
{
    mCoilsOnOff(MCOILS_ALL, MCOILS_MODE_OFF);
}

/***********************************************************
* 函数名称:     mCoilsSet
* 函数功能:     外部调用，设置coils模式,位掩码开发
* 入口参数:     Coils--哪个coil,  mode  模式
* 出口参数:     无      
* 返回参数:     无
* 功能描述: Coils 采用位掩码
* 备 注: 
************************************************************/
void mCoilsSet (uint8_t Coils, mCoils_Mode mode)
{
    uint8_t Coil;
    mCoils_Mode uSMode;
    
    Coil  = MCOILS_1;       //从第一个灯开始查
    Coils &= MCOILS_ALL;        //屏蔽不在驱动内的coils
    
    while (Coils)  //遍历找出想要设置的coils
    {
      if (Coils & Coil){
        if (mode != MCOILS_MODE_TOGGLE){//非切换模式        
            if(PreCoilStatus(Coil)){//查看当前coil状态
              if(mode == MCOILS_MODE_OFF){ //on,then check it want to set off?
                mCoilsOnOff(Coil, MCOILS_MODE_OFF); //动作
              }
            }else{               
              if(mode == MCOILS_MODE_ON){   //off, then check it want to set on?
                mCoilsOnOff(Coil, MCOILS_MODE_ON); 
              }
            }
        } else {           /* Toggle mode*/
            if(PreCoilStatus(Coil)){  //查看当前coil状态
                uSMode = MCOILS_MODE_OFF;   //on, Toggle to off 
            }else{
                uSMode = MCOILS_MODE_ON;    //off, Toggle to on
            }
            mCoilsOnOff(Coil, uSMode);    //动作
        }
        Coils ^= Coil;//这个参数已设置完,清除对应位掩码
      }
      Coil <<= 1;  //下一个coils
    }
}
/***********************************************************
* 函数名称:     mCoilsGetStasus(void)
* 函数功能:     外部调用，获得这个coils模式,位掩码开发
* 入口参数:     which--哪个which, 
* 出口参数:     无      
* 返回参数:     无
* 功能描述: 获得单个coil的状态
* 备 注: 
************************************************************/
bool mCoilsGetStasus(uint8_t which)
{
    return (bool)PreCoilStatus(which);
}
/***********************************************************
* 函数名称:     mCoilsGetMulStasus(void)
* 函数功能:     外部调用，获得这个coils模式,位掩码开发
* 入口参数:     coils--哪个coils, 
* 出口参数:     无      
* 返回参数:     无
* 功能描述: 获得多个coils，需要自己做位移
* 备 注: 
************************************************************/
uint8_t mCoilsGetMulStasus(uint8_t Coils)
{
    return PreCoilStatus(Coils);
}

/**
  * @brief      Coils开关控制
  * @param  Coils --哪个coils, mode---模式
  * @note   
  * @note    
  * @note   
  * @retval None
  */
void mCoilsOnOff(uint8_t Coils, mCoils_Mode mode)
{
#if (mCOILSNUM >= 1)
    if (Coils & MCOILS_1){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS1_ON();      
        PreCoilOnOffStatus |= MCOILS_1;//save current mode
      }else{
        MTURN_COILS1_OFF();     
        PreCoilOnOffStatus &= ~MCOILS_1;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 2)
    if (Coils & MCOILS_2){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS2_ON();
        PreCoilOnOffStatus |= MCOILS_2;//save current mode
      }else{
        MTURN_COILS2_OFF(); 
        PreCoilOnOffStatus &= ~MCOILS_2;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 3)
    if (Coils & MCOILS_3){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS3_ON();  
        PreCoilOnOffStatus |= MCOILS_3;//save current mode
      }else{
        MTURN_COILS3_OFF();
        PreCoilOnOffStatus &= ~MCOILS_3;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 4)
    if (Coils & MCOILS_4){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS4_ON();
        PreCoilOnOffStatus |= MCOILS_4;//save current mode
      }else{
        MTURN_COILS4_OFF();
        PreCoilOnOffStatus &= ~MCOILS_4;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 5)
    if (Coils & MCOILS_5){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS5_ON();
        PreCoilOnOffStatus |= MCOILS_5;//save current mode
      }else{
        MTURN_COILS5_OFF();
        PreCoilOnOffStatus &= ~MCOILS_5;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 6)
    if (Coils & MCOILS_6){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS6_ON();  
        PreCoilOnOffStatus |= MCOILS_6;//save current mode
      }else{
        MTURN_COILS6_OFF();
        PreCoilOnOffStatus &= ~MCOILS_6;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 7)
    if (Coils & MCOILS_7){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS7_ON();
        PreCoilOnOffStatus |= MCOILS_7;//save current mode
      }else{
        MTURN_COILS7_OFF(); 
        PreCoilOnOffStatus &= ~MCOILS_7;//save current mode
      }
    }
#endif
#if (mCOILSNUM >= 8)    
    if (Coils & MCOILS_8){
      if (mode == MCOILS_MODE_ON){
        MTURN_COILS8_ON();  
        PreCoilOnOffStatus |= MCOILS_8;//save current mode
      }else{
        MTURN_COILS8_OFF(); 
        PreCoilOnOffStatus &= ~MCOILS_8;//save current mode
      }
    }
#endif
}


#ifdef DEBUG_MCOILS
#include "debug.h"

void  funcheckinit(void)
{
    dbg_set_dbg_level(7);
    mCoilsInit();

    mCoilsSet(MCOILS_ALL,MCOILS_MODE_ON);
    DBG_VERBOS("coils do on\r\n");
    DBG_VERBOS("coils:0x%2x\r\n",mCoilsGetMulStasus(MCOILS_ALL));   
    mCoilsSet(MCOILS_ALL,MCOILS_MODE_TOGGLE);
    DBG_VERBOS("coils do toggle\r\n");
    DBG_VERBOS("coils:0x%2x\r\n",mCoilsGetMulStasus(MCOILS_ALL));
    mCoilsSet(MCOILS_ALL,MCOILS_MODE_ON);   
    DBG_VERBOS("coils do on\r\n");
    DBG_VERBOS("coils:0x%2x\r\n",mCoilsGetMulStasus(MCOILS_ALL));
    mCoilsSet(MCOILS_ALL,MCOILS_MODE_OFF);
    DBG_VERBOS("coils do off\r\n");
    DBG_VERBOS("coils:0x%2x\r\n",mCoilsGetMulStasus(MCOILS_ALL));
    mCoilsSet(MCOILS_1,MCOILS_MODE_ON); 
    DBG_VERBOS("coils:%d\r\n",mCoilsGetStasus(MCOILS_1));
}

#endif

