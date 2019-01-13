/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : drv_init.c
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��12��18��
  ����޸�   :
  ��������   : ������ʼ��
  �����б�   :
              DRV_Init
  �޸���ʷ   :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#include "ccb_def.h"
#include "stm32f10x.h"
#include "bsp.h"
#include <stdio.h>


 int fputc(int ch, FILE *f)
 {
           USART_SendData(USART1, (uint8_t) ch);
         while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
         {}
         return ch;
 }



#if DESC("GPIO")
VOID BSP_GpioInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8 | GPIO_Pin_14| GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; 
    GPIO_Init(GPIOC, &GPIO_InitStructure);

	//GPIO_ResetBits(GPIOA, GPIO_Pin_3);
	//GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

VOID BSP_GpioSet(ULONG ulDevId, ULONG ulSw)
{
	if (BSP_GPIO_OUT_2401_CE == ulDevId)
	{
		if (ulSw)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_3);
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		}
	}
	else if (BSP_GPIO_OUT_2401_CSN == ulDevId)
	{
		if (ulSw)
		{
			GPIO_SetBits(GPIOA, GPIO_Pin_4);
		}
		else
		{
			GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		}
	}
}

ULONG BSP_GpioRead(ULONG ulDevId)
{
    if (BSP_GPIO_IN_2401_IRQ == ulDevId)
    {
		return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_2);
    }
}

#endif

#if DESC("CPU��ʱ")
/*****************************************************************************
 �� �� ��  : TIME_ApiDeLayMs
 ��������  : ��ʱ���������Ϊ���뼶������ȷ
 �������  : USHORT usTime  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��10��10��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID BSP_ApiDeLayMs(USHORT usTime)
{
    ULONG i;
    ULONG j;
    
    for (i = 0; i < usTime; i++)
    {
        for(j = 0; j < 12000; j++)
        {
            __asm ( "NOP" );
        }
    }
}

/*****************************************************************************
 �� �� ��  : TIME_ApiDeLayUs
 ��������  : ��ʱ���������Ϊ���뼶������ȷ
 �������  : USHORT usTime  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��10��10��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID BSP_ApiDeLayUs(USHORT usTime)
{
    ULONG i;
    ULONG j;
    
    for (i = 0; i < usTime; i++)
    {
        for(j = 0; j < 120; j++)
        {
            __asm ( "NOP" );
        }
    }
}

#endif

#if DESC("����")
void BSP_KeyInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
    
    /* key */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3 | GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING; 

    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    /*
         if(0 == GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_3))
        {
            if(w == 0)
            {
                w = 1;
            }
            else
            {
                w = 0;
            }
            printf("\r\n PIN 0");
            buf[0] = 0x06;
            buf[1] = w;
            //USB_SIL_Write(0x81, buf, 2);  
            //SetEPTxValid(1);
            
            for(i = 0; i < 500000; i++);
            {
                for(j = 0; j < 2000000; j++);
            }
            
        }
        
    }
    */
}

#endif

#if DESC("��ʱ��")
#if 0
void TIM3_DMA_Config(void)
{

        DMA_InitTypeDef DMA_InitStructure;

    	DMA_DeInit(DMA1_Channel2);

        DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)TIM2_CCR2_Address;     
        DMA_InitStructure.DMA_MemoryBaseAddr = (u32)LED_BYTE_Buffer;
        DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  
        DMA_InitStructure.DMA_BufferSize = 66;
        DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 
        DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable; 
        DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
        DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;  
        DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
        DMA_InitStructure.DMA_Priority = DMA_Priority_High;  
        DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
		
        DMA_Init(DMA1_Channel2, &DMA_InitStructure);       
        DMA_Cmd (DMA1_Channel2,ENABLE); 

        //TIM_DMACmd(TIM3, TIM_DMA_Update, ENABLE);
        //DMA_ITConfig(DMA1_Channel2, DMA_IT_TC, ENABLE);
}


void Timer2_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    /* GPIOA Configuration: TIM3 Channel 1 as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Compute the prescaler value */
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 29; // 800kHz 
    TIM_TimeBaseStructure.TIM_Prescaler = 2;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
 
    /* PWM1 Mode configuration: Channel3 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 0;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
    TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
    /* configure DMA */
    /* TIM3 CC1 DMA Request enable */
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
	TIM_ARRPreloadConfig(TIM2, ENABLE);
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
    TIM_DMACmd(TIM2, TIM_DMA_Update, ENABLE);

	TIM3_DMA_Config();
}
#endif

void Timer2_init(void)
{
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStruct;
	GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* ���ö�ʱ��������Ϣ */
    TIM_TimeBaseStructure.TIM_Period = 0xFFFF; // 800kHz 
    TIM_TimeBaseStructure.TIM_Prescaler = 35;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);

	/* ͨ��ѡ�� */
	TIM_ICInitStruct.TIM_Channel = TIM_Channel_2;
	/* �����ش��� */
	TIM_ICInitStruct.TIM_ICPolarity = TIM_ICPolarity_Rising;
    /* �ܽ���Ĵ�����Ӧ��ϵ */
	TIM_ICInitStruct.TIM_ICSelection = TIM_ICSelection_DirectTI;
	/* ����Ԥ��Ƶ */
	TIM_ICInitStruct.TIM_ICPrescaler = TIM_ICPSC_DIV8;
	/* �˲� */
	TIM_ICInitStruct.TIM_ICFilter = 0x0;
    /* ���� */
	TIM_PWMIConfig(TIM2, &TIM_ICInitStruct);

	
	/* IC2Ϊ����Դ */
	TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
	/* ���������¼��� */
	TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
	/* ������ʱ���ı������� */
	TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
	/* TIM2ʹ�� */
	TIM_Cmd(TIM2, ENABLE);
	/* ���ж� */
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE);
}


#endif

#if DESC("FLASH")

/*****************************************************************************
 �� �� ��  : FLASH_ApiSaveData
 ��������  : FLASHģ��д������
 �������  : ULONG ulFlashAddr  
             UCHAR *pucSavebuf  
             ULONG ulLen        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��17��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG FLASH_ApiSaveData(ULONG ulFlashAddr, UCHAR *pucSavebuf, ULONG ulLen)
{
    ULONG ulPageStart;
    ULONG ulPageEnd;
    ULONG ulAddr;
    ULONG ulLenTemp;
    
    /* ��ַ�Ϸ��Լ�� */
#if 0
    if ((FLASH_API_DATA_ADDR_START > ulFlashAddr)
        || (FLASH_API_DATA_ADDR_END < ulFlashAddr)
        || (FLASH_API_DATA_ADDR_END < (ulFlashAddr + ulLen)))
    {
        return COM_ERR;
    }
#endif

    __disable_irq();

    /* ����FLASH */
    FLASH_Unlock();
    
    /* ����Ҫ�����Ŀ� */
    ulPageStart = FLASH_API_GET_PAGE_ADDR(ulFlashAddr);
    ulPageEnd   = FLASH_API_GET_PAGE_ADDR(ulFlashAddr + ulLen);

    printf("%08x,%08x", ulPageStart, ulPageStart);
    /* ����FLASH */
    for (ulAddr = ulPageStart; ulAddr <= ulPageEnd; ulAddr+=FLASH_API_PAGE_SIZE)
    {
        printf("\r\n Flash E %08x", ulAddr);
        FLASH_ErasePage(ulAddr);
    }

    /* ���FLASH */
    ulAddr = ulFlashAddr;
    for (ulLenTemp = 0; ulLenTemp < ulLen; ulLenTemp+=2)
    {
        FLASH_ProgramHalfWord(ulAddr, *(USHORT*)(pucSavebuf + ulLenTemp));
        ulAddr+=2;
    }
    
    /* ��FLASH */
    FLASH_Lock();

    __enable_irq();

    return COM_OK;
}


/*****************************************************************************
 �� �� ��  : FLASH_ApiReadData
 ��������  : FLASHģ���ȡ����
 �������  : ULONG ulFlashAddr  
             UCHAR *pucReadbuf  
             ULONG ulLen        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��17��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG FLASH_ApiReadData(ULONG ulFlashAddr, UCHAR *pucReadbuf, ULONG ulLen)
{
#if 0
    /* ��ַ�Ϸ��Լ�� */
    if ((FLASH_API_DATA_ADDR_START > ulFlashAddr)
        || (FLASH_API_DATA_ADDR_END < ulFlashAddr)
        || (FLASH_API_DATA_ADDR_END < (ulFlashAddr + ulLen)))
    {
        return COM_ERR;
    }
#endif

    /* ����FLASH */
    FLASH_Unlock();

    /* ��ȡ */
    memcpy(pucReadbuf, (VOID *)ulFlashAddr, ulLen);

    /* ��FLASH */
    FLASH_Lock();

    return COM_OK;
}

#endif

#if DESC("�ж�")
ULONG BSP_InterruptInit()
{
	NVIC_InitTypeDef   NVIC_InitStructure;
    EXTI_InitTypeDef   EXTI_InitStructure;

	/* �����ж�������ַ */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
	
	/* �������ȼ� */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 

    /* �ⲿ�ж����� */
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource8);
    //GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource15);
	
    EXTI_InitStructure.EXTI_Line    = EXTI_Line8;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    EXTI_InitStructure.EXTI_Line    = EXTI_Line15;
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

	/* �����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* �����ж� */
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
#if 0
	/* Enable the EXTI8 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel					 = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel					 = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 4;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	/* Usart3 */
	NVIC_InitStructure.NVIC_IRQChannel					 = USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
   // NVIC_Init(&NVIC_InitStructure);	 

	/* Enable the EXTI8 Interrupt */
	NVIC_InitStructure.NVIC_IRQChannel					 = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority		 = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd				 = ENABLE;
	//NVIC_Init(&NVIC_InitStructure);
#endif 
}
#endif

#if DESC("���")

void BSP_LedInit()
{
    GPIO_InitTypeDef  GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin   = BSP_LED_PIN_1;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(BSP_LED_GPIO, &GPIO_InitStructure);
	BSP_LED_GPIO->BRR = BSP_LED_PIN_1;
#if (2 == BSP_LED_NUM)
	GPIO_InitStructure.GPIO_Pin   = BSP_LED_PIN_2;
	GPIO_Init(BSP_LED_GPIO, &GPIO_InitStructure);
	BSP_LED_GPIO->BRR = BSP_LED_PIN_2;
#endif
}

void BSP_ApiLedCtrl(ULONG ulNo, ULONG ulSw)
{

    if (ulNo == BSP_LED_0)
    {
    	if (ulSw == COM_ENABLE)
    	{
        	GPIO_ResetBits(BSP_LED_GPIO, BSP_LED_PIN_1);
    	}
		else
		{
			GPIO_SetBits(BSP_LED_GPIO, BSP_LED_PIN_1);
		}
    }
#if (2 == BSP_LED_NUM)
	if (ulNo == BSP_LED_1)
    {
    	if (ulSw == COM_ENABLE)
    	{
        	GPIO_ResetBits(BSP_LED_GPIO, BSP_LED_PIN_2);
    	}
		else
		{
			GPIO_SetBits(BSP_LED_GPIO, BSP_LED_PIN_2);
		}
    }
#endif
}

#endif

#if DESC("SPI")

/*   STM32F103 SPI�ӿڶ���
     PA4  ---  CS
     PA5  ---  SCK
     PA6  ---  MISO
     PA7  ---  MOSI
*/

ULONG BSP_SpiInit()
{
    SPI_InitTypeDef SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    /* SPI GPIO CONFIG */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 |  GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* SPI CONFIG */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);

    //SPI_I2S_ITConfig(SPI1, SPI_I2S_IT_TXE, ENABLE);
    SPI_Cmd(SPI1, ENABLE);

}

UCHAR BSP_ApiSpiRead(ULONG ulDevId)
{
    /* Wait for SPI1 Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI1, 0xff);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);

}

UCHAR BSP_ApiSpiWrite(ULONG ulDevId, UCHAR ucData)
{
    /* Wait for SPI1 Tx buffer empty */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);
    
    /* Send SPI1 data */
    SPI_I2S_SendData(SPI1, ucData);

    /* Wait to receive a byte */
    while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);

    /* Return the byte read from the SPI bus */
    return SPI_I2S_ReceiveData(SPI1);

}

#endif

#if DESC("IIC")

ULONG BSP_I2CInit()
{
 	GPIO_InitTypeDef  GPIO_InitStructure;
	I2C_InitTypeDef   I2C_InitStructure;
	
	/* Configure I2C2 pins: SCL and SDA ----------------------------------------*/
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
  
	/* Configure I2C2 */
	/* I2C DeInit */ 
	I2C_DeInit(I2C1);
	
	/* Set the I2C structure parameters */
	I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
	I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitStructure.I2C_OwnAddress1 = 0xFE;
	I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_InitStructure.I2C_ClockSpeed = 10000;
	
	/* Initialize the I2C peripheral w/ selected parameters */
	I2C_Init(I2C1, &I2C_InitStructure);
	
	/* Enable the I2C peripheral */
	I2C_Cmd(I2C1, ENABLE);
}

#endif

#if DESC("UART")




/*****************************************************************************
 �� �� ��  : DRV_UsartInit
 ��������  : ���ڳ�ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
void BSP_UartInit()
{
    USART_InitTypeDef USART_InitStructure;
    GPIO_InitTypeDef  GPIO_InitStructure;

    /* �������� */
    USART_InitStructure.USART_BaudRate = 115200;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

    /* ����1Tx Gpio */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /* ����1Rx Gpio */
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_Init(USART1, &USART_InitStructure);

    /* �������� */
    USART_Cmd(USART1, ENABLE);

    /* �����ж� */
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
}

void BSP_ApiUartPutc(UCHAR ch)
{
    /* ������д�� */
    while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
    {}
  
    /* ���� */
    USART_SendData(USART1, (uint8_t)ch);
}

#endif

#if DESC("��ʼ��")
/*****************************************************************************
 �� �� ��  : BSP_Init
 ��������  : BSP��ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID BSP_ApiInit()
{
    RCC_ClocksTypeDef RCC_Clocks;  
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
	I2C_InitTypeDef I2C_InitStructure;

	/* ��ʼ�� */
	SystemInit();

	/* ����Դģ��Ŀ��� */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3 | RCC_APB1Periph_TIM2 | 
		                   RCC_APB1Periph_I2C1 | RCC_APB1Periph_I2C2 | RCC_AHBPeriph_DMA1,  ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC |
                              RCC_APB2Periph_SPI1 | RCC_APB2Periph_USART1 |
                              RCC_APB2Periph_AFIO, ENABLE);


    /* TICK��ʼ�� */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
    RCC_GetClocksFreq(&RCC_Clocks);
    //SysTick_Config(RCC_Clocks.SYSCLK_Frequency / OS_TICKS_PER_SEC);

    /* ��ʼ���� */
    BSP_LedInit();
    
    /* ��ʼ������ */
    BSP_UartInit();

	/* ��ʼ��GPIO */
	BSP_GpioInit();

	Timer2_init();

	/* ��ʼ��SPI */
	//BSP_SpiInit();

	BSP_I2CInit();
	
    /* ��ʼ���ж� */
    BSP_InterruptInit();
}
#endif

