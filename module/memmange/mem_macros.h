/**
  ******************************************************************************
  * @file   mem_macros.h
  * @author   jgb
  * @version 
  * @date    
  * @brief    ���ĵ������ڿ��ƶѵ����ã�
  ******************************************************************************
  * @attention      v1.1   	jgb		20170512�ع� 
  ******************************************************************************
  */
  /*  ���ѹ���ȫ����ֲ��freertos �Ķѹ�������δ�������޸ģ�ֻ�޸����ٽ����ı���
  *  heap_1  ֻʵ�ַ��䣬��ʵ���ͷ�
  *  heap_2  ʵ�ַ�����ͷţ�����֧����Ƭ����
  *  heap_3  �Կ��ļ�malloc free�����ٽ籣��
  *  heap_4  ʵ�ַ�����ͷ�,����֧����Ƭ����
  *  heap_5 û�о�����Ҳ��ֲ��
  *
  *  һ������¶��ڵ�Ƭ��ֻҪ��heap_1��heap_1�����ˣ�ֻ���䲻�ͷ���heap_1Ҳ��
  *
  */
#ifndef __MEM_MACROS_H_
#define __MEM_MACROS_H_

#include <stdint.h>
#include "common_signal.h"

/* set support dynamic alloca compatibility. */
#define configSUPPORT_DYNAMIC_ALLOCATION    1
/* Default total heap size 17K*/
#define configTOTAL_HEAP_SIZE		( ( size_t ) ( 4 * 1024 ) )
/* set byte alignment */
#define portBYTE_ALIGNMENT			4

#define portPOINTER_SIZE_TYPE   uint32_t

/* set use malloc failed hook */
#define configUSE_MALLOC_FAILED_HOOK 0

/* for debug prinft malloc and free trace */
#define traceMALLOC( pvAddress, uiSize )
#define traceFREE( pvAddress, uiSize )
#define mtCOVERAGE_TEST_MARKER()
#define configASSERT( x )

typedef long BaseType_t;

#define pdFALSE			( ( BaseType_t ) 0 )
#define pdTRUE			( ( BaseType_t ) 1 )

#endif



