/********************************************************************
;
;
;  This package uses T0-CAP0 input (capture and interrupt on both edges)
;  CAP0.0 (P0.30) is connected to P0.16 (to check high / low level by software)
;
; RC5 format:
;
;      | S | F | C |   5 sytem bits    |    6 command bits     |
;      | 1 | 1 | 0 | 0 | 0 | 0 | 0 | 1 | 1 | 0 | 1 | 1 | 1 | 1 |
;
;   -----+ +-+   +-+ +-+ +-+ +-+ +---+ +-+   +---+ +-+ +-+ +-+ +----
;        | | |   | | | | | | | | |   | | |   |   | | | | | | | |
;        +-+ +---+ +-+ +-+ +-+ +-+   +-+ +---+   +-+ +-+ +-+ +-+
;
********************************************************************/

#include <LPC214x.h>                       // LPC214x definitions
#include "rc5.h"

rc5_t rc5_cur;

static signed int low_time;
static signed int high_time;
static unsigned char half_bit;
static unsigned char sys;                  // system byte
static unsigned char cmd;                  // Command byte


static void __RC5_Shift_Bit(char val)
{
    if (sys & 0x80){
        if (cmd & 0x80){                   // command full ?
            sys = 0;                       // yes, ERROR
            cmd = 0;
        }
        else{
            cmd = (cmd << 1) | val;        // shift command
        }
    }
    else{
        sys = (sys << 1) | val;            // shift system
    }
}

/************************************************************************
; RC5_Decode (we only take action at a rising edge)
;
; Half(prev) Bit   Low Time        High Time      Action     New Half Bit
; -------------------------------------------------------------------
;     0               0               0          Shift 0         0
;     0               0               1          Shift 1         1
;     0               1               0          -ERROR-         *
;     0               1               1          Shift 1,0       0
;     1               0               0          Shift 1         1
;     1               0               1          -ERROR-         *
;     1               1               0          Shift 1,0       0
;     1               1               1          -ERROR-         *
;
***********************************************************************/
//双边沿捕获 ----> 上升沿才进行解码
static void RC5_RisingDecode(signed int delta_time)
{
    unsigned char action;
    
    if (sys == 0){                  // First pulse ?
        low_time  = RC5_HALF_BIT_TIME; // assume short low time
        high_time = RC5_HALF_BIT_TIME; // assume short high time
        half_bit  = 1;             // assume half bit is true
        cmd = 0x02;                // = 00000010, prepare command byte
    }
    else{
        low_time = delta_time;          // rising, so capture low time
    }

    action = half_bit << 2;

// Check High Time
    if ((high_time > RC5_FULL_BIT_TIME_MIN) && (high_time < RC5_FULL_BIT_TIME_MAX)){
        action = action | 1;               // high_time = long
    }
    else if (!((high_time > RC5_HALF_BIT_TIME_MIN) && (high_time < RC5_HALF_BIT_TIME_MAX))){
        sys = 0;                           // RC5 ERROR
        cmd = 0;
        return;
    }

// Check Low Time
    if ((low_time > RC5_FULL_BIT_TIME_MIN) && (low_time < RC5_FULL_BIT_TIME_MAX)){
        action = action | 2;               // low_time = long
    }
    else if (!((low_time > RC5_HALF_BIT_TIME_MIN) && (low_time < RC5_HALF_BIT_TIME_MAX))){
        sys = 0;                           // RC5 ERROR
        cmd = 0;
        return;
    }

    switch (action){
    case 0:  __RC5_Shift_Bit(0);           // short low, short high, shift 0
           break;
    case 1:  __RC5_Shift_Bit(1);           // short low, long high, shift 1
           half_bit = 1;               // new half bit is true
           break;
    case 3:  __RC5_Shift_Bit(1);           // long low, long high, shift 1,0
           __RC5_Shift_Bit(0);
           break;
    case 4:  __RC5_Shift_Bit(1);           // short low, short high, shift 1
           break;
    case 6:  __RC5_Shift_Bit(1);           // long low, short high, shift 1,0
           __RC5_Shift_Bit(0);
           half_bit = 0;               // new half bit is false
           break;
    case 2:                              // long low, short high, ERROR
    case 5:                              // short low, long high, ERROR
    case 7: sys = 0;                    // long low, long high, ERROR
            cmd = 0;
    default: 
    break;                      // invalid
    }
}

//双边沿捕获 ----> 下降沿进行解码
static void RC5_FallingDecode(signed int delta_time)
{
    high_time = delta_time;    
}
// 超时调用 12ms超时
static void RC5_TimeOut(void)
{
    if (cmd & 0x80) {                    // command full ?
        rc5_cur.command = cmd & 0x7F;      // OK ! Save command byte
        rc5_cur.system = sys;              // save system byte
        rc5_cur.flag = 1;                  // set event to application
    }
    sys = 0;
    cmd = 0;
}

/********** DEMO ************/
void RC5_TimeOut_Isr(void)
{
// reset timer

// check timeout interrupt

	RC5_TimeOut();

// chear timeout interrupt flag	
}
void RC5_Capture_Isr(void)
{

	// reset timer

	// capture interrupt
	//check rising or falling edge
    if ()           // rising edge    
    {
        RC5_RisingDecode(delta_time);
    }
	else{                            // falling, so capture high time
        RC5_FallingDecode(delta_time);
    }

	// chear capture interrupt flag 
}

void RC5_Init(void)
{
//  12 ms High (idle) Timeout 初始一个dat 脚高电平 12ms超时

// Capture on both edges and enable the interrupt 初始一个双边沿捕获的中断

// Reset timer
// start timer
}
