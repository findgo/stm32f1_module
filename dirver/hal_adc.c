
#include "hal_adc.h"


typedef struct adc_config_s {
	uint32_t adcChannel;         // ADC1_INxx channel number
	uint8_t sampleTime;
} adc_config_t;

static const adc_config_t adcConfig[ADC_CHANNEL_MAX] = {
    {HAL_NTC1_ADC_CHANNEL, ADC_SampleTime_239Cycles5},
    {HAL_NTC2_ADC_CHANNEL, ADC_SampleTime_239Cycles5},
    {HAL_NTC3_ADC_CHANNEL, ADC_SampleTime_239Cycles5},
    {HAL_NTC4_ADC_CHANNEL, ADC_SampleTime_239Cycles5},
    {HAL_NTC5_ADC_CHANNEL, ADC_SampleTime_239Cycles5},
};
static volatile uint16_t adcValues[16][ADC_CHANNEL_MAX];

void haladcInit(void)
{
	uint8_t i;
	uint8_t configuredAdcChannels = 0;
	ADC_InitTypeDef		ADC_InitStructure;
	GPIO_InitTypeDef	GPIO_InitStructure;
	DMA_InitTypeDef	DMA_InitStructure;
	
	/* ADC1 DeInit */  
	ADC_DeInit(ADC_ACTIVE);

	RCC_ADCCLKConfig( RCC_PCLK2_Div6);
	
	/* GPIOC Periph clock enable */
	HAL_ADC_GPIO_PeriphClock_EN();

	/* ADC1 Periph clock enable */
	HAL_ADC_ACTIVE_Clock_EN();

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC1_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(HAL_NTC1_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC2_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(HAL_NTC2_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC3_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(HAL_NTC3_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC4_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(HAL_NTC4_ADC_PORT, &GPIO_InitStructure);

	/* Configure ADC Channel11 and channel10 as analog input */
	GPIO_InitStructure.GPIO_Pin = HAL_NTC5_ADC_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
	GPIO_Init(HAL_NTC5_ADC_PORT, &GPIO_InitStructure);
	
	/* Configure ADC Channel11 and channel10 as analog input */
//	GPIO_InitStructure.GPIO_Pin = HAL_NTC6_ADC_PIN;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz ;
//	GPIO_Init(HAL_NTC6_ADC_PORT, &GPIO_InitStructure);

	/* Initialize ADC structure */
	ADC_StructInit(&ADC_InitStructure);

	/* Configure the ADC1 in continuous mode withe a resolution equal to 12 bits  */
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE; 
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = configuredAdcChannels;
	ADC_Init(ADC_ACTIVE, &ADC_InitStructure); 

	for(i = 0; i < ADC_CHANNEL_MAX; i++){
		ADC_RegularChannelConfig(ADC_ACTIVE, adcConfig[i].adcChannel,i + 1, adcConfig[i].sampleTime); 
	}	
	ADC_TempSensorVrefintCmd(DISABLE);

	/* DMA1 clock enable */
	ADC_DMA_ACTIVE_Clock_EN();

	/* DMA1 Channel1 Config */
	DMA_DeInit(ADC_DMA_CHANNEL);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&ADC_ACTIVE->DR;
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&adcValues[0][0];
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = sizeof(adcValues)/sizeof(uint16_t);
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(ADC_DMA_CHANNEL, &DMA_InitStructure);

	/* DMA1 Channel1 enable */
	DMA_Cmd(ADC_DMA_CHANNEL, ENABLE);
	
	/* ADC reset Calibration */
    ADC_ResetCalibration(ADC1);
    while(ADC_GetResetCalibrationStatus(ADC1));
	/* ADC Calibration */
    ADC_StartCalibration(ADC1);  
    while(ADC_GetCalibrationStatus(ADC1)){};  

	/* Enable ADC_DMA */
	ADC_DMACmd(ADC_ACTIVE, ENABLE);  

	/* Enable the ADC peripheral */
	ADC_Cmd(ADC_ACTIVE, ENABLE);	 

	/* ADC1 regular Software Start Conv */ 
	ADC_SoftwareStartConvCmd(ADC_ACTIVE,ENABLE);
}


/**
  * @brief	读ADC的值，未处理
  * @param	channel 通道
  * @note		
  * @retval  None
  */
uint16_t adcGetRawValue(uint8_t channel)
{
	uint16_t aver_value = 0;
	uint8_t i;

	for(i = 0; i < sizeof(adcValues)/sizeof(uint16_t)/ADC_CHANNEL_MAX; i++){
		aver_value += adcValues[i][channel];
	}
	
    return aver_value;
}

/*
#define TEMP110_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7c2))
#define TEMP30_CAL_ADDR ((uint16_t *)((uint32_t)0x1ffff7b8))

// 未测试
int32_t adcInternalTemp(void)
{
	uint16_t sensor;
	int32_t temperature;
	
	sensor = adcGetRawValue(ADC_SENSOR);
	temperature = ((int32_t) sensor * 300 / 330) - ((int32_t)*TEMP30_CAL_ADDR);
	temperature = temperature *((int32_t)(110 - 30));
	temperature = temperature /((int32_t)(*TEMP110_CAL_ADDR - *TEMP30_CAL_ADDR)) + 30;

	return temperature;
}

//未测试
uint32_t adcInternalVref(void)
{
	uint16_t vtemp;

	vtemp = adcGetRawValue(ADC_VREF);

	return (uint32_t)vtemp;
}
*/



