/******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : Cmd_Main.h
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��9��10��
  ����޸�   :
  ��������   : Cmd_Main.c ��ͷ�ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��9��10��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#ifndef __CMD_MAIN_H__
#define __CMD_MAIN_H__

#ifdef __cplusplus
	extern "C"{
#endif /* __cplusplus */

typedef VOID (*CMD_FUN)(VOID);

#define CMD_BUF_LEN  64
#define CMD_NAME_LEN 32
#define CMD_DESC_LEN 32
#define CMD_ROOT_CMD_NUM  16

typedef struct
{
    UCHAR ucCmd[CMD_NAME_LEN];
    UCHAR ucDesc[CMD_DESC_LEN];
    ULONG ulNum;
    VOID  *pFunOrNext;
}CMD_S;

typedef struct
{
    CMD_S stRootCmd;
    CMD_S astRootCmdGroup[CMD_ROOT_CMD_NUM];                      //�ܹ�֧��16��������ע��
    ULONG ulCmdCount;
    UCHAR ucCmdBufOld[CMD_BUF_LEN];
    UCHAR ucCmdBuf[CMD_BUF_LEN];
    UCHAR *pucCurPos;
    UCHAR ucCmdLen;
    UCHAR ucCmdLenOld;
}CMD_CTRL_S;

#define SUB_CMD(x) (sizeof(x)/sizeof(CMD_S)), x
#define CMD_FUN(x)  0, (VOID*)x


#ifdef WIN32
#define CMD_KEY_FUNC   0xe0
#define CMD_KEY_UP     0x48
#define CMD_KEY_DOWN   0x50
#define CMD_KEY_FUN_LEN  1
#else
#define CMD_KEY_FUNC   0x1B
#define CMD_KEY_UP     0x5B
#define CMD_KEY_FUN_LEN  2
#endif


enum
{
    CMD_OK           =0,
    CMD_ERR          =1,
    CMD_FORMAT_ERR  =2,
};

VOID CMD_Rx(UCHAR ch);
VOID CMD_GetCmdName(UCHAR *ucCmdName, ULONG *ulCmdNameLen);
VOID CMD_DisplayVersion(VOID);
VOID CMD_Proc();
ULONG CMD_GetULONG(ULONG *pulPara);
ULONG CMD_GetIp(UCHAR *ucIp);
ULONG CMD_Reg(CMD_S *pCmd);
ULONG PDT_Init();

#ifdef __cplusplus
	}
#endif /* __cplusplus */ 

#endif /* __CMD_MAIN_H__ */