
#include "mcfg.h"
#include "mtimers.h"

typedef struct {
    uint16_t valid;         // = 1,表明有有效数据band = 0,表明无有效band
    uint16_t band_index;    // 数据有效, 指示有效band index,数据无效，指示下一块可写band index
    uint32_t band_sn;       // 只记录有效数据sn号
}sector_info_t;

typedef struct  {
    uint8_t empty;
    uint8_t valid;
}load_result_t;

// 存入flash的宏 定义
// bandsn32 + cfg(NUM * sizeof(uint16_t)) + crc32
#define CFG_SAVE_BANDSN32_SIZE     (4)
#define CFG_SAVE_CRC32_SIZE        (4)
#define CFG_SAVE_BAND_SIZE       ((sizeof(mcfg_para_t) + 3) / 4 * 4 + CFG_SAVE_BANDSN32_SIZE + CFG_SAVE_CRC32_SIZE)  // hold 4 byte align

// 每一页 能存cfg最大个数  ( Sector /  bandSize = bandnum)
#define CFG_BAND_NUM_MAX  (CFG_FLASH_SECTOR_SIZE / CFG_SAVE_BAND_SIZE)

/* extern default para */
extern mcfg_para_t para_default;

static sector_info_t sector_info[CFG_FLASH_USE_SECTOR_NUM];
// 下一个要存储的sn编码  sn: 用于识别配置信息的,SN越大,则表示此信息最新
static volatile uint32_t next_sn;
// 下一个要存储的扇区位置
static volatile uint16_t next_sector;

// 当前配置参数
static mcfg_para_t cfg_curbuf;
// flash写入缓冲区
static uint32_t cfg_flashbuf[CFG_SAVE_BAND_SIZE / 4];
//
static mtimer_t cfgtime = MTIMER_INIT();

static const uint32_t crc32c_table[256] = {  
    0x00000000L, 0xF26B8303L, 0xE13B70F7L, 0x1350F3F4L,  
    0xC79A971FL, 0x35F1141CL, 0x26A1E7E8L, 0xD4CA64EBL,  
    0x8AD958CFL, 0x78B2DBCCL, 0x6BE22838L, 0x9989AB3BL,  
    0x4D43CFD0L, 0xBF284CD3L, 0xAC78BF27L, 0x5E133C24L,  
    0x105EC76FL, 0xE235446CL, 0xF165B798L, 0x030E349BL,  
    0xD7C45070L, 0x25AFD373L, 0x36FF2087L, 0xC494A384L,  
    0x9A879FA0L, 0x68EC1CA3L, 0x7BBCEF57L, 0x89D76C54L,  
    0x5D1D08BFL, 0xAF768BBCL, 0xBC267848L, 0x4E4DFB4BL,  
    0x20BD8EDEL, 0xD2D60DDDL, 0xC186FE29L, 0x33ED7D2AL,  
    0xE72719C1L, 0x154C9AC2L, 0x061C6936L, 0xF477EA35L,  
    0xAA64D611L, 0x580F5512L, 0x4B5FA6E6L, 0xB93425E5L,  
    0x6DFE410EL, 0x9F95C20DL, 0x8CC531F9L, 0x7EAEB2FAL,  
    0x30E349B1L, 0xC288CAB2L, 0xD1D83946L, 0x23B3BA45L,  
    0xF779DEAEL, 0x05125DADL, 0x1642AE59L, 0xE4292D5AL,  
    0xBA3A117EL, 0x4851927DL, 0x5B016189L, 0xA96AE28AL,  
    0x7DA08661L, 0x8FCB0562L, 0x9C9BF696L, 0x6EF07595L,  
    0x417B1DBCL, 0xB3109EBFL, 0xA0406D4BL, 0x522BEE48L,  
    0x86E18AA3L, 0x748A09A0L, 0x67DAFA54L, 0x95B17957L,  
    0xCBA24573L, 0x39C9C670L, 0x2A993584L, 0xD8F2B687L,  
    0x0C38D26CL, 0xFE53516FL, 0xED03A29BL, 0x1F682198L,  
    0x5125DAD3L, 0xA34E59D0L, 0xB01EAA24L, 0x42752927L,  
    0x96BF4DCCL, 0x64D4CECFL, 0x77843D3BL, 0x85EFBE38L,  
    0xDBFC821CL, 0x2997011FL, 0x3AC7F2EBL, 0xC8AC71E8L,  
    0x1C661503L, 0xEE0D9600L, 0xFD5D65F4L, 0x0F36E6F7L,  
    0x61C69362L, 0x93AD1061L, 0x80FDE395L, 0x72966096L,  
    0xA65C047DL, 0x5437877EL, 0x4767748AL, 0xB50CF789L,  
    0xEB1FCBADL, 0x197448AEL, 0x0A24BB5AL, 0xF84F3859L,  
    0x2C855CB2L, 0xDEEEDFB1L, 0xCDBE2C45L, 0x3FD5AF46L,  
    0x7198540DL, 0x83F3D70EL, 0x90A324FAL, 0x62C8A7F9L,  
    0xB602C312L, 0x44694011L, 0x5739B3E5L, 0xA55230E6L,  
    0xFB410CC2L, 0x092A8FC1L, 0x1A7A7C35L, 0xE811FF36L,  
    0x3CDB9BDDL, 0xCEB018DEL, 0xDDE0EB2AL, 0x2F8B6829L,  
    0x82F63B78L, 0x709DB87BL, 0x63CD4B8FL, 0x91A6C88CL,  
    0x456CAC67L, 0xB7072F64L, 0xA457DC90L, 0x563C5F93L,  
    0x082F63B7L, 0xFA44E0B4L, 0xE9141340L, 0x1B7F9043L,  
    0xCFB5F4A8L, 0x3DDE77ABL, 0x2E8E845FL, 0xDCE5075CL,  
    0x92A8FC17L, 0x60C37F14L, 0x73938CE0L, 0x81F80FE3L,  
    0x55326B08L, 0xA759E80BL, 0xB4091BFFL, 0x466298FCL,  
    0x1871A4D8L, 0xEA1A27DBL, 0xF94AD42FL, 0x0B21572CL,  
    0xDFEB33C7L, 0x2D80B0C4L, 0x3ED04330L, 0xCCBBC033L,  
    0xA24BB5A6L, 0x502036A5L, 0x4370C551L, 0xB11B4652L,  
    0x65D122B9L, 0x97BAA1BAL, 0x84EA524EL, 0x7681D14DL,  
    0x2892ED69L, 0xDAF96E6AL, 0xC9A99D9EL, 0x3BC21E9DL,  
    0xEF087A76L, 0x1D63F975L, 0x0E330A81L, 0xFC588982L,  
    0xB21572C9L, 0x407EF1CAL, 0x532E023EL, 0xA145813DL,  
    0x758FE5D6L, 0x87E466D5L, 0x94B49521L, 0x66DF1622L,  
    0x38CC2A06L, 0xCAA7A905L, 0xD9F75AF1L, 0x2B9CD9F2L,  
    0xFF56BD19L, 0x0D3D3E1AL, 0x1E6DCDEEL, 0xEC064EEDL,  
    0xC38D26C4L, 0x31E6A5C7L, 0x22B65633L, 0xD0DDD530L,  
    0x0417B1DBL, 0xF67C32D8L, 0xE52CC12CL, 0x1747422FL,  
    0x49547E0BL, 0xBB3FFD08L, 0xA86F0EFCL, 0x5A048DFFL,  
    0x8ECEE914L, 0x7CA56A17L, 0x6FF599E3L, 0x9D9E1AE0L,  
    0xD3D3E1ABL, 0x21B862A8L, 0x32E8915CL, 0xC083125FL,  
    0x144976B4L, 0xE622F5B7L, 0xF5720643L, 0x07198540L,  
    0x590AB964L, 0xAB613A67L, 0xB831C993L, 0x4A5A4A90L,  
    0x9E902E7BL, 0x6CFBAD78L, 0x7FAB5E8CL, 0x8DC0DD8FL,  
    0xE330A81AL, 0x115B2B19L, 0x020BD8EDL, 0xF0605BEEL,  
    0x24AA3F05L, 0xD6C1BC06L, 0xC5914FF2L, 0x37FACCF1L,  
    0x69E9F0D5L, 0x9B8273D6L, 0x88D28022L, 0x7AB90321L,  
    0xAE7367CAL, 0x5C18E4C9L, 0x4F48173DL, 0xBD23943EL,  
    0xF36E6F75L, 0x0105EC76L, 0x12551F82L, 0xE03E9C81L,  
    0x34F4F86AL, 0xC69F7B69L, 0xD5CF889DL, 0x27A40B9EL,  
    0x79B737BAL, 0x8BDCB4B9L, 0x988C474DL, 0x6AE7C44EL,  
    0xBE2DA0A5L, 0x4C4623A6L, 0x5F16D052L, 0xAD7D5351L  
};  

static uint32_t calculate_CRC32 (uint32_t uCRCValue,uint8_t *pStart, uint32_t uSize)  
{  
    while (uSize --)  
    {  
        uCRCValue = crc32c_table[(uCRCValue ^ *pStart++) & 0xFFL] ^ (uCRCValue >> 8);  
    }  
  
    return uCRCValue ^ 0xffffffffL;  
} 

//add  bandsn32  crc32
static void __cfgbufleft(void)
{
    uint16_t *p;
    cfg_flashbuf[0] = next_sn;
    // 清除那些需求对齐的不用的字节，使之为0xffff
    for (p = (uint16_t*) (cfg_flashbuf + 1) + sizeof(cfg_curbuf) / 2;
            p < (uint16_t*) (cfg_flashbuf + CFG_SAVE_BAND_SIZE / 4 - 1); ++p)
        *p = 0xFFFF;
            
    cfg_flashbuf[CFG_SAVE_BAND_SIZE / 4 - 1] = calculate_CRC32(CFG_CRC32_INIT_VALUE,(uint8_t *)cfg_flashbuf,CFG_SAVE_BAND_SIZE - CFG_SAVE_CRC32_SIZE);
}

/* 从指定地址读出数据  并判断是否为空，数据是否有效 */
static void __loadcfg(uint32_t addr_offset,load_result_t *result)
{
    uint16_t i;
    uint32_t checksum;
    
    while(CFGFLASHStatusBusy());// 忙 等芯片空闲
    CFGFLASH_Read(addr_offset, (uint8_t *)cfg_flashbuf , CFG_SAVE_BAND_SIZE);

    result->empty = 1;
    result->valid = 0;
    for (i = 0; i < CFG_SAVE_BAND_SIZE / 4; i++) {
       if(cfg_flashbuf[i] != 0xFFFFFFFF) {
           result->empty = 0;
           break;
       }
    }

    if (!result->empty){
        checksum = calculate_CRC32(CFG_CRC32_INIT_VALUE,(uint8_t *)cfg_flashbuf,CFG_SAVE_BAND_SIZE - CFG_SAVE_CRC32_SIZE);
        result->valid = checksum == cfg_flashbuf[CFG_SAVE_BAND_SIZE / 4 - 1]; 
    }
}

/**
 * find valid page to load,set next sector, next sn and next band_index to write
 * return <0, no valid band found, >=0 valid page to load
 */
 // TODO : 解决sn回绕问题
static int __findBand(void)
{
    uint16_t i;
    int sector_toload = -1;
    
    next_sector = 0;
    next_sn = 0xfffffffd;
    for (i = 0; i < CFG_FLASH_USE_SECTOR_NUM; ++i) {
        if(sector_info[i].valid) {
            // find which sector to load cfg  转化为有符号比较，解决回绕问题，请参考linux time_after的回绕
            if (sector_toload < 0 ||  ((int32_t)sector_info[sector_toload].band_sn - (int32_t)sector_info[i].band_sn) < 0){
                sector_toload = i;
            }
            //
            if (sector_info[next_sector].valid && ((int32_t)sector_info[i].band_sn - (int32_t)sector_info[next_sector].band_sn) < 0){
                next_sector = i;
            }
        }else {
            next_sector = i;
        }
    }

    // set next sn
    if (sector_toload >= 0)
        next_sn = sector_info[sector_toload].band_sn + 1;
    // set next band_index
    if(sector_info[next_sector].valid)
        sector_info[next_sector].band_index++;


    return sector_toload;
}


void mcfgLoadinit(void)
{
    uint16_t i;
    int j;
    int toload;
    uint32_t offset;
    load_result_t result0;

    // 初始化sector_info的管理信息
    for (i = 0; i < CFG_FLASH_USE_SECTOR_NUM; ++i) {
        sector_info[i].valid = 0;
        sector_info[i].band_index = 0;
        sector_info[i].band_sn = 0;
        // find this sector valid band ,from back to front
        for (j = CFG_BAND_NUM_MAX - 1; j >= 0; --j) {
            offset = CFG_FLASH_SECTOR_BASE_ADDRESS + i * CFG_FLASH_SECTOR_SIZE + j * CFG_SAVE_BAND_SIZE;
            __loadcfg(offset, &result0);

            // find a band in sector
            if (!result0.empty) {
                sector_info[i].valid = result0.valid;
                if (result0.valid) {
                    // 这里很关键，要记录当前有数据索引
                    sector_info[i].band_sn = cfg_flashbuf[0];                    
                    sector_info[i].band_index = j;
                }else{
                    sector_info[i].band_index = j + 1;
                }
                break;
            }
        }
    }

    toload = __findBand();
    // not found load and then set default para
    if (toload < 0) { 
        memcpy(&cfg_curbuf, &para_default, sizeof(cfg_curbuf));
    }else{
        if(toload == next_sector) // 这种情况，索引会被指向下一块空区域
            __loadcfg(CFG_FLASH_SECTOR_BASE_ADDRESS + CFG_FLASH_SECTOR_SIZE * toload + CFG_SAVE_BAND_SIZE * (sector_info[toload].band_index - 1),&result0);
        else
            __loadcfg(CFG_FLASH_SECTOR_BASE_ADDRESS + CFG_FLASH_SECTOR_SIZE * toload + CFG_SAVE_BAND_SIZE * sector_info[toload].band_index,&result0);
            
        if(!result0.empty && result0.valid)
            memcpy(&cfg_curbuf, cfg_flashbuf + 1, sizeof(cfg_curbuf));
        else
            memcpy(&cfg_curbuf, &para_default, sizeof(cfg_curbuf));
    }

    // start to flush cfg to flash time
    mtimer_start(&cfgtime,CFG_FLUSH_TIME);
}




mcfg_para_t *mcfgPara(void)
{
    return &cfg_curbuf;
}
void mcfgFlush(void)
{
    mtimer_start(&cfgtime,0);
}

enum {
  MCFG_NOTSTART = 0,
  MCFG_ENWRITE,
  MCFG_ENWRITEREMAIN,
  MCFG_ERASE,
  MCFG_ERASE_QUERY,
  MCFG_PROGRAM_QUERY_OVER,
};

static uint32_t remainByte;
static uint8_t remainPage;
static uint16_t offsetPageaddr;
static uint8_t cfgstate = MCFG_NOTSTART;
void mcfgUpdate(void)
{    
    load_result_t result;

    switch(cfgstate){
    case MCFG_NOTSTART:
        if(!mtimer_expired(&cfgtime))
            break;

        mtimer_start(&cfgtime,CFG_FLUSH_TIME);
        
        // 比较配置,是否要写
        if(!memcmp(cfg_flashbuf + 1, &cfg_curbuf,sizeof(cfg_curbuf)))
            break;// same as previous

        //start write to flash 
        memcpy(cfg_flashbuf + 1, &cfg_curbuf, sizeof(cfg_curbuf));
        __cfgbufleft();
        cfgstate = MCFG_ENWRITE;
        //break;  fall down to write
        
    case MCFG_ENWRITE:
        // 芯片忙,退出
        if(CFGFLASHStatusBusy())
            return;
        
        if (sector_info[next_sector].band_index < CFG_BAND_NUM_MAX) {
            uint32_t addrhead,addrtail;
            uint8_t pageidxhead,pageidxtail;
            uint8_t isband_inpage;

            uint16_t nowByte;
            
            addrhead = sector_info[next_sector].band_index * CFG_SAVE_BAND_SIZE;
            addrtail = addrhead + CFG_SAVE_BAND_SIZE - 1;

            pageidxhead = addrhead / CFG_FLASH_PAGE_SIZE;
            pageidxtail = addrtail / CFG_FLASH_PAGE_SIZE;
            if(pageidxhead != pageidxtail){
                isband_inpage = 0;                
                offsetPageaddr = (pageidxhead + 1) * CFG_FLASH_PAGE_SIZE;
                nowByte = offsetPageaddr - addrhead;
                remainByte = CFG_SAVE_BAND_SIZE - nowByte;
                remainPage = pageidxtail - pageidxhead - 1;
            }else{
                isband_inpage = 1; 
            }

            // 要写的数据在一页内
            if(isband_inpage){
                cfgstate = MCFG_PROGRAM_QUERY_OVER;
                CFGFLASH_WritePageSequeue(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE + sector_info[next_sector].band_index * CFG_SAVE_BAND_SIZE,
                            (void*)cfg_flashbuf, CFG_SAVE_BAND_SIZE);
            }else{ // 跨页写，先写前一页多的数据
                cfgstate = MCFG_ENWRITEREMAIN;
                CFGFLASH_WritePageSequeue(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE + sector_info[next_sector].band_index * CFG_SAVE_BAND_SIZE,
                            (void*)cfg_flashbuf, nowByte);
            }
        } else { // 超出扇区索引,进行擦除再写
            cfgstate = MCFG_ERASE_QUERY;
            CFGFLASH_EraseSectorSequeue(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE);
        }
        break;

    case MCFG_ENWRITEREMAIN:
        if (!CFGFLASHStatusBusy()) {            
            if(remainPage){
                // 数据地址要偏移,要4字节偏移
                CFGFLASH_WritePageSequeue(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE + offsetPageaddr,
                            (void*)&cfg_flashbuf[ (CFG_SAVE_BAND_SIZE - remainByte) / 4 ], CFG_FLASH_PAGE_SIZE);
                remainByte -= CFG_FLASH_PAGE_SIZE;
                remainPage--;
                offsetPageaddr += CFG_FLASH_PAGE_SIZE;
                
                if(!remainByte)
                    cfgstate = MCFG_PROGRAM_QUERY_OVER;
            }else{
                CFGFLASH_WritePageSequeue(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE  + offsetPageaddr,
                            (void*)&cfg_flashbuf[ (CFG_SAVE_BAND_SIZE - remainByte) / 4 ], remainByte);
                remainByte = 0;
                offsetPageaddr = 0;
                cfgstate = MCFG_PROGRAM_QUERY_OVER;
            }
        }        
        break;
        
    case MCFG_ERASE_QUERY:
        if (!CFGFLASHStatusBusy()) {            
            sector_info[next_sector].valid = 0;
            sector_info[next_sector].band_index = 0;
            cfgstate = MCFG_ENWRITE;
        }
        break;

    case MCFG_PROGRAM_QUERY_OVER:
        if (!CFGFLASHStatusBusy()) {

            __loadcfg(CFG_FLASH_SECTOR_BASE_ADDRESS + next_sector * CFG_FLASH_SECTOR_SIZE + sector_info[next_sector].band_index * CFG_SAVE_BAND_SIZE
                ,&result);

            if(result.empty || !result.valid){ 
                // 无法写入，为空? 可能也是坏块，那写下一个地址
                cfgstate = MCFG_ENWRITE;
                //reload write to flash 
                memcpy(cfg_flashbuf + 1, &cfg_curbuf, sizeof(cfg_curbuf));
                __cfgbufleft();
                sector_info[next_sector].band_index++;
            }else{ //数据不为空，且有效
                cfgstate = MCFG_NOTSTART;
                sector_info[next_sector].valid = 1;
                sector_info[next_sector].band_sn = next_sn;
                __findBand();
            }
        }
        break;

    default: 
        cfgstate = MCFG_NOTSTART;
        break;
        
    }
}


#ifdef MCFG_DEBUG

#include "debug.h"
#include "hal_spi.h"
#include "bsp_spi_flash.h"

void  funcheckinit(void)
{
    uint16_t erase = 0;
    
    dbg_set_dbg_level(7);
    halSPI1_Init(SPI_BaudRatePrescaler_256);// 初始化SPI硬件底层
    sf_InitFlash(); // 初始化FLASH
    // 选择是否要擦除flash芯片进行测试
    //sf_EraseSector(0);
    if(erase)
        sf_EraseChip();
    mcfgLoadinit(); // 初始化mcfg
}

void funchecktask(void)
{
    mcfgUpdate();
}







#endif

