


#include "util_fifo.h"

/*	队列初始化		*/  		
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

// 清空缓冲 
bool fifo_flush(fifo_buf_t *ptQ)
{
	if (NULL == ptQ ){
		return false;				
	}
	
	ptQ->tTail = ptQ->tHead;
	ptQ->tCounter = 0;
	
	return true;
}

 /*	入队		*/                   					     
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
  /*	从头入队	 实现栈	*/  
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
  /*	出队		*/                                       
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
  /* 我就看看,就是不出队*/
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

/*  入队     长度为length的数据buf  */  
uint16_t fifo_bufput(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length)
{
  uint16_t availcount;

  if (NULL == ptQ || buf == NULL ){
      return 0;               
  }   
  //查看有多少空位
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
  /*	出队	 长度为length的数据到buf	*/ 
uint16_t fifo_bufpop(fifo_buf_t *ptQ, uint8_t *buf, uint16_t length)
{
	uint16_t lens;
	uint16_t validcount;
	
	if (NULL == ptQ || buf == NULL ){
		return 0;				
	}	
	//查看有多少有效数值
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
/* 有效字节数 */
uint16_t fifo_count(fifo_buf_t *ptQ)
{
	if (NULL == ptQ)
        return 0;
    
    return ptQ->tCounter;
}
/* 队列是否为空 */
bool fifo_empty(fifo_buf_t *ptQ)
{
	return (ptQ ? (ptQ->tCounter == 0) : true);
}
/* 队列是否为满 */
bool fifo_full(fifo_buf_t *ptQ)
{
	return (ptQ ? (ptQ->tCounter == ptQ->tSize) : true);
}


