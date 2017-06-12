
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
;  一段数据包含14位，周期长度为25ms
;  前两位是起始位s，通常是逻辑1，在RC5扩展模式下,第二位F将6位命令扩展到7位
;  (F做为高位MSB)
;  第三位是控制位C 它在每按下一个键后翻转，这样可以区分一个键到底是一直按着
;  还是松手后重复按
;  长按时，数据每隔114ms重发送一次，第三位不发生翻转
;
;
;  解码原理: 记录前一位的电平,上升沿解码，根据新的高低电平时间，决定本位是高还是低
;
;
; 硬件如H3V4F 接收器
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
