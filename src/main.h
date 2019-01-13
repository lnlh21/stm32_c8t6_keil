/******************************************************************************

                  ��Ȩ���� (C), 2001-2011, ���������������޹�˾

 ******************************************************************************
  �� �� ��   : main.h
  �� �� ��   : ����
  ��    ��   : lnlh21
  ��������   : 2014��8��31��
  ����޸�   :
  ��������   : main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2014��8��31��
    ��    ��   : lnlh21
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __MAIN_H__
#define __MAIN_H__

/*  MINI STM32 ����
    PA0 -- LED1
    PA1 -- LED2
    PA2 -- �ɱ����
    PA3 -- KEY1
    PA4 -- CS
    PA5 -- SCK
    PA6 -- MISO
    PA7 -- MOSI
    PA8 -- KEY2
    PA9 -- U1TX
    PA10 - U1RX
    PA11 - USBDM
    PA12 - USBDP
    PA13 - SWDIO/JTMS
    PA14 - SWDCLK/JTCK
    PA15 - JTDI

    PB0
    PB1
    PB2  - BOOT1    --ENC_RESET
    PB3  - JTDO
    PB4  - JNTRST
    PB5
    PB6  - SCL
    PB7  - SDA
    PB8
    PB9
    PB10
    PB11
    PB12
    PB13
    PB14
    PB15

    PC0
    PC1
    PC2
    PC3
    PC4
    PC5
    PC6
    PC7
    PC8
    PC9
    PC10
    PC11
    PC12
*/

/*  ENC28J60ģ��ͼ

               _________
    CLKOUT-----|1     6 |-----INT  (J1)
    WOL   -----|2     7 |-----MISO
    MOSI  -----|3     8 |-----SCK
    CS    -----|4     9 |-----RESET
    3.3   -----|5     10|-----GND
               ---------
*/


#define SYS_TAG                          2
#define SYS_VER                          1
#define SYS_DEFAULT_MAC                 {0x00, 0x22, 0x89, 0x43, 0x00, 0x01}
#define SYS_DEFAULT_IP                  {192, 168, 1, 250}
#define SYS_DEFAULT_MASK                {255, 255, 255, 0}
#define SYS_DEFAULT_GW                  {192, 168, 1, 1}
#define SYS_DEFAULT_TFTP_REMOTE_IP      {192, 168, 1, 200}
#define SYS_DEFAULT_TFTP_LOCAL_PORT     69
#define SYS_DEFAULT_TFTP_REMOTE_PORT    69
#define SYS_DEFAULT_HTTP_PORT           8089
#define SYS_DEFAULT_TELNET_PORT         23

#define START_MAGIC  0x5aa55aa5         /* �����ڴ�ļ���ħ���� */
#define APP_MAGIC  0x5a5a0aa5
#define LOAD_PARA_ADDR  0x20004F00   /* �ż��ز������ڴ��ַ */

#pragma pack(1)

typedef struct
{
    ULONG  ulTag;                   /* ��������ʶ */
    ULONG  ulVer;                   /* �汾�� */
    UCHAR  aucMacAddr[6];           /* MAC��ַ */ 
    UCHAR  aucIpAddr[4];            /* IP��ַ */
    UCHAR  aucMask[4];              /* ���� */
    UCHAR  aucGw[4];                /* ���� */
    UCHAR  ucRemoteTftpIp[4];       /* TFTPԶ�̵�ַ */
    USHORT usHttpPort;              /* HTTP�˿� */
    USHORT usTelnetPort;            /* TFTPԶ�̶˿� */
    USHORT usLocalTftpPort;         /* TFTP���ض˿� */
    USHORT usRemoteTftpPort;        /* TFTPԶ�̶˿� */
    UCHAR  aucTftpFile[32];
}SYS_INFO_S;


/* �ż��ز����Ľṹ */
typedef struct
{
    ULONG ulStartMagic;
    UCHAR ucMyMac[6];
    UCHAR ucMyIp[4];
    UCHAR ucMyMask[4];
    UCHAR ucMyGw[4];
    UCHAR ucTftpServerIp[4];
    USHORT usTftpClientPort;
    USHORT usTftpServerPort;
    UCHAR  ucTftpFile[32];
}APP_LOAD_PARA_S;

#pragma pack()

extern APP_LOAD_PARA_S *g_pstAppLoadPara;
extern SYS_INFO_S g_stSysInfo;

#endif /* __MAIN_H__ */


