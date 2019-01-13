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
#include "vos.h"
#include "stm32f10x.h"
#include "bsp.h"
#include "drv_nrf24l01.h"

#if BSP_24L01_SW 

UCHAR g_aucTxAddr[DRV_NRF24L01_TX_ADR_WIDTH]={0x00,0x43,0x10,0x10,0x00}; //���͵�ַ
const UCHAR g_aucBcAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0xff,0x43,0x10,0x10,0x00}; //���յ�ַ

#if (BSP_BOARD_SOFT_TYPE == BSP_BOARD_SOFT_TYPE_MAIN)
const UCHAR BSP_BOARD_SOFT_TYPE[DRV_NRF24L01_RX_ADR_WIDTH]={0xfe,0x43,0x10,0x10,0x00}; //���յ�ַ
#elif (BSP_BOARD_SOFT_TYPE == BSP_BOARD_SOFT_TYPE_SUB1)
const UCHAR g_aucMyAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0x01,0x43,0x10,0x10,0x00}; //���յ�ַ
#elif (BSP_BOARD_SOFT_TYPE == BSP_BOARD_SOFT_TYPE_SUB2)
const UCHAR g_aucMyAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0x02,0x43,0x10,0x10,0x00}; //���յ�ַ
#elif (BSP_BOARD_SOFT_TYPE == BSP_BOARD_SOFT_TYPE_SUB3)
const UCHAR g_aucMyAddr[DRV_NRF24L01_RX_ADR_WIDTH]={0x03,0x43,0x10,0x10,0x00}; //���յ�ַ
#endif 


#if DESC("�ĳ�������")

	 
UCHAR DRV_NRF24L01_WriteReg(UCHAR ucReg, UCHAR ucValue)
{
	UCHAR ucStatus;	
    
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 0);
	
	/* ���ͼĴ����� */
  	ucStatus =BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* д��Ĵ�����ֵ */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucValue);

	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_ReadReg(UCHAR ucReg)
{
	UCHAR ucValue;
	
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 0);

	/* ���ͼĴ����� */
  	BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);
  	
	/* ��ȡ�Ĵ������� */
  	ucValue = BSP_ApiSpiRead(BSP_SPI_24L01);

	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 1);
	
  	return ucValue;
}	

UCHAR DRV_NRF24L01_ReadBuf(UCHAR reg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;	  

    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 0);

    /* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, reg);

	/* �������� */
 	for(ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		pBuf[ucLoop] = BSP_ApiSpiRead(BSP_SPI_24L01);
 	}
 	
 	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 1);
	
  	return ucStatus;
}

UCHAR DRV_NRF24L01_WriteBuf(UCHAR ucReg, UCHAR *pBuf, UCHAR ucLen)
{
	UCHAR ucStatus;
	UCHAR ucLoop;
	
    /* Ƭѡ */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 0);

	/* ���ͼĴ���ֵ(λ��),����ȡ״ֵ̬ */
  	ucStatus = BSP_ApiSpiWrite(BSP_SPI_24L01, ucReg);

	/* д������ */
  	for (ucLoop = 0; ucLoop < ucLen; ucLoop++)
	{
		BSP_ApiSpiWrite(BSP_SPI_24L01, *pBuf++);
  	}
  	
	/* ȡ��Ƭѡ */
  	BSP_GpioSet(BSP_GPIO_OUT_2401_CSN, 1);

  	return ucStatus;
}				   
#endif

#if DESC("�Լ�")
UCHAR DRV_NRF24L01_Check(VOID)
{
	UCHAR buf[5] = {0XA5, 0XA5, 0XA5, 0XA5, 0XA5};
	UCHAR i;

	/* д��5���ֽڵĵ�ַ */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, buf, 5);

	/* ����д��ĵ�ַ */
	DRV_NRF24L01_ReadBuf(DRV_NRF24L01_TX_ADDR, buf, 5);
	for (i = 0; i < 5; i++)
	{
		if(buf[i] != 0XA5)
		{
			break;
		}
	}

	if (i != 5)
	{
		return COM_ERR;
	}
	
	return COM_OK;
}	 

#endif

#if DESC("�շ���")
UCHAR DRV_NRF24L01_TxPkt(UCHAR *txbuf)
{
	UCHAR ucStatus;
 
 	/* ����CE */
 	BSP_GpioSet(BSP_GPIO_OUT_2401_CE, 0);

	/* д���ݵ�TX BUF  32���ֽ� */
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WR_TX_PLOAD, txbuf, DRV_NRF24L01_TX_PLOAD_WIDTH);

 	/* ��������	*/
 	BSP_GpioSet(BSP_GPIO_OUT_2401_CE, 1);

	/* �ȴ�������� */
	while(BSP_GpioRead(BSP_GPIO_IN_2401_IRQ) != 0);
	
	/* ��ȡ״̬�Ĵ�����ֵ */
	ucStatus = DRV_NRF24L01_ReadReg(DRV_NRF24L01_STATUS);
	
	/* ���TX_DS��MAX_RT�жϱ�־ */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_STATUS, ucStatus);

	if (ucStatus & DRV_NRF24L01_STATUS_BIT_MAX_TX)//�ﵽ����ط�����
	{
		/* ���TX FIFO�Ĵ��� */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_TX, 0xff);

		return ucStatus; 
	}
	if (ucStatus & DRV_NRF24L01_STATUS_BIT_TX_OK)//�������
	{
		return COM_OK;
	}
	
	return COM_ERR;
}

//����NRF24L01����һ������
//txbuf:�����������׵�ַ
//����ֵ:0��������ɣ��������������
UCHAR DRV_NRF24L01_RxPkt(UCHAR *rxbuf)
{
	UCHAR ucStatus;

	/* ���û���ж� */
	if (0 != BSP_GpioRead(BSP_GPIO_IN_2401_IRQ))
	{
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
		DRV_NRF24L01_ReadBuf(DRV_NRF24L01_RD_RX_PLOAD, rxbuf, DRV_NRF24L01_RX_PLOAD_WIDTH);

		/* ���RX FIFO�Ĵ��� */
		DRV_NRF24L01_WriteReg(DRV_NRF24L01_FLUSH_RX, 0xff);
		return COM_OK; 
	}

	return COM_ERR;//û�յ��κ�����
}

//�ú�����ʼ��NRF24L01��RXģʽ
//����RX��ַ,дRX���ݿ��,ѡ��RFƵ��,�����ʺ�LNA HCURR
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
void DRV_NRF24L01_RX_Mode(void)
{
    /* ����CE */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CE, 0);

	/* ʹ��ͨ��1�Ľ��յ�ַ */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

	/* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0f);

	/* CEΪ��,�������ģʽ */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CE, 1);
}		

//�ú�����ʼ��NRF24L01��TXģʽ
//����TX��ַ,дTX���ݿ��,����RX�Զ�Ӧ��ĵ�ַ,���TX��������,ѡ��RFƵ��,�����ʺ�LNA HCURR
//PWR_UP,CRCʹ��
//��CE��ߺ�,������RXģʽ,�����Խ���������		   
//CEΪ�ߴ���10us,����������.	 
VOID DRV_NRF24L01_TX_Mode(UCHAR ucNodId)
{														 
    /* ����CE */
	BSP_GpioSet(BSP_GPIO_OUT_2401_CE, 0);

	g_aucTxAddr[0] = ucNodId;

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
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_TX_ADDR, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK */
  	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* ʹ��ͨ��0�Ľ��յ�ַ */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x01);

	/* ���û�������ģʽ�Ĳ���;PWR_UP,EN_CRC,16BIT_CRC,����ģʽ,���������ж� */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_CONFIG, 0x0e);
}	

VOID DRV_NRF24L01_Init(VOID)
{
	/* ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK */
  	//DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P0, (UCHAR*)g_aucTxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* ����TX�ڵ��ַ,��ҪΪ��ʹ��ACK */
	//DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)g_aucRxAddr, DRV_NRF24L01_TX_ADR_WIDTH);

	/* ѡ��ͨ��0����Ч���ݿ�� */
  	//DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P0, DRV_NRF24L01_RX_PLOAD_WIDTH);

    /* ͨ��0���ڷ���ʱ����ACK����ַ�ڷ���ʱ���� */
	/* ͨ��1���ڽ��յ������ݣ����Զ��ظ� */
	/* ͨ��2yo���ڹ㲥���ݣ����Զ��ظ� */
	DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P1, (UCHAR*)g_aucMyAddr, DRV_NRF24L01_TX_ADR_WIDTH);
    DRV_NRF24L01_WriteBuf(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_ADDR_P2, (UCHAR*)g_aucBcAddr, 1);

	/* ѡ��ͨ��1��2����Ч���ݿ�� */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P1, DRV_NRF24L01_RX_PLOAD_WIDTH);
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RX_PW_P2, DRV_NRF24L01_RX_PLOAD_WIDTH);

	/* ʹ��ͨ��0��1���Զ�Ӧ�� */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_AA, 0x07);

	/* ʹ��ͨ��2��3�Ľ��յ�ַ */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_EN_RXADDR, 0x06);

	/* ����RFͨ��Ϊ40 */
  	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_CH, 40);

	/* ����TX�������,0db����,2Mbps,���������濪�� */
	DRV_NRF24L01_WriteReg(DRV_NRF24L01_WRITE_REG | DRV_NRF24L01_RF_SETUP, 0x0f);

}

#endif

#endif



