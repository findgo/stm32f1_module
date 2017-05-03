/**
  ******************************************************************************
  * @file   mkey.h
  * @author   jgb
  * @version 
  * @date    
  * @brief    keyģ��
  ******************************************************************************
  * @attention      v1.1   	jgb		20151117�ع� 		
  			      v1.2        		20170306�޸Ŀ�ܣ�ֱ�ӷ��ؼ�ֵ
  			      v1.3             20170429�ٴ��ع�����������û��޸Ľ���       
  ******************************************************************************
  */
/*
; ���ڰ�������˵��
; ֧�ֶ̰���������ʱ����������ʱ�����ᴥ���̰�Ч��
; ֧�ֶ̰���������������ʱ���ǲ��ᴥ���̰�״̬����Ȼ�����Ͷ̰���ֵһ���������������������ǰ�и�Ĭ�ϳ���ʱ
;
*/
#ifndef __MKEY_H_
#define __MKEY_H_

#include "app_cfg.h"

//��������С
#define KEY_QUEUE_MAX_SIZE		3
//Ĭ���˲�ʱ��,����key_decetor_task
#define KEY_FILTER_TIME     	2   // �˲�ʱ��
//�����֧�ֳ�������֧���������и�����ʱ��������Ĭ��ʱ��
//�����������������֧�֣�ֱ�ӵ�̧��
#define KEY_LONGTOREAP_TIME     50

typedef uint8_t (*KeyDownFunc)(void);

typedef struct {
    KeyDownFunc IsKeyDownFunc;  // �������µ��жϺ���,1��ʾ���� 
    void *next;
    
    uint8_t filterCount;             // �˲�����ʱ��
    uint8_t filterTime;              // �˲�ʱ��(�����Ϊ0����Ĭ��Ϊ20ms)
    uint16_t longrepCount;           // �����������ü����� 
    uint16_t longTime;               // �������³���ʱ��, 0��ʾ����ⳤ�� 
    uint8_t repeatSpeed;             // ����ʱ��

    uint8_t KeyCodeUp;               // �����ֵ
    uint8_t KeyCodeDown;             // �̰���ֵ
    uint8_t KeyCodeLong;             // ������ֵ
    uint8_t state;                   // ״̬��
}key_cfg_t;

/*����ֵ �û��޸�*/
enum
{
	KEY_NULL = 0,			/* 0 ��ʾ�ް����¼� */	

	KEY_PRESS1_DOWN,			
	KEY_PRESS1_LONG, 		
	KEY_PRESS1_UP
};

void mkey_create(key_cfg_t *cfg,
                KeyDownFunc IsKeyDownFunc,      // ����ú��� ������ ����1
                uint8_t KeyCodeDown,    
                uint8_t KeyCodeLong,
                uint8_t KeyCodeUp,              // Ϊ KEY_NULL��ʾ��֧��̧��
                uint8_t filterTime,             // Ϊ0 ��ʹ��Ĭ���˲�ʱ��
                uint16_t longTime,              // Ϊ0 ��ʾ��֧�ֳ���
                uint8_t repeatSpeed);           // Ϊ 0 ��ʾ��֧������

//ÿ��һ��������һ�μ�ɨ һ��Ϊ10ms
void mkey_decetor_task(void);
uint8_t mkey_getvalue(void);

#endif

