/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_usart.c
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��12��17��
  ����޸�   :
  ��������   : ��������
  �����б�   :
              DRV_UsartInit
  �޸���ʷ   :
  1.��    ��   : 2013��12��17��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#include "ccb_def.h"
#include "stm32f10x.h"
#include "drv_usart.h"
#include <stdio.h>

ULONG ulPrintfDir = DRV_PRINT_UART;       


#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
    /* ������д�� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {
    }
  
    /* �������ݵ�USART2 */
    USART_SendData(USART1, (uint8_t) ch);

    return ch;
}

/*****************************************************************************
 �� �� ��  : DRV_UsartDbgPrint
 ��������  : ���Դ�ӡ��
 �������  : UCHAR *pucData  
             USHORT usLen    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID DRV_UsartDbgPrint(UCHAR *pucData, USHORT usLen)
{
    ULONG i;

    printf("\r\n Len = %u", usLen);
     
    for (i = 0; i < usLen; i++)
    {
        if (0 == (i%8))
        {
            printf("\r\n");
        }
        printf("%02x ", pucData[i]);
    }
}


