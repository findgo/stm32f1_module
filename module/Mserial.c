/**
  ******************************************************************************
  * @file   mserial.c
  * @author  
  * @version 
  * @date    
  * @brief   
  ******************************************************************************
  * @attention	 20151110     v1.1   	jgb		�ع�
  ******************************************************************************
  */
  
#include "Mserial.h"


typedef struct {
	uint8_t *ptxbuf;
#if SERIAL_TX_MAX_SIZE < 256
	uint8_t txHead;
	uint8_t txTail;
	uint8_t txcount;
	uint8_t txsize;
#else
	uint16_t txHead;	//���ͻ�����ͷ
	uint16_t txTail;	//���ͻ�����β
	uint16_t txcount;	//���ͻ�����Ч����
	uint16_t txsize;	//���ͻ�������С
#endif

	uint8_t *prxbuf;
#if SERIAL_RX_MAX_SIZE < 256
	uint8_t rxHead;
	uint8_t rxTail;
	uint8_t rxcount;
	uint8_t rxsize;
#else
	uint16_t rxHead;	//���ջ�����ͷ
	uint16_t rxTail;	//���ջ�����β
	uint16_t rxcount;	//���ջ�����Ч����
	uint16_t rxsize;	//���ջ�������С
#endif
}serialcfg_t;

//���ͻ����������ֽ���
#define SERIAL_TX_IDLE_AVAIL	(serialcfg.txsize - serialcfg.txcount)
//���ͻ�������Ч����
#define SERIAL_TX_VALID_AVAIL	(serialcfg.txcount)
//���ͻ������Ƿ��пɶ�����
#define IS_SERIAL_TX_VALID()	(serialcfg.txcount > 0)

//���ջ����������ֽ���
#define SERIAL_RX_IDLE_AVAIL	(serialcfg.rxsize - serialcfg.rxcount)
//���ջ�������Ч�ֽ���   
#define SERIAL_RX_VALID_AVAIL	(serialcfg.rxcount)
//���ջ������Ƿ��пɶ�����
#define IS_SERIAL_RX_VALID()	(serialcfg.rxcount > 0)

//private 
static uint8_t serialtxBuf[SERIAL_TX_MAX_SIZE];	//���ͻ�����
static uint8_t serialrxBuf[SERIAL_RX_MAX_SIZE];	//���ջ�����
static serialcfg_t serialcfg = {&serialtxBuf[0], 0, 0, 0, SERIAL_TX_MAX_SIZE, &serialrxBuf[0], 0, 0, 0, SERIAL_RX_MAX_SIZE};	// serial����ʵ��


static bool serialtxgoingflag = false;//���ڱ�����ǰ�Ƿ����ڷ�����


/**
  * @brief	���ͻ�������������
  * @param	buf: Ҫ���뻺���������� Len: ���뻺���������ݳ��� 
  * @note	
  * @retval number to buf
  */
static uint16_t SerialTxBufPut(uint8_t *buf, uint16_t len)
{
    uint16_t availcount;

    availcount = SERIAL_TX_IDLE_AVAIL;
	if (availcount < len) {   // �жϷ��ͻ����Ƿ����㹻�ռ�
        len = availcount;
    }

    availcount = len;
    
    while(availcount){
        serialcfg.ptxbuf[serialcfg.txTail] = *buf++;
        serialcfg.txcount++;
        if (++serialcfg.txTail >= serialcfg.txsize){
            serialcfg.txTail = 0;
        } 
        availcount--;
    }

	return len;
}
/**
  * @brief	���ͻ�������һ������
  * @param	dat: Ҫ���뻺����������
  * @note	
  * @retval true or false
  */
static uint16_t SerialTxBytePut(uint8_t dat)
{
	if (SERIAL_TX_IDLE_AVAIL > 0){  // �жϷ��ͻ����Ƿ����㹻�ռ�  
		serialcfg.ptxbuf[serialcfg.txTail] = dat;
		serialcfg.txcount++;
		if (++serialcfg.txTail >= serialcfg.txsize){
			serialcfg.txTail = 0;
		} 
		return 1;
	}

  return 0;
}

/**
  * @brief	���ͻ�����ȡ��һ���ֽ�����
  * @param	dat: Ҫȡ�����������ݵ�ַ
  * @note	
  * @retval	����ȡ�����ֽ���
  * @note		�жϷ���ȡ����ʱ��Ҫ����
  */
uint16_t SerialTxBytePop(uint8_t *dat)
{
	if(IS_SERIAL_TX_VALID()){//���ͻ������Ƿ�������
		*dat = serialcfg.ptxbuf[serialcfg.txHead];
		serialcfg.txcount--;
		if(++serialcfg.txHead >= serialcfg.txsize){
			serialcfg.txHead = 0;
		}
		return 1; 
	}

	return 0;
}
/**
  * @brief	�����ͻ�������Ч�ֽ���
  * @param	
  * @note	
  * @retval	���ط��ͻ�������Ч�ֽ���
  * @note		
  */
uint16_t serialTxValidAvail(void)
{
	return SERIAL_TX_VALID_AVAIL;
}



/**
  * @brief	����ջ���������һ������
  * @param	dat:���������
  * @note	
  * @retval  	ture or false
  * @note		�жϽ��մ�����ʱ�жϵ���
  */
bool SerialRxBytePut(uint8_t dat)
{
	if(SERIAL_RX_IDLE_AVAIL > 0){
		serialcfg.prxbuf[serialcfg.rxTail] = dat;
		serialcfg.rxcount++;
		if(++serialcfg.rxTail >= serialcfg.rxsize){
			serialcfg.rxTail = 0;
		} 
		return true;
	}

	return false;
}

/**
  * @brief	�ӽ��ջ�����ȡ��len������
  * @param	buf:ȡ�����ݵĻ�����,len: ���ݳ���
  * @note	
  * @retval  ����ȡ�����ݵ�ʵ�ʸ���
  */
static uint16_t SerialRxBufPop(uint8_t *buf, uint16_t len)
{
	uint16_t cnt = 0;

    while(IS_SERIAL_RX_VALID() && (cnt < len))//���ջ�����������
    {
		*buf++ = serialcfg.prxbuf[serialcfg.rxHead];//�ӽ��ջ�������ȡ����
		serialcfg.rxcount--;
		if(++serialcfg.rxHead >= serialcfg.rxsize){
			serialcfg.rxHead = 0;
		}
		cnt++;
    }

	return cnt;
}
/**
  * @brief	�ӽ��ջ�����ȡ��len������
  * @param	buf:ȡ�����ݵĻ�����,len: ���ݳ���
  * @note	
  * @retval  ����ȡ�����ݵ�ʵ�ʸ���
  */
uint16_t serialRxValidAvail(void)
{
	return SERIAL_RX_VALID_AVAIL;
}

/**
  * @brief	TX�������Ϊlen������
  * @param	buf:������ݻ�����,len:������ݳ���
  * @note	
  * @retval  number write
  */
uint16_t Serial_WriteBuf(uint8_t *buf,uint16_t len)
{
//	uint8_t temp;
	uint16_t count;
	halIntState_t bintstate;
	
	ENTER_SAFE_ATOM_CODE(bintstate);
	count = SerialTxBufPut(buf, len);
	EXIT_SAFE_ATOM_CODE(bintstate);
	if(!serialtxgoingflag){//ֻ�е�ǰ���ڷ��ͽ���ʱ��������
	
		//���÷�������ж� has some bug
//		if(SerialTxBytePop(&temp))
//		{
//			SerialPutByte(temp);
//		}
		
		//���÷�����ɿ��ж�
		if(IS_SERIAL_TX_VALID())//������
		{
			//ʹ�ܷ����ж�
			SerialTxIEEnable();
		}
	}
	
	return count;
}
uint16_t Serial_WriteByte(uint8_t dat)
{
//	uint8_t temp;
	uint16_t count;
	halIntState_t bintstate;
	
	ENTER_SAFE_ATOM_CODE(bintstate);
	count = SerialTxBytePut(dat);
	//���÷�������ж� has some bug
//		if(SerialTxBytePop(&temp)){
//			SerialPutByte(temp);
//			SerialTxIEEnable();
//		}
	
	//���÷�����ɿ��ж�
	if(IS_SERIAL_TX_VALID()){//������
		//ʹ�ܷ����ж�
		SerialTxIEEnable();
	}
	EXIT_SAFE_ATOM_CODE(bintstate);

	return count;
}

/**
  * @brief	�ӽ���ȡ��len������
  * @param	buf:�������ݻ�����,len:�������ݳ���
  * @note	
  * @retval  ����ȡ�����ݵ�ʵ�ʸ���
  */
uint16_t Serial_Read(uint8_t *buf, uint16_t len)
{
	uint16_t length;
	halIntState_t bintstate;

	ENTER_SAFE_ATOM_CODE(bintstate);
	length = SerialRxBufPop(buf, len);
	EXIT_SAFE_ATOM_CODE(bintstate);
	
	return length;
}

/***********only for Isr callback***********/
/**
  * @brief	���Ϳ��жϵ��ú���
  * @param	None
  * @note	
  * @note	 
  * @note	
  * @retval  None
  */
void Serial_TXE_Isr_callback(void)
{
	uint8_t temp;
	halIntState_t bintstate;

	ENTER_SAFE_ATOM_CODE(bintstate);
	if(SerialTxBytePop(&temp)){
		SerialPutByte(temp);
	}else{
		SerialTxIEDisable();
	}
    EXIT_SAFE_ATOM_CODE(bintstate);
}
/**
  * @brief	��������жϻص�����
  * @param	None
  * @note	
  * @retval  None
  */
void Serial_TXC_Isr_callback(void)
{


}
/**
  * @brief	�����жϻص�����
  * @param	None
  * @note	
  * @retval  None
  */
void Serial_RX_Isr_callback(void)
{
	uint8_t temp = SerialGetByte();
	halIntState_t bintstate;

	ENTER_SAFE_ATOM_CODE(bintstate);	
	SerialRxBytePut(temp);
    EXIT_SAFE_ATOM_CODE(bintstate);
}

