#include "ccb_def.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "bsp.h"

#if DESC("������")
/*****************************************************************************
 �� �� ��  : main
 ��������  : ������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���5

*****************************************************************************/
int main(void)
{   
    ULONG i;
	ULONG ulRet;

    BSP_ApiDeLayMs(500);
        
	/* ��ʼ��BSP */
	BSP_ApiInit();
	
	/* ��ʼ��CMD */
	CMD_Init();

	/* ҵ���� */
	SRV_Main();
}

#endif

