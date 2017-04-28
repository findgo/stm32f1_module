


#include "util_fifo.h"

/*	���г�ʼ��		*/  		
bool fifo_init(fifo_buf_t *ptQ, uint8_t *ptBuffer, uint16_t tSize)
{                                                        
    if (NULL == ptQ || NULL == ptBuffer || 0 == tSize) { 
        return false;                                    
    }                                                    
                                                         
    ptQ->ptBuffer 	= ptBuffer;                          
    ptQ->tSize 		= tSize;                             
    ptQ->tHead 		= 0;                                 
    ptQ->tTail 		= 0;                                 
    ptQ->tCounter 	= 0;                                 
                                                         
    return true;                                         
}

// ��ջ��� 
bool fifo_flush(fifo_buf_t *ptQ)
{
	if (NULL == ptQ ){
		return false;				
	}
	
	ptQ->tTail = ptQ->tHead;
	ptQ->tCounter = 0;
	
	return true;
}

 /*	���		*/                   					     
uint16_t fifo_put(fifo_buf_t *ptQ, uint8_t tObj)
{														
	if (ptQ == NULL){
		return 0;				
	}												
																					
	if (ptQ->tCounter != ptQ->tSize) {		
		ptQ->ptBuffer[ptQ->tTail] = tObj;
		ptQ->tCounter++;
		if (++ptQ->tTail >= ptQ->tSize) { 		
			ptQ->tTail = 0; 					
		}										
		return 1; 						
	}
	
	return 0; 								
}	
  /*	��ͷ���	 ʵ��ջ	*/  
uint16_t fifo_put_front(fifo_buf_t *ptQ, uint8_t tObj)
{
	if ( ptQ == NULL){
		return 0;				
	}												
																					
	if (ptQ->tCounter != ptQ->tSize) {
		if(ptQ->tHead == 0){
			ptQ->tHead = ptQ->tSize - 1;
		}else{
			ptQ->tHead--;
		}
		ptQ->ptBuffer[ptQ->tHead] = tObj;
		ptQ->tCounter++;
		return 1; 						
	}
	
	return 0; 	
}
  /*	����		*/                                       
uint16_t fifo_pop(fifo_buf_t *ptQ, uint8_t *ptObj)
{                                                          
	if(ptQ == NULL && NULL == ptObj){						
		return 0;										
    }
	
	if (ptQ->tCounter != 0) {						
		*ptObj = ptQ->ptBuffer[ptQ->tHead];
		ptQ->tCounter--;	
		if(++ptQ->tHead >= ptQ->tSize){				
			ptQ->tHead = 0; 						
		}										
		return 1; 								
	}												
		                                                       
    return 0;                                        
}
  /* �ҾͿ���,���ǲ�����*/
  bool fifo_peek(fifo_buf_t *ptQ, uint8_t *ptObj)
  {      
      if (NULL == ptQ && NULL == ptObj) { 
          return false;
      }
                                                                  
      if (ptQ->tCounter != 0) {           
          *ptObj = ptQ->ptBuffer[ptQ->tHead];     
          return true;                 
      }
      
      return false; 
  }

/*  ���     ����Ϊlength������buf  */  
uint16_t fifo_bufput(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length)
{
  uint16_t availcount;

  if (NULL == ptQ || buf == NULL ){
      return 0;               
  }   
  //�鿴�ж��ٿ�λ
  availcount = ptQ->tSize - ptQ->tCounter;
  if(availcount < length){
      length = availcount;
  }
  
  availcount = length;
  while(availcount)
  {
      ptQ->ptBuffer[ptQ->tTail] = *buf;
      ptQ->tCounter++;
      if (++ptQ->tTail >= ptQ->tSize) {       
          ptQ->tTail = 0;                     
      }
      buf++;
      availcount--;
  }
  
  return length;
}
  /*	����	 ����Ϊlength�����ݵ�buf	*/ 
uint16_t fifo_bufpop(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length)
{
	uint16_t lens;
	uint16_t validcount;
	
	if (NULL == ptQ || buf == NULL ){
		return 0;				
	}	
	//�鿴�ж�����Ч��ֵ
	validcount = ptQ->tCounter;
	if(validcount >= length){
		lens = length;
	}else{
		lens = validcount;
	}
    
	validcount = lens;
	while(validcount)
	{
		*buf = ptQ->ptBuffer[ptQ->tHead];
		ptQ->tCounter--;	
		if(++ptQ->tHead >= ptQ->tSize){				
			ptQ->tHead = 0; 						
		}
		buf++;
		validcount--;
	}
	
	return lens;
}
/* ��Ч�ֽ��� */
uint16_t fifo_count(fifo_buf_t *ptQ)
{
	if (NULL == ptQ)
        return 0;
    
    return ptQ->tCounter;
}
/* �����Ƿ�Ϊ�� */
bool fifo_empty(fifo_buf_t *ptQ)
{
	return (ptQ ? (ptQ->tCounter == 0) : true);
}
/* �����Ƿ�Ϊ�� */
bool fifo_full(fifo_buf_t *ptQ)
{
	return (ptQ ? (ptQ->tCounter == ptQ->tSize) : true);
}


