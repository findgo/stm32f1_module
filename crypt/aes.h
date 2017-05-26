/**
  ******************************************************************************
  * @file   aes.h
  * @author  
  * @version 
  * @date    
  * @brief    AES加密算法   特殊要求 明文必需16*n byte
  ******************************************************************************
  * @attention      v1.1    jgb         20160907
  ******************************************************************************
  */

#ifndef __AES_H_
#define __AES_H_

#ifdef __cplusplus
    extern "C" {
#endif
/**************************使用方法**********************************
 * 定义密钥长度 128，192，256
 * 定义密钥模式 CBC,ECB
 * 定义密钥值
 * 定义向量值(CBC模式，如果需要)
 * 调用API
 *********************************************************************/
 
// 加解密模式
#define AES_MODE_ECB    0               // 电子密码本模式（一般模式）
#define AES_MODE_CBC    1               // 密码分组链接模式



// 以bit为单位的密钥长度，只能为 128，192 和 256 三种
#define AES_KEY_LENGTH  128
//加密模式
#define AES_MODE        AES_MODE_CBC

//原始密钥，
// note : 其长度必须为 AES_KEY_LENGTH/8 字节。
#define AES_DEFAULT_KEY {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,\
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,}
#if AES_MODE == AES_MODE_CBC
//固定16字节
#define AES_IV_KEY  {0x05, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,\
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00}
#endif

/*
 *@  describe: init
 *@  input:   none
 *@  output:  none
 *@  return:  none
 */
void AES_Init(void);

//仅用于CBC模式，设向量IV为初始值
void AES_Set_CBCDefaultIV(unsigned char ivec[16]);
/*
 *@  describe: Encrypt
 *@  input:   in  -- 明文
 *@           inLen -- 数据长度，必须为16byte的整倍数。
 *@           ivec -- 初始化向量
 *@  output:  out -- 密文
 *@           ivec -- cbc模式加链新向量,ECB模式为NULL
 *@  return:  none
 */
void AES_Encrypt(const unsigned char *in, unsigned char *out, unsigned int inLen, unsigned char ivec[16]);
/*
 *@  describe: Decrypt
 *@  input：   in -- 密文，
 *@            inlen    -- 数据长度, 必须为16byte的整倍数。 
 *@            ivec -- 初始化向量
 *@  output：  out  -- 明文
 *@            ivec -- cbc模式加链新向量,ECB模式为NULL
 *@  return：  none
 */
void AES_Decrypt(const unsigned char *in, unsigned char *out, unsigned int inLen, unsigned char ivec[16]);

#ifdef __cplusplus
    }
#endif


#endif  // __AES_H_
