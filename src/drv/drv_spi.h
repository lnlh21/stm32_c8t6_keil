/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_spi.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��12��18��
  ����޸�   :
  ��������   : drv_spi.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_SPI_H__
#define __DRV_SPI_H__
/*   STM32F103 SPI�ӿڶ���
     PA4  ---  CS
     PA5  ---  SCK
     PA6  ---  MISO
     PA7  ---  MOSI
*/
#define DRV_SPI1_CS_LOW()   GPIO_ResetBits(GPIOA, GPIO_Pin_4)
#define DRV_SPI1_CS_HIGH()  GPIO_SetBits(GPIOA, GPIO_Pin_4)

extern void DRV_SpiInit();
extern ULONG DRV_SpiRecvMsg(UCHAR *pucData, USHORT *pusLen);
extern VOID DRV_SpiSendMsg(UCHAR *pucData, USHORT usLen);
extern unsigned char SPI1_SendByte(unsigned char byte);

#endif /* __DRV_SPI_H__ */

