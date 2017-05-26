﻿
#include "bsp_spi_flash.h"

#define SF_SECTOR_SIZE  (4 * 1024)

#define SF_CMD_AAI       0xAD       /* AAI 连续编程指令(FOR SST25VF016B) */
#define SF_CMD_DISWR      0x04      /* 禁止写, 退出AAI状态 */
#define SF_CMD_EWRSR      0x50      /* 允许写状态寄存器的命令 */
 
#define SF_CMD_WRITE     0x02       /*!< Write to Memory instruction */
#define SF_CMD_RDSR      0x05       /*!< Write Status Register instruction */
#define SF_CMD_WRSR      0x01       /*!< Write enable instruction */
#define SF_CMD_WREN      0x06       /*!< Read from Memory instruction */
#define SF_CMD_READ      0x03       /*!< Read Status Register instruction  */
#define SF_CMD_RDID      0x9F       /*!< Read identification */
#define SF_CMD_SE        0x20       /*!< Sector Erase instruction */
#define SF_CMD_BE        0xC7       /*!< Bulk Erase instruction */
#define SF_DUMMY_BYTE    0xA5       /*!< Dummy commad */

#define WIP_FLAG      0x01          /*!< Write In Progress (WIP) flag */



static sflash_info_t g_sf_info;

static void __sf_WriteEnable(void)
{
    /*!< Select the FLASH: Chip Select low */
    SF_SPI_CS_ASSERT();                                 
    /*!< Send "Write Enable" instruction */
    sf_spiReadWriteByte(SF_CMD_WREN);                       
    /*!< Deselect the FLASH: Chip Select high */
    SF_SPI_CS_DEASSERT();                               
}

static void __sf_WaitForWriteEnd(void)
{
    /*!< Select the FLASH: Chip Select low */
    SF_SPI_CS_ASSERT();     
    /*!< Send "Read Status Register" instruction */
    sf_spiReadWriteByte(SF_CMD_RDSR);       
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    while(sf_spiReadWriteByte(SF_DUMMY_BYTE) & WIP_FLAG);

    /*!< Deselect the FLASH: Chip Select high */
    SF_SPI_CS_DEASSERT();                               
}

/*
*********************************************************************************************************
*   函 数 名: sf_WriteStatus
*   功能说明: 写状态寄存器
*   形    参:  _ucValue : 状态寄存器的值
*   返 回 值: 无
*********************************************************************************************************
*/
static void __sf_WriteStatus(uint8_t _ucValue)
{
    SF_SPI_CS_ASSERT();                                 /* 使能片选 */
    sf_spiReadWriteByte(SF_CMD_WRSR);                       /* 发送命令， 写状态寄存器 */
    sf_spiReadWriteByte(_ucValue);                      /* 发送数据：状态寄存器的值 */
    SF_SPI_CS_DEASSERT();                               /* 禁能片选 */
}

void __sf_ReadInfo(void)
{
    g_sf_info.ChipID = sf_ReadID();/* 芯片ID */
    
    switch (g_sf_info.ChipID){
    case W25Q256FV_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 32 * 1024 * 1024; /* 总容量 = 32M */
        break;

    case W25Q128FV_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 16 * 1024 * 1024; /* 总容量 = 16M */
        break;

    case W25Q64FV_ID:
    case W25Q64BV_ID:
    case W25X64_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 8 * 1024 * 1024;  /* 总容量 = 8M */
        break;
        
    case W25Q32FV_ID:
    case W25Q32BV_ID:
    case W25X32_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 4 * 1024 * 1024;  /* 总容量 = 4M */
        break;
        
    case MX25L1606E_ID:
    case W25Q16BV_ID:
    case W25X16_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 2 * 1024 * 1024;  /* 总容量 = 2M */
        break;
    
    case W25Q80DV_ID:
    case W25X80_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 1 * 1024 * 1024;  /* 总容量 = 1M */
        break;
    
    case W25Q40BW_ID:
    case W25X40_ID:
    case GD25Q40B_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 512 * 1024;       /* 总容量 = 512k */
        break;

    case W25X20_ID:
    case GD25Q20B_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 256 * 1024;       /* 总容量 = 256k */
        break;

    case W25X10_ID:
        g_sf_info.PageSize = 256;               /* 页面大小 = 256字节 */
        g_sf_info.SectorSize = 4 * 1024;        /* 扇区大小 = 4K */
        g_sf_info.TotalSize = 128 * 1024;       /* 总容量 = 128K */
        break;

    default:
        g_sf_info.PageSize = 0;
        g_sf_info.SectorSize = 0;
        g_sf_info.TotalSize = 0;
        break;
    }
}

sflash_info_t *sf_info(void)
{
    return &g_sf_info;
}

uint32_t sf_ReadID(void)
{
    uint32_t uiID;
    uint8_t id1, id2, id3;

    SF_SPI_CS_ASSERT(); 
    /*!< Send "RDID " instruction */
    sf_spiReadWriteByte(SF_CMD_RDID);   
    /*!< Read a byte from the FLASH */
    id1 = sf_spiReadWriteByte(SF_DUMMY_BYTE);               
    id2 = sf_spiReadWriteByte(SF_DUMMY_BYTE);               
    id3 = sf_spiReadWriteByte(SF_DUMMY_BYTE);
    
    SF_SPI_CS_DEASSERT();                               

    uiID = ((uint32_t)id1 << 16) | ((uint32_t)id2 << 8) | id3;

    return uiID;
}

void sf_EraseSector(uint32_t _uiSectorAddr)
{
    /*!< Send write enable instruction */
    __sf_WriteEnable();                             

    SF_SPI_CS_ASSERT();                                 
    /*!< Send Sector Erase instruction */
    sf_spiReadWriteByte(SF_CMD_SE);
    /*!< Send Sector Erase instruction */
    sf_spiReadWriteByte((_uiSectorAddr & 0xFF0000) >> 16);
    sf_spiReadWriteByte((_uiSectorAddr & 0xFF00) >> 8); 
    sf_spiReadWriteByte(_uiSectorAddr & 0xFF);          
    SF_SPI_CS_DEASSERT();                               

    /*!< Wait the end of Flash writing */
    __sf_WaitForWriteEnd();                         
}

void sf_EraseChip(void)
{
    /*!< Send write enable instruction */
    __sf_WriteEnable();             

    SF_SPI_CS_ASSERT();             
    /*!< Bulk Erase */
    sf_spiReadWriteByte(SF_CMD_BE); 
    SF_SPI_CS_DEASSERT();           
    /*!< Wait the end of Flash writing */
    __sf_WaitForWriteEnd();         
}

void sf_ReadBuffer(uint32_t _uiReadAddr, uint8_t * _pBuf,uint32_t _uiSize)
{
    /* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
    if ((_uiSize == 0) ||(_uiReadAddr + _uiSize) > g_sf_info.TotalSize){
        return;
    } 

    SF_SPI_CS_ASSERT();                                     
    /*!< Send "Read from Memory " instruction */
    sf_spiReadWriteByte(SF_CMD_READ);                           
    /*!< Send ReadAddr high nibble address byte to read from */
    sf_spiReadWriteByte((_uiReadAddr & 0xFF0000) >> 16);    
    sf_spiReadWriteByte((_uiReadAddr & 0xFF00) >> 8);       
    sf_spiReadWriteByte(_uiReadAddr & 0xFF);                
    while (_uiSize--)
    {
        *_pBuf = sf_spiReadWriteByte(SF_DUMMY_BYTE);
        _pBuf++;
    }
    SF_SPI_CS_DEASSERT();                                   
}

/* NumByteToWrite must be equal or less than pagesize */
void sf_WritePage(uint32_t _PageAddr,uint8_t * _pBuf,uint16_t NumByteToWrite)
{
    /* for MX25L1606E 、 W25Q64BV */
    __sf_WriteEnable();                             

    SF_SPI_CS_ASSERT();                             
    /*!< Send "Write to Memory " instruction */
    sf_spiReadWriteByte(SF_CMD_WRITE);  
    /*!< Send WriteAddr high nibble address byte to write to */
    sf_spiReadWriteByte((_PageAddr & 0xFF0000) >> 16);
    sf_spiReadWriteByte((_PageAddr & 0xFF00) >> 8);     
    sf_spiReadWriteByte(_PageAddr & 0xFF);              

    while(NumByteToWrite--)
    {
        sf_spiReadWriteByte(*_pBuf);
        _pBuf++;
    }

    SF_SPI_CS_DEASSERT();                               

    __sf_WaitForWriteEnd();                     
}


void sf_WriteSector(uint32_t _SectorAddr,uint8_t * _pBuf,uint16_t NumByteToWrite)
{
    uint32_t i;
    
    /* for MX25L1606E 、 W25Q64BV */
    for (i = 0; i < NumByteToWrite / 256; i++){
        sf_WritePage(_SectorAddr,_pBuf,256);                       
        _SectorAddr += 256;
        _pBuf += 256;
    }
}

void sf_InitFlash(void)
{
    __sf_ReadInfo();                /* 自动识别芯片型号 */

    SF_SPI_CS_ASSERT();             /* 软件方式，使能串行Flash片选 */
    sf_spiReadWriteByte(SF_CMD_DISWR);      /* 发送禁止写入的命令,即使能软件写保护 */
    SF_SPI_CS_DEASSERT();               /* 软件方式，禁能串行Flash片选 */

    __sf_WaitForWriteEnd();     /* 等待串行Flash内部操作完成 */
    __sf_WriteStatus(0);            /* 解除所有BLOCK的写保护 */
}

#if SF_FLASH_WRITE_ADVANCED_MODE == 1
static uint8_t g_sfBuf[SF_SECTOR_SIZE]; /* 用于写函数，先读出整个sector，修改缓冲区后，再整个sector回写 */

/*
*********************************************************************************************************
*   函 数 名: sf_NeedErase
*   功能说明: 判断写PAGE前是否需要先擦除。
*   形    参:   _ucpOldBuf ： 旧数据
*              _ucpNewBuf ： 新数据
*              _uiLen ：数据个数，不能超过页面大小
*   返 回 值: 0 : 不需要擦除， 1 ：需要擦除
*********************************************************************************************************
*/
static uint8_t __sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
    uint16_t i;
    uint8_t ucOld;

    /*
    算法第1步：old 求反, new 不变
          old    new
          1101   0101
    ~     1111
        = 0010   0101

    算法第2步: old 求反的结果与 new 位与
          0010   old
    &     0101   new
         =0000

    算法第3步: 结果为0,则表示无需擦除. 否则表示需要擦除
    */

    for (i = 0; i < _usLen; i++)
    {
        ucOld = *_ucpOldBuf++;
        ucOld = ~ucOld;

        /* 注意错误的写法: if (ucOld & (*_ucpNewBuf++) != 0) */
        if ((ucOld & (*_ucpNewBuf++)) != 0)
        {
            return 1;
        }
    }
    return 0;
}
/*
*********************************************************************************************************
*   函 数 名: sf_CmpData
*   功能说明: 比较Flash的数据.
*   形    参:   _ucpTar : 数据缓冲区
*               _uiSrcAddr ：Flash地址
*               _uiSize ：数据个数, 可以大于PAGE_SIZE,但是不能超出芯片总容量
*   返 回 值: 0 = 相等, 1 = 不等
*********************************************************************************************************
*/
static uint8_t __sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
    uint8_t ucValue;

    /* 如果读取的数据长度为0或者超出串行Flash地址空间，则直接返回 */
    if ((_uiSrcAddr + _uiSize) > g_sf_info.TotalSize)
        return 1;

    if (_uiSize == 0)
        return 0;

    SF_SPI_CS_ASSERT();                                 /* 使能片选 */
    sf_spiReadWriteByte(SF_CMD_READ);                           /* 发送读命令 */
    sf_spiReadWriteByte((_uiSrcAddr & 0xFF0000) >> 16);     /* 发送扇区地址的高8bit */
    sf_spiReadWriteByte((_uiSrcAddr & 0xFF00) >> 8);        /* 发送扇区地址中间8bit */
    sf_spiReadWriteByte(_uiSrcAddr & 0xFF);                 /* 发送扇区地址低8bit */
    while (_uiSize--)
    {
        /* 读一个字节 */
        ucValue = sf_spiReadWriteByte(SF_DUMMY_BYTE);
        if (*_ucpTar++ != ucValue){
            SF_SPI_CS_DEASSERT();
            
            return 1;
        }
    }
    SF_SPI_CS_DEASSERT();
    return 0;
}

/*
*********************************************************************************************************
*   函 数 名: sf_AutoWriteSector
*   功能说明: 写1个Sector并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiWriteAddr ：目标区域首地址
*               _usSize ：数据个数，不能超过扇区面大小
*   返 回 值: 0 : 错误， 1 ： 成功
*********************************************************************************************************
*/
static uint8_t sf_AutoWriteSector( uint32_t _uiWrAddr, uint8_t *_ucpSrc, uint16_t _usWrLen)
{
    uint16_t i;
    uint16_t j;                 /* 用于延时 */
    uint32_t uiFirstAddr;       /* 扇区首址 */
    uint8_t ucNeedErase;        /* 1表示需要擦除 */
    uint8_t cRet;

    /* 长度为0时不继续操作,直接认为成功 */
    if (_usWrLen == 0){
        return 1;
    }

    /* 如果偏移地址超过芯片容量则退出 */
    if (_uiWrAddr >= g_sf_info.TotalSize){
        return 0;
    }

    /* 如果数据长度大于扇区容量，则退出 */
    if (_usWrLen > g_sf_info.SectorSize)
    {
        return 0;
    }

    /* 如果FLASH中的数据没有变化,则不写FLASH */
    sf_ReadBuffer(_uiWrAddr,g_sfBuf,_usWrLen);
    if (memcmp(g_sfBuf, _ucpSrc, _usWrLen) == 0){
        return 1;
    }

    /* 判断是否需要先擦除扇区 */
    /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
    ucNeedErase = 0;
    if (__sf_NeedErase(g_sfBuf, _ucpSrc, _usWrLen)){
        ucNeedErase = 1;
    }

    uiFirstAddr = _uiWrAddr & (~(g_sf_info.SectorSize - 1));

    if (_usWrLen == g_sf_info.SectorSize){      /* 整个扇区都改写 */
        for (i = 0; i < g_sf_info.SectorSize; i++){
            g_sfBuf[i] = _ucpSrc[i];
        }
    }else{                      /* 改写部分数据 */
        /* 先将整个扇区的数据读出 */
        sf_ReadBuffer(uiFirstAddr,g_sfBuf,g_sf_info.SectorSize);

        /* 再用新数据覆盖 */
        i = _uiWrAddr & (g_sf_info.SectorSize - 1);
        memcpy(&g_sfBuf[i], _ucpSrc, _usWrLen);
    }

    /* 写完之后进行校验，如果不正确则重写，最多3次 */
    cRet = 0;
    for (i = 0; i < 3; i++){
        /* 如果旧数据修改为新数据，所有位均是 1->0 或者 0->0, 则无需擦除,提高Flash寿命 */
        if (ucNeedErase == 1){
            sf_EraseSector(uiFirstAddr);        /* 擦除1个扇区 */
        }

        /* 编程一个Sector */
        sf_WriteSector(uiFirstAddr, g_sfBuf, g_sf_info.SectorSize);

        if (__sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0){
            cRet = 1;
            break;
        }else{
            if (__sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0){
                cRet = 1;
                break;
            }

            /* 失败后延迟一段时间再重试 */
            for (j = 0; j < 500; j++);
        }
    }

    return cRet;
}


/*
*********************************************************************************************************
*   函 数 名: sf_WriteBuffer
*   功能说明: 写1个扇区并校验,如果不正确则再重写两次。本函数自动完成擦除操作。
*   形    参:   _pBuf : 数据源缓冲区；
*               _uiWrAddr ：目标区域首地址
*               _usSize ：数据个数，不能超过页面大小
*   返 回 值: 1 : 成功， 0 ： 失败
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint32_t _uiWriteAddr, uint8_t* _pBuf,  uint16_t _usWriteSize)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = _uiWriteAddr % g_sf_info.SectorSize;
    count = g_sf_info.SectorSize - Addr;
    NumOfPage =  _usWriteSize / g_sf_info.SectorSize;
    NumOfSingle = _usWriteSize % g_sf_info.SectorSize;

    if (Addr == 0){ /* 起始地址是扇区首地址  */
        if (NumOfPage == 0){ /* 数据长度小于扇区大小 */
            if (sf_AutoWriteSector( _uiWriteAddr, _pBuf,_usWriteSize) == 0){
                return 0;
            }
        }else{  /* 数据长度大于等于扇区面大小 */

            while (NumOfPage--)
            {
                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, g_sf_info.SectorSize) == 0){
                    return 0;
                }
                _uiWriteAddr +=  g_sf_info.SectorSize;
                _pBuf += g_sf_info.SectorSize;
            }
            if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, NumOfSingle) == 0){
                return 0;
            }
        }
    }else{  /* 起始地址不是页面首地址  */
    
        if (NumOfPage == 0){ /* 数据长度小于页面大小 */
            if (NumOfSingle > count){ /* (_usWriteSize + _uiWriteAddr) > SPI_FLASH_PAGESIZE */
            
                temp = NumOfSingle - count;
                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, count) == 0){
                    return 0;
                }

                _uiWriteAddr +=  count;
                _pBuf += count;

                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, temp) == 0){
                    return 0;
                }
            }else{
                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, _usWriteSize) == 0){
                    return 0;
                }
            }
        }else{  /* 数据长度大于等于页面大小 */
        
            _usWriteSize -= count;
            NumOfPage =  _usWriteSize / g_sf_info.SectorSize;
            NumOfSingle = _usWriteSize % g_sf_info.SectorSize;

            if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, count) == 0){
                return 0;
            }

            _uiWriteAddr +=  count;
            _pBuf += count;

            while (NumOfPage--)
            {
                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, g_sf_info.SectorSize) == 0){
                    return 0;
                }
                _uiWriteAddr +=  g_sf_info.SectorSize;
                _pBuf += g_sf_info.SectorSize;
            }

            if (NumOfSingle != 0){
                if (sf_AutoWriteSector( _uiWriteAddr, _pBuf, NumOfSingle) == 0){
                    return 0;
                }
            }
        }
    }
    
    return 1;   /* 成功 */
}
#endif

uint8_t sf_StatusBusy(void)
{
    uint8_t status;
    
    /*!< Select the FLASH: Chip Select low */
    SF_SPI_CS_ASSERT();     
    /*!< Send "Read Status Register" instruction */
    sf_spiReadWriteByte(SF_CMD_RDSR);       
    /*!< Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    status = sf_spiReadWriteByte(SF_DUMMY_BYTE) & WIP_FLAG;
    /*!< Deselect the FLASH: Chip Select high */
    SF_SPI_CS_DEASSERT();

    return status;
}

void sf_StartEraseSectoreSequeue(uint32_t _uiSectorAddr)
{
    /*!< Send write enable instruction */
    __sf_WriteEnable();                             

    SF_SPI_CS_ASSERT();                                 
    /*!< Send Sector Erase instruction */
    sf_spiReadWriteByte(SF_CMD_SE);
    /*!< Send Sector Erase instruction */
    sf_spiReadWriteByte((_uiSectorAddr & 0xFF0000) >> 16);
    sf_spiReadWriteByte((_uiSectorAddr & 0xFF00) >> 8); 
    sf_spiReadWriteByte(_uiSectorAddr & 0xFF);          
    SF_SPI_CS_DEASSERT();                                   
}

void sf_StartWritePageSequeue(uint32_t _PageAddr,uint8_t * _pBuf,uint16_t NumByteToWrite)
{
    /* for MX25L1606E 、 W25Q64BV */
    __sf_WriteEnable();                             

    SF_SPI_CS_ASSERT();                             
    /*!< Send "Write to Memory " instruction */
    sf_spiReadWriteByte(SF_CMD_WRITE);  
    /*!< Send WriteAddr high nibble address byte to write to */
    sf_spiReadWriteByte((_PageAddr & 0xFF0000) >> 16);
    sf_spiReadWriteByte((_PageAddr & 0xFF00) >> 8);     
    sf_spiReadWriteByte(_PageAddr & 0xFF);              

    while(NumByteToWrite--)
    {
        sf_spiReadWriteByte(*_pBuf);
        _pBuf++;
    }

    SF_SPI_CS_DEASSERT();                               
}


#ifdef DEBUG_SFLASH

#include "debug.h"
#include "hal_spi.h"


static uint8_t testbuf[256];
void  FunDebuginit(void)
{
    sflash_info_t *pinfo;
    
    dbg_set_dbg_level(7);
    halSPI1_Init(SPI_BaudRatePrescaler_256);
    sf_InitFlash();
    pinfo = sf_info();

    sf_EraseSector(0);
    
    DBG_VERBOS("chip id:%d\r\n flash size:%d k\r\n",pinfo->ChipID,pinfo->TotalSize/1024);
    memset(testbuf,0xaa,256);
    sf_WritePage(0,testbuf,256);
    memset(testbuf,0,256);
    sf_ReadBuffer(0,testbuf,256);

    memset(testbuf,0xbb,256);
    sf_WriteBuffer(128,testbuf,256);
    memset(testbuf,0,256);
    sf_ReadBuffer(0,testbuf,256);
    
    memset(testbuf,0,256);
    sf_ReadBuffer(128,testbuf,256);
    while(1);
}


void FunDebugtask(void)
{
    
}
#endif

