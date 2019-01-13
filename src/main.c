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

    BSP_ApiDeLayMs(500);
        
	/* ��ʼ��BSP */
	BSP_ApiInit();

	/* ��ʼ��VOS */
	CMD_Init();
	CMDTREE_Init();

	while(1)
	{
		BSP_ApiLedCtrl(BSP_LED_0, 0);
		BSP_ApiDeLayMs(50);
		BSP_ApiLedCtrl(BSP_LED_0, 1);
		BSP_ApiDeLayMs(50);
	}
}

#endif

