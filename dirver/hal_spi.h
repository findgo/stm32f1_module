/**
  ******************************************************************************
  * @file   hal_spi.h
  * @author  
  * @version 
  * @date    
  * @brief    
  ******************************************************************************
  * @attention    v1.1      jgb                 20170515 
  ******************************************************************************
  */
#ifndef __HAL_SPI_H_
#define __HAL_SPI_H_

/*
 *  硬件SPI
 *  模式0  默认低，第一上升边沿捕获
 *  模式1  默认低，第二上升边沿捕获
 *  模式2  默认高，第一下降边沿捕获
 *  模式3  默认高，第二下降边沿捕获
*/


#include "app_cfg.h"

//spi1
#define HAL_SPI1_MODE        (3)
/*定义硬件IO*/
#define HAL_SPI1_SCK_PORT        GPIOA
#define HAL_SPI1_SCK_PIN         GPIO_Pin_5
#define HAL_SPI1_MISO_PORT       GPIOA
#define HAL_SPI1_MISO_PIN        GPIO_Pin_6
#define HAL_SPI1_MOSI_PORT       GPIOA
#define HAL_SPI1_MOSI_PIN        GPIO_Pin_7
#define HAL_SPI1_CS_PORT         GPIOA
#define HAL_SPI1_CS_PIN          GPIO_Pin_2

#define HAL_SPI1_GPIO_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO ,ENABLE );}while(0)
#define HAL_SPI1_ACTIVE_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_SPI1 ,ENABLE );}while(0)

#define HAL_SPI1_CS_HIGH()  (PAout(2) = 1)
#define HAL_SPI1_CS_LOW()   (PAout(2) = 0)

// spi2
#define HAL_SPI2_MODE        (0)
/*定义硬件IO*/
#define HAL_SPI2_SCK_PORT        GPIOB
#define HAL_SPI2_SCK_PIN         GPIO_Pin_13
#define HAL_SPI2_MISO_PORT       GPIOB
#define HAL_SPI2_MISO_PIN        GPIO_Pin_14
#define HAL_SPI2_MOSI_PORT       GPIOB
#define HAL_SPI2_MOSI_PIN        GPIO_Pin_15
#define HAL_SPI2_CS_PORT         GPIOB
#define HAL_SPI2_CS_PIN          GPIO_Pin_12

#define HAL_SPI2_GPIO_CLOCK_EN()	do{RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO ,ENABLE );}while(0)
#define HAL_SPI2_ACTIVE_CLOCK_EN()	do{RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);}while(0)

#define HAL_SPI2_CS_HIGH()  (PAout(12) = 1)
#define HAL_SPI2_CS_LOW()  (PAout(12) = 0)


void halSPI1_Init(uint16_t baud);									// SPI1初始化
uint8_t halSPI1_ReadWrite(uint8_t writedat);			// SPI1读写一字节数据
void halSPI2_Init(uint16_t baud);
uint8_t halSPI2_ReadWrite(uint8_t writedat);

#endif

