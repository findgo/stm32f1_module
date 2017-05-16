
#include "bsp_spi_flash.h"

#define SF_SECTOR_SIZE  (4 * 1024)

#define SF_CMD_AAI       0xAD       /* AAI �������ָ��(FOR SST25VF016B) */
#define SF_CMD_DISWR      0x04      /* ��ֹд, �˳�AAI״̬ */
#define SF_CMD_EWRSR      0x50      /* ����д״̬�Ĵ��������� */
 
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
*   �� �� ��: sf_WriteStatus
*   ����˵��: д״̬�Ĵ���
*   ��    ��:  _ucValue : ״̬�Ĵ�����ֵ
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void __sf_WriteStatus(uint8_t _ucValue)
{
    SF_SPI_CS_ASSERT();                                 /* ʹ��Ƭѡ */
    sf_spiReadWriteByte(SF_CMD_WRSR);                       /* ������� д״̬�Ĵ��� */
    sf_spiReadWriteByte(_ucValue);                      /* �������ݣ�״̬�Ĵ�����ֵ */
    SF_SPI_CS_DEASSERT();                               /* ����Ƭѡ */
}

void __sf_ReadInfo(void)
{
    g_sf_info.ChipID = sf_ReadID();/* оƬID */
    
    switch (g_sf_info.ChipID){
    case W25Q256FV_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 32 * 1024 * 1024; /* ������ = 32M */
        break;

    case W25Q128FV_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 16 * 1024 * 1024; /* ������ = 16M */
        break;

    case W25Q64FV_ID:
    case W25Q64BV_ID:
    case W25X64_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 8 * 1024 * 1024;  /* ������ = 8M */
        break;
        
    case W25Q32FV_ID:
    case W25Q32BV_ID:
    case W25X32_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 4 * 1024 * 1024;  /* ������ = 4M */
        break;
        
    case MX25L1606E_ID:
    case W25Q16BV_ID:
    case W25X16_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 2 * 1024 * 1024;  /* ������ = 2M */
        break;
    
    case W25Q80DV_ID:
    case W25X80_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 1 * 1024 * 1024;  /* ������ = 1M */
        break;
    
    case W25Q40BW_ID:
    case W25X40_ID:
    case GD25Q40B_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 512 * 1024;       /* ������ = 512k */
        break;

    case W25X20_ID:
    case GD25Q20B_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 256 * 1024;       /* ������ = 256k */
        break;

    case W25X10_ID:
        g_sf_info.PageSize = 256;               /* ҳ���С = 256�ֽ� */
        g_sf_info.SectorSize = 4 * 1024;        /* ������С = 4K */
        g_sf_info.TotalSize = 128 * 1024;       /* ������ = 128K */
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
    /* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
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
    /* for MX25L1606E �� W25Q64BV */
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
    
    /* for MX25L1606E �� W25Q64BV */
    for (i = 0; i < NumByteToWrite / 256; i++){
        sf_WritePage(_SectorAddr,_pBuf,256);                       
        _SectorAddr += 256;
        _pBuf += 256;
    }
}

void sf_InitFlash(void)
{
    __sf_ReadInfo();                /* �Զ�ʶ��оƬ�ͺ� */

    SF_SPI_CS_ASSERT();             /* �����ʽ��ʹ�ܴ���FlashƬѡ */
    sf_spiReadWriteByte(SF_CMD_DISWR);      /* ���ͽ�ֹд�������,��ʹ�����д���� */
    SF_SPI_CS_DEASSERT();               /* �����ʽ�����ܴ���FlashƬѡ */

    __sf_WaitForWriteEnd();     /* �ȴ�����Flash�ڲ�������� */
    __sf_WriteStatus(0);            /* �������BLOCK��д���� */
}

#if SF_FLASH_WRITE_ADVANCED_MODE == 1
static uint8_t g_sfBuf[SF_SECTOR_SIZE]; /* ����д�������ȶ�������sector���޸Ļ�������������sector��д */

/*
*********************************************************************************************************
*   �� �� ��: sf_NeedErase
*   ����˵��: �ж�дPAGEǰ�Ƿ���Ҫ�Ȳ�����
*   ��    ��:   _ucpOldBuf �� ������
*              _ucpNewBuf �� ������
*              _uiLen �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: 0 : ����Ҫ������ 1 ����Ҫ����
*********************************************************************************************************
*/
static uint8_t __sf_NeedErase(uint8_t * _ucpOldBuf, uint8_t *_ucpNewBuf, uint16_t _usLen)
{
    uint16_t i;
    uint8_t ucOld;

    /*
    �㷨��1����old ��, new ����
          old    new
          1101   0101
    ~     1111
        = 0010   0101

    �㷨��2��: old �󷴵Ľ���� new λ��
          0010   old
    &     0101   new
         =0000

    �㷨��3��: ���Ϊ0,���ʾ�������. �����ʾ��Ҫ����
    */

    for (i = 0; i < _usLen; i++)
    {
        ucOld = *_ucpOldBuf++;
        ucOld = ~ucOld;

        /* ע������д��: if (ucOld & (*_ucpNewBuf++) != 0) */
        if ((ucOld & (*_ucpNewBuf++)) != 0)
        {
            return 1;
        }
    }
    return 0;
}
/*
*********************************************************************************************************
*   �� �� ��: sf_CmpData
*   ����˵��: �Ƚ�Flash������.
*   ��    ��:   _ucpTar : ���ݻ�����
*               _uiSrcAddr ��Flash��ַ
*               _uiSize �����ݸ���, ���Դ���PAGE_SIZE,���ǲ��ܳ���оƬ������
*   �� �� ֵ: 0 = ���, 1 = ����
*********************************************************************************************************
*/
static uint8_t __sf_CmpData(uint32_t _uiSrcAddr, uint8_t *_ucpTar, uint32_t _uiSize)
{
    uint8_t ucValue;

    /* �����ȡ�����ݳ���Ϊ0���߳�������Flash��ַ�ռ䣬��ֱ�ӷ��� */
    if ((_uiSrcAddr + _uiSize) > g_sf_info.TotalSize)
        return 1;

    if (_uiSize == 0)
        return 0;

    SF_SPI_CS_ASSERT();                                 /* ʹ��Ƭѡ */
    sf_spiReadWriteByte(SF_CMD_READ);                           /* ���Ͷ����� */
    sf_spiReadWriteByte((_uiSrcAddr & 0xFF0000) >> 16);     /* ����������ַ�ĸ�8bit */
    sf_spiReadWriteByte((_uiSrcAddr & 0xFF00) >> 8);        /* ����������ַ�м�8bit */
    sf_spiReadWriteByte(_uiSrcAddr & 0xFF);                 /* ����������ַ��8bit */
    while (_uiSize--)
    {
        /* ��һ���ֽ� */
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
*   �� �� ��: sf_AutoWriteSector
*   ����˵��: д1��Sector��У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*   ��    ��:   _pBuf : ����Դ��������
*               _uiWriteAddr ��Ŀ�������׵�ַ
*               _usSize �����ݸ��������ܳ����������С
*   �� �� ֵ: 0 : ���� 1 �� �ɹ�
*********************************************************************************************************
*/
static uint8_t sf_AutoWriteSector( uint32_t _uiWrAddr, uint8_t *_ucpSrc, uint16_t _usWrLen)
{
    uint16_t i;
    uint16_t j;                 /* ������ʱ */
    uint32_t uiFirstAddr;       /* ������ַ */
    uint8_t ucNeedErase;        /* 1��ʾ��Ҫ���� */
    uint8_t cRet;

    /* ����Ϊ0ʱ����������,ֱ����Ϊ�ɹ� */
    if (_usWrLen == 0){
        return 1;
    }

    /* ���ƫ�Ƶ�ַ����оƬ�������˳� */
    if (_uiWrAddr >= g_sf_info.TotalSize){
        return 0;
    }

    /* ������ݳ��ȴ����������������˳� */
    if (_usWrLen > g_sf_info.SectorSize)
    {
        return 0;
    }

    /* ���FLASH�е�����û�б仯,��дFLASH */
    sf_ReadBuffer(_uiWrAddr,g_sfBuf,_usWrLen);
    if (memcmp(g_sfBuf, _ucpSrc, _usWrLen) == 0){
        return 1;
    }

    /* �ж��Ƿ���Ҫ�Ȳ������� */
    /* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
    ucNeedErase = 0;
    if (__sf_NeedErase(g_sfBuf, _ucpSrc, _usWrLen)){
        ucNeedErase = 1;
    }

    uiFirstAddr = _uiWrAddr & (~(g_sf_info.SectorSize - 1));

    if (_usWrLen == g_sf_info.SectorSize){      /* ������������д */
        for (i = 0; i < g_sf_info.SectorSize; i++){
            g_sfBuf[i] = _ucpSrc[i];
        }
    }else{                      /* ��д�������� */
        /* �Ƚ��������������ݶ��� */
        sf_ReadBuffer(uiFirstAddr,g_sfBuf,g_sf_info.SectorSize);

        /* ���������ݸ��� */
        i = _uiWrAddr & (g_sf_info.SectorSize - 1);
        memcpy(&g_sfBuf[i], _ucpSrc, _usWrLen);
    }

    /* д��֮�����У�飬�������ȷ����д�����3�� */
    cRet = 0;
    for (i = 0; i < 3; i++){
        /* ����������޸�Ϊ�����ݣ�����λ���� 1->0 ���� 0->0, ���������,���Flash���� */
        if (ucNeedErase == 1){
            sf_EraseSector(uiFirstAddr);        /* ����1������ */
        }

        /* ���һ��Sector */
        sf_WriteSector(uiFirstAddr, g_sfBuf, g_sf_info.SectorSize);

        if (__sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0){
            cRet = 1;
            break;
        }else{
            if (__sf_CmpData(_uiWrAddr, _ucpSrc, _usWrLen) == 0){
                cRet = 1;
                break;
            }

            /* ʧ�ܺ��ӳ�һ��ʱ�������� */
            for (j = 0; j < 500; j++);
        }
    }

    return cRet;
}


/*
*********************************************************************************************************
*   �� �� ��: sf_WriteBuffer
*   ����˵��: д1��������У��,�������ȷ������д���Ρ��������Զ���ɲ���������
*   ��    ��:   _pBuf : ����Դ��������
*               _uiWrAddr ��Ŀ�������׵�ַ
*               _usSize �����ݸ��������ܳ���ҳ���С
*   �� �� ֵ: 1 : �ɹ��� 0 �� ʧ��
*********************************************************************************************************
*/
uint8_t sf_WriteBuffer(uint32_t _uiWriteAddr, uint8_t* _pBuf,  uint16_t _usWriteSize)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = _uiWriteAddr % g_sf_info.SectorSize;
    count = g_sf_info.SectorSize - Addr;
    NumOfPage =  _usWriteSize / g_sf_info.SectorSize;
    NumOfSingle = _usWriteSize % g_sf_info.SectorSize;

    if (Addr == 0){ /* ��ʼ��ַ�������׵�ַ  */
        if (NumOfPage == 0){ /* ���ݳ���С��������С */
            if (sf_AutoWriteSector( _uiWriteAddr, _pBuf,_usWriteSize) == 0){
                return 0;
            }
        }else{  /* ���ݳ��ȴ��ڵ����������С */

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
    }else{  /* ��ʼ��ַ����ҳ���׵�ַ  */
    
        if (NumOfPage == 0){ /* ���ݳ���С��ҳ���С */
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
        }else{  /* ���ݳ��ȴ��ڵ���ҳ���С */
        
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
    
    return 1;   /* �ɹ� */
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
    /* for MX25L1606E �� W25Q64BV */
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
void  funcheckinit(void)
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


void funchecktask(void)
{
    
}
#endif

