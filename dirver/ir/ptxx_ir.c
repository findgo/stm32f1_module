



#include "ptxx_ir.h"


#define PTXX_Time_Sync_Lo       (4 * PTXX_TIME_PULSE_CYCLE)    // mark sync0
#define PTXX_Time_Sync_Hi       (124 * PTXX_TIME_PULSE_CYCLE)  // mark sync1
#define PTXX_Time_Narrow        (4 * PTXX_TIME_PULSE_CYCLE)   // mark 0
#define PTXX_Time_Wide          (12 * PTXX_TIME_PULSE_CYCLE) // mark 1
#define PTXX_Time_Limit         (PTXX_TIME_PULSE_CYCLE * 3 / 2) 

#define PTXX_BIT0   0x05    //(0b00000101)
#define PTXX_BIT1   0x0a    //(0b00001010)
#define PTXX_BITF   0x06    //(0b00000110)

static uint32_t pt_data[2];
static uint8_t pt_count;

static uint8_t pt_curflag = 0;
static uint8_t pt_curdat;
static uint32_t pt_curaddr;

#define PT_STATE_START 0
#define PT_STATE_SYNC0 1
#define PT_STATE_SYNC1 2
#define PT_STATE_CODE  3
#define PT_STATE_RESET()  do{ pt_state = PT_STATE_START; }while(0)

static uint8_t pt_state = 0;

/*双边沿捕获
 * delta_time 上下边沿之间的时间差
*/
void PTxx_Capture_Decode(uint32_t delta_time)
{
    uint32_t ir_tempaddr;   
    uint8_t ir_tempdat;
    uint8_t shift;
    uint8_t idx;
    uint8_t hafhex;
    
    if(pt_curflag)
       return;
    
    switch(pt_state){
    case PT_STATE_START:   
        pt_data[0] = 0;
        pt_data[1] = 0;
        pt_count = 0;
/*        pt_state = PT_STATE_SYNC0;
        break;
        
    case PT_STATE_SYNC0:  // sync bit low
        if((ir_temp.CapWide < (PTXX_Time_Sync_Lo + PTXX_Time_Limit)) 
            && (ir_temp.CapWide > (PTXX_Time_Sync_Lo - PTXX_Time_Limit)))
            pt_state = PT_STATE_SYNC1;
        else
            PT_STATE_RESET();
        break;
        
    case PT_STATE_SYNC1: // sync bit high */
        // capture sync code
        if((delta_time < (PTXX_Time_Sync_Hi + (PTXX_Time_Limit << 3))) 
            && (delta_time > (PTXX_Time_Sync_Hi - (PTXX_Time_Limit << 3))))
            pt_state = PT_STATE_CODE;
        else
            PT_STATE_RESET();
        break;
        
    case PT_STATE_CODE:
        idx = pt_count >> 5;// pt_count / 32
        pt_data[idx] <<= 1;
        if((delta_time < (PTXX_Time_Narrow + PTXX_Time_Limit )) 
            && (delta_time > (PTXX_Time_Narrow - PTXX_Time_Limit))){
            pt_data[idx] |= 0x00000000;
       }
        else if((delta_time < (PTXX_Time_Wide + PTXX_Time_Limit)) 
            && (delta_time > (PTXX_Time_Wide - PTXX_Time_Limit))){
            pt_data[idx] |= 0x00000001;
        }
        else{
            PT_STATE_RESET();
        }

        if(++pt_count >= 48){
            pt_count = 0;
            ir_tempaddr = 0;
            ir_tempdat = 0;
            for(idx = 0; idx < (PTXX_ADDR_BIT_NUM + PTXX_DATA_BIT_NUM); idx++){
                 shift = ((((idx / 8) > 0) ? 3 : 7) - (idx % 8)) * 4;
                 hafhex = (pt_data[idx / 8] >> shift) & 0x0f;
                 if(hafhex == PTXX_BIT0){
                    hafhex = 0;
                 }
                 else if(hafhex == PTXX_BIT1){
                    hafhex = 1;
                 }
                 else if(hafhex == PTXX_BITF){
                    hafhex = 2;
                 }else{
                    PT_STATE_RESET();
                    return;
                 }

                 if(idx < PTXX_ADDR_BIT_NUM){
                    ir_tempaddr = ir_tempaddr * 3 + hafhex;
                 }else{
                    ir_tempdat = (ir_tempdat << 1) + hafhex;
                 }
            }

            pt_curaddr = ir_tempaddr;
            pt_curdat = ir_tempdat;
            pt_curflag = 1;
            PT_STATE_RESET();            
        }
        break;
        
    default:
        pt_state = 0;
        break;
    }
}

u8 PTxx_GetCode(u32 *addr,u8 *cmd)
{
    if(pt_curflag){
        *addr = pt_curaddr;
        *cmd = pt_curdat;
        pt_curflag = 0;

        return 1;
    }

    return 0;
}

