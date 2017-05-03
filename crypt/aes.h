/**
  ******************************************************************************
  * @file   aes.h
  * @author  
  * @version 
  * @date    
  * @brief    AES�����㷨   ����Ҫ�� ���ı���16*n byte
  ******************************************************************************
  * @attention      v1.1    jgb         20160907
  ******************************************************************************
  */

#ifndef __AES_H_
#define __AES_H_

#ifdef __cplusplus
    extern "C" {
#endif
/**************************ʹ�÷���**********************************
 * ������Կ���� 128��192��256
 * ������Կģʽ CBC,ECB
 * ������Կֵ
 * ��������ֵ(CBCģʽ�������Ҫ)
 * ����API
 *********************************************************************/
 
// �ӽ���ģʽ
#define AES_MODE_ECB    0               // �������뱾ģʽ��һ��ģʽ��
#define AES_MODE_CBC    1               // �����������ģʽ



// ��bitΪ��λ����Կ���ȣ�ֻ��Ϊ 128��192 �� 256 ����
#define AES_KEY_LENGTH  128
//����ģʽ
#define AES_MODE        AES_MODE_CBC

//ԭʼ��Կ��
// note : �䳤�ȱ���Ϊ AES_KEY_LENGTH/8 �ֽڡ�
#define AES_DEFAULT_KEY {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,\
                         0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,}
#if AES_MODE == AES_MODE_CBC
//�̶�16�ֽ�
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

//������CBCģʽ��������IVΪ��ʼֵ
void AES_Set_CBCDefaultIV(unsigned char ivec[16]);
/*
 *@  describe: Encrypt
 *@  input:   in  -- ����
 *@           inLen -- ���ݳ��ȣ�����Ϊ16byte����������
 *@           ivec -- ��ʼ������
 *@  output:  out -- ����
 *@           ivec -- cbcģʽ����������,ECBģʽΪNULL
 *@  return:  none
 */
void AES_Encrypt(const unsigned char *in, unsigned char *out, unsigned int inLen, unsigned char ivec[16]);
/*
 *@  describe: Decrypt
 *@  input��   in -- ���ģ�
 *@            inlen    -- ���ݳ���, ����Ϊ16byte���������� 
 *@            ivec -- ��ʼ������
 *@  output��  out  -- ����
 *@            ivec -- cbcģʽ����������,ECBģʽΪNULL
 *@  return��  none
 */
void AES_Decrypt(const unsigned char *in, unsigned char *out, unsigned int inLen, unsigned char ivec[16]);

#ifdef __cplusplus
    }
#endif


#endif  // __AES_H_
