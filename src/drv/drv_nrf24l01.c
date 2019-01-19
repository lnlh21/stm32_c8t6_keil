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
#include "drv_nrf24l01.h"

/* ��ַ���� 43 10 10 00��β����һ�ֽ����ֹ��� */
UCHAR g_aucTxAddr[DRV_NRF24L01_ADR_WIDTH]={0x00,0x43,0x10,0x10,0x00}; //���͵�ַ
UCHAR g_aucBcAddr[DRV_NRF24L01_ADR_WIDTH]={0xff,0x43,0x10,0x10,0x00}; //���յ�ַ
UCHAR g_aucMyAddr[DRV_NRF24L01_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x00}; //���յ�ַ

DRV_NRF24L01_INFO_S g_st24L01;


#if DESC("�ĳ�������")

	 
UCHAR DRV_NRF24L01_WriteReg(UCHAR ucReg, UCHAR ucValue)
{
	UCHAR ucStatus;	
    
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);
	
	/* ���ͼĴ����� */
  	ucStatus =BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* д��Ĵ�����ֵ */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucValue);

	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_ReadReg(UCHAR ucReg)
{
	UCHAR ucValue;
	
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

	/* ���ͼĴ����� */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);
  	
	/* ��ȡ�Ĵ������� */
  	ucValue = BSP_ApiSpiRead(BSP_SPI_24L01);

	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucValue;
}	

UCHAR DRV_NRF24L01_ReadBuf(UCHAR reg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;	  

    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

    /* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, reg);

	/* �������� */
 	for(ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		pBuf[ucLoop] = BSP_ApiSpiRead(BSP_SPI_24L01);
 	}
 	
 	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_WriteBuf(UCHAR ucReg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;
	
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_2401_CSN, 0);

	/* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* д������ */
  	for (ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		BSP_ApiSpiWrite(BSP_SPI_24L01, *pBuf++);
  	}
  	
	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_2401_CSN, 1);

  	return ucStatus;
}				   
#endif

#if DESC("�Լ�")
UCHAR DRV_NRF24L01_Check(VOID)
{
	UCHAR buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	UCHAR i;
	UCHAR ucCount = 0;

	/* д��5���ֽڵĵ�ַ */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, buf, 5);

	/* ����д��ĵ�ַ */
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

#if DESC("�շ���")

void DRV_NRF24L01_RX_Mode(void)
{
	if (DRV_NRF_24L01_MODE_TX == g_st24L01.ucCurMode)
	{
	    BSP_ApiDeLayUs(100);
		
		/* 0ͨ���رգ�ͨ��1��ͨ��2�������  */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

		/* ���û�������ģʽ�Ĳ��� PWR_UP�ϵ�,EN_CRCʹ��,16BIT_CRCģʽ,����ģʽ */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0f);

		/* CEΪ��,�������ģʽ */
		BSP_GpioSet(BSP_GPIO_2401_CE, 1);

		g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_RX;
	}
}		



UCHAR DRV_NRF24L01_RxPkt()
{
#if 0
	UCHAR ucStatus;

	__disable_irq();
	
	/* ���û���ж� */
	if (0 != BSP_GpioRead(BSP_GPIO_2401_IRQ))
	{
		__enable_irq();
		return COM_ERR;
	}

	/* ��ȡ״̬�Ĵ�����ֵ */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

	/* ���TX_DS��MAX_RT�жϱ�־ */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
    
	/* ���յ����� */
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_RX_OK)
	{
		/* ��ȡ���� */
		DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, rxbuf, DRV_NRF24L01_PLOAD_WIDTH);

		/* ���RX FIFO�Ĵ��� */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);
		return COM_OK; 
	}

	__enable_irq();
	
	return COM_ERR;//û�յ��κ�����
#endif 

	/* ��ȡ���� */
	DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, g_st24L01.g_ucRxBuf, DRV_NRF24L01_PLOAD_WIDTH);

	/* ���RX FIFO�Ĵ��� */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);

	g_st24L01.pfnRxProc(g_st24L01.g_ucRxBuf);
	
	return COM_OK; 


}

VOID DRV_NRF24L01_TxMode()
{
    if (DRV_NRF_24L01_MODE_RX == g_st24L01.ucCurMode)
    {
		/* ʹ��ͨ��0�Ľ��յ�ַ */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x01);

		/* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж� */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0e);

		g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_TX;
    }
}

ULONG DRV_NRF24L01_TXPkt(UCHAR ucNodId, UCHAR* txbuf)
{			
	UCHAR ucStatus;
	UCHAR ucRet;
	
	__disable_irq();
	
    /* ����CE */
	BSP_GpioSet(BSP_GPIO_2401_CE, 0);

	DRV_NRF24L01_TxMode();
#if DRV_NRF24L01_CFG_EN_STATISTIC
	g_st24L01.g_usTxPktCnt++;
#endif

	if (g_st24L01.ucCurTxNode != ucNodId)
	{	
		if (0xff == ucNodId)
		{
			/* �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� */
	  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x00);
		}
		else
		{
			/* �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� */
	  		DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x1a);
		}

		/* дTX�ڵ��ַ */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, ucNodId);

		/* ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK */
	  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, ucNodId);
	}

	/* д���ݵ�TX BUF  32���ֽ� */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WR_TX_PLOAD, txbuf, DRV_NRF24L01_PLOAD_WIDTH);
	
	/* �������� */
	BSP_GpioSet(BSP_GPIO_2401_CE, 1);

	/* �ȴ�������� */
	while(BSP_GpioRead(BSP_GPIO_2401_IRQ) != 0);

	/* ��ȡ״̬�Ĵ�����ֵ */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

	/* ���TX_DS��MAX_RT�жϱ�־ */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)//�ﵽ����ط�����
	{
#if DRV_NRF24L01_CFG_EN_STATISTIC
	g_st24L01.g_usTxMaxRetryCnt++;
#endif
		/* ���TX FIFO�Ĵ��� */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);
		ucRet = ucStatus; 
	}
	else if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)//�������
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
	
    /* ����ֱ���ⲿ�ص��ߵ�ƽ */
	while (0 == BSP_GpioRead(BSP_GPIO_2401_IRQ))
	{
		/* ��ȡ״̬�Ĵ�����ֵ */
		ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);

		/* �����ж� */
		if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)
		{
#if DRV_NRF24L01_CFG_EN_STATISTIC
			g_st24L01.g_usTxMaxRetryCnt++;
#endif
			/* ���TX FIFO�Ĵ��� */
			DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);

			/* תΪ����ģʽ */
			DRV_NRF24L01_RX_Mode();
		}

		if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)
		{
#if DRV_NRF24L01_CFG_EN_STATISTIC

			g_st24L01.g_usTxOkCnt++;
#endif
			/* תΪ����ģʽ */
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
			/* ���TX_DS��MAX_RT�жϱ�־ */
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

	/* ��ʼ����� */
	g_st24L01.ucMyNode = ulMyNodeId;
	g_st24L01.ucCurTxNode = 0xff;
	g_st24L01.ucCurMode = DRV_NRF_24L01_MODE_TX;
	g_st24L01.pfnRxProc = pfunc;
	
	aucMyAddr[0] = g_st24L01.ucMyNode;
	/* ͨ��0���ڷ���ʱ����ACK����ַ�ڷ���ʱ���� */
	/* ͨ��1���ڽ��յ������ݣ����Զ��ظ� */
	/* ͨ��2�����ڹ㲥���ݣ����Զ��ظ� */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
    DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P2, (UCHAR*)aucMyAddr, 1);

	/* ����Ĭ��TX���� */
	aucMyAddr[0] = g_st24L01.ucCurTxNode;
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR,    (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)aucMyAddr, DRV_NRF24L01_ADR_WIDTH);
	/* �����Զ��ط����ʱ��:500us + 86us;����Զ��ط�����:10�� */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_RETR, 0x00);

	/* ѡ��ͨ��1��2����Ч���ݿ�� */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_SETUP_AW, 0x03);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P0, DRV_NRF24L01_PLOAD_WIDTH);
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P1, DRV_NRF24L01_PLOAD_WIDTH);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P2, DRV_NRF24L01_PLOAD_WIDTH);

	/* ʹ��ͨ��1���Զ�Ӧ�� ע��:bit0������λ�������ش������� */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_AA, 0x07);

	/* ����RFͨ��Ϊ40 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_CH, 40);

	/* ����TX�������,0db����,2Mbps,���������濪�� */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_SETUP, 0x0f);
	
    /* ��ʼ����ģʽ */
	DRV_NRF24L01_RX_Mode();
}




#endif



