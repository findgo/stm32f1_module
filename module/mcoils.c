
#include "mcoils.h"

/* ���ڼ�¼coil��ǰ�Ĺ���״̬ */
static uint8_t PreCoilOnOffStatus = 0x00;

#define PreCoilStatus(Coil) (PreCoilOnOffStatus & (Coil))

/***********************************************************
* ��������:     mCoilsInit(void)
* ��������:     �ⲿ���ã���ʼ��
* ��ڲ���:     None
* ���ڲ���:     ��      
* ���ز���:     ��
* ��������: Coils ����λ����
* �� ע: 
************************************************************/
void mCoilsInit(void)
{
    mCoilsOnOff(MCOILS_ALL, MCOILS_MODE_OFF);
}

/***********************************************************
* ��������:     mCoilsSet
* ��������:     �ⲿ���ã�����coilsģʽ,λ���뿪��
* ��ڲ���:     Coils--�ĸ�coil,  mode  ģʽ
* ���ڲ���:     ��      
* ���ز���:     ��
* ��������: Coils ����λ����
* �� ע: 
************************************************************/
void mCoilsSet (uint8_t Coils, mCoils_Mode mode)
{
    uint8_t Coil;
    mCoils_Mode uSMode;
    
    Coil  = MCOILS_1;       //�ӵ�һ���ƿ�ʼ��
    Coils &= MCOILS_ALL;        //���β��������ڵ�coils
    
    while (Coils)  //�����ҳ���Ҫ���õ�coils
    {
      if (Coils & Coil){
        if (mode != MCOILS_MODE_TOGGLE){//���л�ģʽ        
            if(PreCoilStatus(Coil)){//�鿴��ǰcoil״̬
              if(mode == MCOILS_MODE_OFF){ //on,then check it want to set off?
                mCoilsOnOff(Coil, MCOILS_MODE_OFF); //����
              }
            }else{               
              if(mode == MCOILS_MODE_ON){   //off, then check it want to set on?
                mCoilsOnOff(Coil, MCOILS_MODE_ON); 
              }
            }
        } else {           /* Toggle mode*/
            if(PreCoilStatus(Coil)){  //�鿴��ǰcoil״̬
                uSMode = MCOILS_MODE_OFF;   //on, Toggle to off 
            }else{
                uSMode = MCOILS_MODE_ON;    //off, Toggle to on
            }
            mCoilsOnOff(Coil, uSMode);    //����
        }
        Coils ^= Coil;//���������������,�����Ӧλ����
      }
      Coil <<= 1;  //��һ��coils
    }
}
/***********************************************************
* ��������:     mCoilsGetStasus(void)
* ��������:     �ⲿ���ã�������coilsģʽ,λ���뿪��
* ��ڲ���:     which--�ĸ�which, 
* ���ڲ���:     ��      
* ���ز���:     ��
* ��������: ��õ���coil��״̬
* �� ע: 
************************************************************/
bool mCoilsGetStasus(uint8_t which)
{
    return (bool)PreCoilStatus(which);
}
/***********************************************************
* ��������:     mCoilsGetMulStasus(void)
* ��������:     �ⲿ���ã�������coilsģʽ,λ���뿪��
* ��ڲ���:     coils--�ĸ�coils, 
* ���ڲ���:     ��      
* ���ز���:     ��
* ��������: ��ö��coils����Ҫ�Լ���λ��
* �� ע: 
************************************************************/
uint8_t mCoilsGetMulStasus(uint8_t Coils)
{
    return PreCoilStatus(Coils);
}

/**
  * @brief      Coils���ؿ���
  * @param  Coils --�ĸ�coils, mode---ģʽ
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

