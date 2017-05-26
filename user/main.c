#include "app_cfg.h"

//for driver
#include "usart.h"
#include "systick.h"
#include "console.h"
#include "debug.h"
#include "mem_mange.h"
#include "list.h"

static void prvClockInit(void);
static void prvnvicInit(void);

int main(void)
{
    SystemCoreClockUpdate();
	prvnvicInit();
	Systick_Configuration();
	Usart_Configuration();
	console_init();

#ifdef DEBUG_MSG
	FunDebuginit();
#endif
	while(1)
	{
		console_task();


		
#ifdef DEBUG_MSG
		FunDebugtask();
#endif

	}
	//Should never reach this point!
}

/*
 *=============================================================================
 *						  System Clock Configuration
 *=============================================================================
 *		 System Clock source		  | PLL(HSI)
 *-----------------------------------------------------------------------------
 *		 SYSCLK 					  | 64000000 Hz
 *-----------------------------------------------------------------------------
 *		 HCLK					  | 64000000 Hz
 *-----------------------------------------------------------------------------
 *		 PCLK1					  | 32000000 Hz
 *-----------------------------------------------------------------------------
 *		 PCLK2					  | 64000000 Hz
 *-----------------------------------------------------------------------------
 *		 ADCCLK					  | 10670000 Hz
 *-----------------------------------------------------------------------------
 *		 AHB Prescaler			  | 1
 *-----------------------------------------------------------------------------
 *		 APB1 Prescaler 			  | 2
 *-----------------------------------------------------------------------------
 *		 APB2 Prescaler 			  | 1
 *-----------------------------------------------------------------------------
 *		 ADC Prescaler 			  | 6
 *-----------------------------------------------------------------------------
 *		 HSI Frequency			  | 8000000 Hz   /   2
 *-----------------------------------------------------------------------------
 *		 PLL MUL					  | 16
 *-----------------------------------------------------------------------------
 *		 VDD						  | 3.3 V
 *-----------------------------------------------------------------------------
 *		 Flash Latency			  | 2 WS
 *-----------------------------------------------------------------------------
 *=============================================================================
*/
//HSI Clock configuration
// 当不使用外部时钟时，配置内部时钟参数
static void prvClockInit(void)
{
    RCC_DeInit();
    /* Enable HSi */
    RCC_HSICmd(ENABLE);

    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* AHB = DIV1 , HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* APB2 = DIV1, PCLK2 = HCLK / 2 */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* APB1 = DIV2, PCLK1 = HCLK */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* ADCCLK = PCLK2/6 = 64 / 6 = 10.67 MHz*/
    RCC_ADCCLKConfig(RCC_PCLK2_Div4);

    /* PLLCLK = 8 / 2MHz * 16 = 64 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSI_Div2, RCC_PLLMul_16);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while(RCC_GetSYSCLKSource() != 0x08); 
}

//nvic configuration
static void prvnvicInit(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}




