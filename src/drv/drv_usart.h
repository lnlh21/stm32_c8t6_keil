/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_usart.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��12��17��
  ����޸�   :
  ��������   : drv_usart.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��12��17��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __DRV_USART_H__
#define __DRV_USART_H__

/* ����Printf����������� */
extern ULONG ulPrintfDir;

enum
{
    DRV_PRINT_UART,         /* �Ӵ������ */
    DRV_PRINT_NET,          /* ��������� */
    DRV_PRINT_BUTT
};


#define DRV_UART_SET_PRINT_UART()  (ulPrintfDir = DRV_PRINT_UART)
#define DRV_UART_SET_PRINT_NET()   (ulPrintfDir = DRV_PRINT_NET)

extern void DRV_UsartInit();


#endif /* __DRV_USART_H__ */



