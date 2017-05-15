
#ifndef __BSP_SPI_FLASH_H_
#define __BSP_SPI_FLASH_H_


#include "app_cfg.h"
#include "hal_spi.h"

/* 使能高级写模式，将会开辟4K Ram 扇区空间用于读写缓冲 */
#define SF_FLASH_WRITE_ADVANCED_MODE    (0)

// 抽像SPI底层
#define SF_SPI_CS_ASSERT()   HAL_SPI1_CS_HIGH()   
#define SF_SPI_CS_DEASSERT() HAL_SPI1_CS_LOW()

#define sf_spiReadWriteByte(_ucValue)   halSPI1_ReadWrite(_ucValue)

/* 定义串行Flash ID */
enum
{
    SST25VF016B_ID = 0xBF2541,
    MX25L1606E_ID  = 0xC22015,
    W25Q256FV_ID   = 0xEF6019,
    W25Q128FV_ID   = 0xEF6018,
    
    W25Q64FV_ID    = 0xEF6017,
    W25Q64BV_ID    = 0xEF4017,
    W25X64_ID      = 0xEF3017,
    
    W25Q32FV_ID    = 0xEF6016,
    W25Q32BV_ID    = 0xEF4016,
    W25X32_ID      = 0xEF3016,
    
    W25Q16BV_ID    = 0xEF4015,
    W25X16_ID      = 0xEF3015,
    
    W25Q80DV_ID    = 0xEF4014,
    W25X80_ID      = 0xEF3014,
    
    W25Q40BW_ID    = 0xEF5013,
    W25X40_ID      = 0xEF3013,

    W25X20_ID      = 0xEF3012,

    W25X10_ID      = 0xEF3011,
    
    GD25Q40B_ID    = 0xC84013,
    GD25Q20B_ID    = 0xC84012
};

typedef struct
{
    uint32_t ChipID;        /* 芯片ID */
    uint16_t PageSize;      /* 页面大小 */
    uint16_t SectorSize;    /* 扇区大小 */  
    uint32_t TotalSize;     /* 总容量 */
}sflash_info_t;

void sf_InitFlash(void);
sflash_info_t *sf_info(void);
uint32_t sf_ReadID(void);
void sf_EraseChip(void);
void sf_EraseSector(uint32_t _uiSectorAddr);
void sf_WritePage(uint32_t _PageAddr,uint8_t * _pBuf,uint16_t NumByteToWrite);
void sf_WriteSector(uint32_t _SectorAddr,uint8_t * _pBuf,uint16_t NumByteToWrite);
void sf_ReadBuffer(uint32_t _uiReadAddr, uint8_t * _pBuf,uint32_t _uiSize);
#if SF_FLASH_WRITE_ADVANCED_MODE == 1
uint8_t sf_WriteBuffer(uint32_t _uiWriteAddr, uint8_t* _pBuf,  uint16_t _usWriteSize);
#endif
#endif

