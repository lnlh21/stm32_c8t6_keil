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
#include "bsp.h"
#include "drv_nrf24l01.h"

/* 地址都是 43 10 10 00结尾，第一字节区分功能 */
UCHAR g_aucTxAddr[DRV_NRF24L01_ADR_WIDTH]={0x00,0x43,0x10,0x10,0x00}; //发送地址
UCHAR g_aucBcAddr[DRV_NRF24L01_ADR_WIDTH]={0xff,0x43,0x10,0x10,0x00}; //接收地址
UCHAR g_aucMyAddr[DRV_NRF24L01_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x00}; //接收地址

DRV_NRF24L01_INFO_S g_st24L01;


#if DESC("寄出器操作")

	 
UCHAR DRV_NRF24L01_WriteReg(UCHAR ucReg, UCHAR ucValue)
{
	UCHAR ucStatus;	
    
    /* 片选 */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);
	
	/* 发送寄存器号 */
  	ucStatus =BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* 写入寄存器的值 */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucValue);

	/* 取消片选 */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_ReadReg(UCHAR ucReg)
{
	UCHAR ucValue;
	
    /* 片选 */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

	/* 发送寄存器号 */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);
  	
	/* 读取寄存器内容 */
  	ucValue = BSP_ApiSpiRead(BSP_SPI_24L01);

	/* 取消片选 */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucValue;
}	

UCHAR DRV_NRF24L01_ReadBuf(UCHAR reg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;	  

    /* 片选 */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

    /* 发送寄存器值(位置),并读取状态值 */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, reg);

	/* 读出数据 */
 	for(ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		pBuf[ucLoop] = BSP_ApiSpiRead(BSP_SPI_24L01);
 	}
 	
 	/* 取消片选 */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_WriteBuf(UCHAR ucReg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;
	
    /* 片选 */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

	/* 发送寄存器值(位置),并读取状态值 */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* 写入数据 */
  	for (ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		BSP_ApiSpiWrite(BSP_SPI_24L01, *pBuf++);
  	}
  	
	/* 取消片选 */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);

  	return ucStatus;
}				   
#endif

#if DESC("自检")
UCHAR DRV_NRF24L01_Check(VOID)
{
	UCHAR buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	UCHAR i;
	UCHAR ucCount = 0;

	/* 写入5个字节的地址 */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, buf, 5);

	/* 读出写入的地址 */
	DRV_NRF24L01_ReadBuf(DRV_NRF24L01_TX_ADDR, buf, 5);
	for (i = 0; i < 5; i++)
	{
		if (buf[i] != 0XA5)
		{
			ucCount++;
		}
	}

	if (ucCount)
	{
		return COM_ERR;
	}
	
	return COM_OK;
}	 

#endif

#if DESC("收发包")

void DRV_NRF24L01_RX_Mode(void)
{
	if (DRV_NRF_24L01_MODE_TX == g_st24L01.ucCurMode)
	{
	    BSP_ApiDeLayUs(100);
		
		/* 0通道关闭，通道1和通道2允许接收  */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

		/* 配置基本工作模式的参数 PWR_UP上电,EN_CRC使能,16BIT_CRC模式,接收模式 */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0f);

		/* CE为高,进入接收模式 */
		BSP_GpioSet(BSP_GPIO_2401_CE, 1);

		g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_RX;
	}
}		



UCHAR DRV_NRF24L01_RxPkt()
{
#if 0
	UCHAR ucStatus;

	__disable_irq();
	
	/* 如果没有中断 */
	if (0 != BSP_GpioRead(BSP_GPIO_2401_IRQ))
	{
		__enable_irq();
		return COM_ERR;
	}

	/* 读取状态寄存器的值 */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

	/* 清除TX_DS或MAX_RT中断标志 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
    
	/* 接收到数据 */
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_RX_OK)
	{
		/* 读取数据 */
		DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, rxbuf, DRV_NRF24L01_PLOAD_WIDTH);

		/* 清除RX FIFO寄存器 */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);
		return COM_OK; 
	}

	__enable_irq();
	
	return COM_ERR;//没收到任何数据
#endif 

	/* 读取数据 */
	DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, g_st24L01.g_ucRxBuf, DRV_NRF24L01_PLOAD_WIDTH);

	/* 清除RX FIFO寄存器 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);

	g_st24L01.pfnRxProc(g_st24L01.g_ucRxBuf);
	
	return COM_OK; 


}

VOID DRV_NRF24L01_TxMode()
{
    if (DRV_NRF_24L01_MODE_RX == g_st24L01.ucCurMode)
    {
		/* 使能通道0的接收地址 */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x01);

		/* 配置基本工作模式的参数;PWR_UP,EN_CRC,16BIT_CRC,接收模式,开启所有中断 */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0e);

		g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_TX;
    }
}

ULONG DRV_NRF24L01_TXPkt(UCHAR ucNodId, UCHAR* txbuf)
{			
	UCHAR ucStatus;
	UCHAR ucRet;
	
	__disable_irq();
	
    /* 拉低CE */
	BSP_GpioSet(BSP_GPIO_2401_CE, 0);

	DRV_NRF24L01_TxMode();
#if DRV_NRF24L01_CFG_EN_STATISTIC
	g_st24L01.g_usTxPktCnt++;
#endif

	if (g_st24L01.ucCurTxNode != ucNodId)
	{	
		if (0xff == ucNodId)
		{
			/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
	  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x00);
		}
		else
		{
			/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
	  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x1a);
		}

		/* 写TX节点地址 */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, ucNodId);

		/* 设置TX节点地址,主要为了使能ACK */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, ucNodId);
	}

	/* 写数据到TX BUF  32个字节 */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WR_TX_PLOAD, txbuf, DRV_NRF24L01_PLOAD_WIDTH);
	
	/* 启动发送 */
	BSP_GpioSet(BSP_GPIO_2401_CE, 1);

	/* 等待发送完成 */
	while(BSP_GpioRead(BSP_GPIO_2401_IRQ) != 0);

	/* 读取状态寄存器的值 */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

	/* 清除TX_DS或MAX_RT中断标志 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)//达到最大重发次数
	{
#if DRV_NRF24L01_CFG_EN_STATISTIC
	g_st24L01.g_usTxMaxRetryCnt++;
#endif
		/* 清除TX FIFO寄存器 */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);
		ucRet = ucStatus; 
	}
	else if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)//发送完成
	{
#if DRV_NRF24L01_CFG_EN_STATISTIC
	g_st24L01.g_usTxOkCnt++;
#endif
		ucRet = COM_OK;
	}
	else
	{
		ucRet = COM_ERR;
	}
	
	EXTI_ClearITPendingBit(EXTI_Line2);
	__enable_irq();
	
	return ucRet;
}	


VOID DRV_NRF24L01_Irq()
{
    UCHAR ucStatus;
	
    /* 处理直到外部回到高电平 */
	while (0 == BSP_GpioRead(BSP_GPIO_2401_IRQ))
	{
		/* 读取状态寄存器的值 */
		ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

		/* 处理中断 */
		if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)
		{
#if DRV_NRF24L01_CFG_EN_STATISTIC
			g_st24L01.g_usTxMaxRetryCnt++;
#endif
			/* 清除TX FIFO寄存器 */
			DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);

			/* 转为接收模式 */
			DRV_NRF24L01_RX_Mode();
		}

		if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)
		{
#if DRV_NRF24L01_CFG_EN_STATISTIC

			g_st24L01.g_usTxOkCnt++;
#endif
			/* 转为接收模式 */
			DRV_NRF24L01_RX_Mode();
		}

		if (ucStatus & DRV_NRF24L01_STATUS_BIT_RX_OK)
		{
#if DRV_NRF24L01_CFG_EN_STATISTIC
			g_st24L01.g_usRxOkCnt++;
#endif
			DRV_NRF24L01_RxPkt();
		}

		if (ucStatus & DRV_NRF24L01_STATUS_MASK)
		{
			/* 清除TX_DS或MAX_RT中断标志 */
			DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
		}
	}
}

#if DRV_NRF24L01_CFG_EN_DBG_CMD
VOID DRV_NRF24L01_Show()
{
	printf("\r\n Nrf Info");
	printf("\r\n TxPkt : %u", g_st24L01.g_usTxPktCnt);
	printf("\r\n MaxRetry : %u", g_st24L01.g_usTxMaxRetryCnt);
	printf("\r\n TxOk : %u", g_st24L01.g_usTxOkCnt);
	printf("\r\n RxOk : %u", g_st24L01.g_usRxOkCnt);

	g_st24L01.g_usTxPktCnt = 0;
	g_st24L01.g_usTxMaxRetryCnt = 0;
	g_st24L01.g_usTxOkCnt = 0;
	g_st24L01.g_usRxOkCnt = 0;
}

VOID DRV_NRF24L01_ShowReg()
{
	UCHAR ucValue[10];

	ucValue[0] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_CONFIG);
	ucValue[1] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_EN_AA);
	ucValue[2] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_EN_RXADDR);
	ucValue[3] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_SETUP_AW);
	ucValue[4] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_SETUP_RETR);
	ucValue[5] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_RF_CH);
	ucValue[6] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_RF_SETUP);
	ucValue[7] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);
	ucValue[8] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_OBSERVE_TX);
	ucValue[9] = DRV_NRF24L01_ReadReg(DRV_NRF24L01_CD);
	printf("\r\n config    [0x00] : %02x", ucValue[0]);
	printf("\r\n en_aa     [0x01] : %02x", ucValue[1]);
	printf("\r\n en_rxaddr [0x02] : %02x", ucValue[2]);
	printf("\r\n au        [0x03] : %02x", ucValue[3]);
	printf("\r\n retr      [0x04] : %02x", ucValue[4]);
	printf("\r\n ch        [0x05] : %02x", ucValue[5]);
	printf("\r\n rf setup  [0x06] : %02x", ucValue[6]);
	printf("\r\n status    [0x07] : %02x", ucValue[7]);
	printf("\r\n ob        [0x08] : %02x", ucValue[8]);
	printf("\r\n cd        [0x09] : %02x", ucValue[9]);
}

#endif 

VOID DRV_NRF24L01_Init(UCHAR ulMyNodeId, fnRegRxProc pfunc)
{
    
	UCHAR aucMyAddr[DRV_NRF24L01_ADR_WIDTH] = {0x03, 0x43, 0x10, 0x10, 0x00};

	/* 初始化软表 */
	g_st24L01.ucMyNode = ulMyNodeId;
	g_st24L01.ucCurTxNode = 0xff;
	g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_TX;
	g_st24L01.pfnRxProc = pfunc;
	
	aucMyAddr[0] = g_st24L01.ucMyNode;
	/* 通道0用于发送时接收ACK，地址在发送时设置 */
	/* 通道1用于接收单播数据，带自动回复 */
	/* 通道2用用于广播数据，无自动回复 */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
    DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P2, (UCHAR*)aucMyAddr, 1);

	/* 配置默认TX参数 */
	aucMyAddr[0] = g_st24L01.ucCurTxNode;
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR,    (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
	/* 设置自动重发间隔时间:500us + 86us;最大自动重发次数:10次 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x00);

	/* 选择通道1和2的有效数据宽度 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_AW, 0x03);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P0, DRV_NRF24L01_PLOAD_WIDTH);
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P1, DRV_NRF24L01_PLOAD_WIDTH);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P2, DRV_NRF24L01_PLOAD_WIDTH);

	/* 使能通道1的自动应答 注意:bit0必须置位，否则重传有问题 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_AA, 0x07);

	/* 设置RF通道为40 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_CH, 40);

	/* 设置TX发射参数,0db增益,2Mbps,低噪声增益开启 */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_SETUP, 0x0f);
	
    /* 开始接收模式 */
	DRV_NRF24L01_RX_Mode();
}




#endif



