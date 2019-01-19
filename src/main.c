#include "ccb_def.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "main.h"
#include "bsp.h"

#if DESC("主函数")
/*****************************************************************************
 函 数 名  : main
 功能描述  : 主函数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月18日
    作    者   : linhao
    修改内容   : 新生成函数5

*****************************************************************************/
int main(void)
{   
    ULONG i;
	ULONG ulRet;

    BSP_ApiDeLayMs(500);
        
	/* 初始化BSP */
	BSP_ApiInit();
	
	/* 初始化CMD */
	CMD_Init();

	/* 业务处理 */
	SRV_Main();
}

#endif

