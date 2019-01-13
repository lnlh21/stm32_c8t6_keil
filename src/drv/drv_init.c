/******************************************************************************

              版权所有 (C)2010, 深圳市中兴长天信息技术有限公司

 ******************************************************************************
  文 件 名   : drv_init.c
  版 本 号   : 初稿
  作    者   : linhao
  生成日期   : 2013年12月18日
  最近修改   :
  功能描述   : 驱动初始化
  函数列表   :
              DRV_Init
  修改历史   :
  1.日    期   : 2013年12月18日
    作    者   : linhao
    修改内容   : 创建文件

******************************************************************************/
#include "ccb_def.h"
#include "stm32f10x.h"
#include "drv_init.h"

/*****************************************************************************
 函 数 名  : DRV_Init
 功能描述  : 驱动模块初始化
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月18日
    作    者   : linhao
    修改内容   : 新生成函数

*****************************************************************************/
VOID DRV_Init()
{
    RCC_ClocksTypeDef RCC_Clocks;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | 
		                   RCC_APB1Periph_I2C2 | RCC_AHBPeriph_DMA1,  ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                              RCC_APB2Periph_SPI1 | RCC_APB2Periph_USART1 |
                              RCC_APB2Periph_AFIO, ENABLE);

    /* TICK初始化 */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    RCC_GetClocksFreq(&RCC_Clocks);
    //SysTick_Config(RCC_Clocks.SYSCLK_Frequency / OS_TICKS_PER_SEC);
    
    /* 初始化灯 */
    //DRV_LedInit();
	//DRV_LedCtrl(0, 1);

	Timer2_init();

    /* 初始化按键 */
    //DRV_KeyInit();

    /* 初始化串口 */
    //DRV_UsartInit();

    /* 串口信息 */
    //CMD_FUN_ShowMsg();

    /* 初始化SPI */
    //DRV_SpiInit();

    /* 以太网初始化 */
    //ETH_ApiInit();

    /* 以太网中断配置 */
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);
    
    EXTI_InitStructure.EXTI_Line    = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    //EXTI_Init(&EXTI_InitStructure);

    /* PB6-I2C2_SCL、PB7-I2C2_SDA*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD; 
	//GPIO_Init(GPIOB, &GPIO_InitStructure); 

	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	//I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	//I2C_InitStructure.I2C_OwnAddress1 = 0xA0;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 50000;
	//I2C_Init(I2C2, &I2C_InitStructure);
	//I2C_Cmd(I2C2, ENABLE);
	/*允许应答模式*/
	//I2C_AcknowledgeConfig(I2C2, ENABLE);   
    
    /* 初始化中断 */
    NVIC_Configuration();

	//InitMPU6050();
}

