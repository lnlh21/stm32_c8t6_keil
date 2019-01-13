/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : Flash_Api.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��9��10��
  ����޸�   :
  ��������   : Flash_Api.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��9��10��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __FLASH_API_H__
#define __FLASH_API_H__

/* STM32F103RBT6    FLASH:128K     RAM:20K    
   ҳ���СΪ           2K
   ��ҳ��               128
   
-----------------   0x08000000
|               |
|               |
|  BOOT����(32K)|
|               |
|               |
-----------------   0x08008000
|               |
|               |
|  ������(94K)  |
|               |
|               |
-----------------   0x0801F800
|  ������(1K)   |
-----------------   0x0801FC00
| BOOT������(1K)|
-----------------   0x08020000
*/



#define FLASH_API_APP_AREA_START            0x08008000
#define FLASH_API_APP_BEGIN                 0x08008200

/* ��������ʼ��ַ */
#define FLASH_API_DATA_ADDR_START           0x0801F800

/* ������������ַ */
#define FLASH_API_DATA_ADDR_END             0x08020000

/* FLASHҳ���С ����STM32F107�����Ͳ�Ʒ��FLASHҳ���СΪ2K����ֲʱ��ע�� */
#define FLASH_API_PAGE_SIZE                 1024

/* ��ȡ����ҳ���ַ */
#define FLASH_API_GET_PAGE_ADDR(addr)       (addr&0xFFFFFC00)
/* ��ȡ����ҳ��ƫ�Ƶ�ַ */
#define FLASH_API_GET_PAGE_OFFSET(addr)     (addr&0x000003ff)

extern ULONG FLASH_ApiReadData(ULONG ulFlashAddr, UCHAR *pucReadbuf, ULONG ulLen);
extern ULONG FLASH_ApiSaveData(ULONG ulFlashAddr, UCHAR *pucSavebuf, ULONG ulLen);

#endif /* __FLASH_API_H__ */

