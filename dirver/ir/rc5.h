
/********************************************************************
; RC5 format:
;
;      | S | F | C |   5 sytem bits    |    6 command bits     |
;      | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 |
;
;   -----+ +-+   +-+ +-+ +-+ +-+ +---+ +-+   +---+ +-+ +-+ +-+ +----
;        | | |   | | | | | | | | |   | | |   |   | | | | | | | |
;        +-+ +---+ +-+ +-+ +-+ +-+   +-+ +---+   +-+ +-+ +-+ +-+
;
; 
;  һ�����ݰ���14λ�����ڳ���Ϊ25ms
;  ǰ��λ����ʼλs��ͨ�����߼�1����RC5��չģʽ��,�ڶ�λF��6λ������չ��7λ
;  (F��Ϊ��λMSB)
;  ����λ�ǿ���λC ����ÿ����һ������ת��������������һ����������һֱ����
;  �������ֺ��ظ���
;  ����ʱ������ÿ��114ms�ط���һ�Σ�����λ��������ת
;
;
;  ����ԭ��: ��¼ǰһλ�ĵ�ƽ,�����ؽ��룬�����µĸߵ͵�ƽʱ�䣬������λ�Ǹ߻��ǵ�
;
;
; Ӳ����H3V4F ������
********************************************************************/

#ifndef __RC5_H_
#define __RC5_H_

/* RC5 Half bit period */
#define RC5_HALF_BIT_TIME       889             // 889 us  
#define RC5_HALF_BIT_TIME_MIN   640             // 640 us 
#define RC5_HALF_BIT_TIME_MAX   1140            // 1140 us 
/* RC5 Half bit period */
#define RC5_FULL_BIT_TIME       1778            // 1778 us  
#define RC5_FULL_BIT_TIME_MIN   1340            // 1340 us  
#define RC5_FULL_BIT_TIME_MAX   2220            // 2220 us      
/* RC5 message time 14 bit * 1.7778 = 24.889ms*/
#define RC5_MESSAGE_TIME       24889            // 24.889 ms  
#define RC5_MESSAGE_TIME_MIN   23644            // 23.644 ms  
#define RC5_MESSAGE_TIME_MAX   26133            // 26.133 ms 
/* RC5 message repetition time */
#define RC5_MESSAGE_REPEAT_TIME       113778    // 113.778 ms  
#define RC5_MESSAGE_REPEAT_TIME_MIN   108089    // 108.089 ms  
#define RC5_MESSAGE_REPEAT_TIME_MAX   119467    // 119.467 ms 
/* Carrier pulse bit time */
#define RC5_CARPULSE_BIT_TIME       27778       // 27.778 ms  
#define RC5_CARPULSE_TIME_MIN       27233       // 27.233 ms  
#define RC5_CARPULSE_TIME_MAX       28345       // 28.345 ms 

typedef struct rc5_s{
    unsigned char system;                  // Format 1 E/N  t  s4 s3 s3 s1 s0
    unsigned char command;                 // Format 0  0  c5 c4 c3 c2 c1 c0
    unsigned char flag;                    // flag rc5 ok;
}rc5_t;



#endif /* end of __RC5_H_ */
