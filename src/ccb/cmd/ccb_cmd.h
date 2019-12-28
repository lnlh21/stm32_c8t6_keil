#ifndef __CCB_CMD_H__
#define __CCB_CMD_H__

typedef VOID (*CMD_FUN)(VOID);

#define CMD_BUF_LEN  64
#define CMD_NAME_LEN 32
#define CMD_DESC_LEN 32
#define CMD_ROOT_CMD_NUM  16

typedef struct
{
    UCHAR cmd[CMD_NAME_LEN];
    UCHAR desc[CMD_DESC_LEN];
    ULONG num;
    VOID* sub;
} CmdNode;

typedef struct
{
    CmdNode rootCmd;
    CmdNode rootCmdG[CMD_ROOT_CMD_NUM];                      //总共支持16条根命令注册
    ULONG cmdCount;
    UCHAR cmdBufOld[CMD_BUF_LEN];
    UCHAR cmdBuf[CMD_BUF_LEN];
    UCHAR *curPos;
    UCHAR cmdLen;
    UCHAR cmdLenOld;
} CmdCtrl;

#define SUB_CMD(x) (sizeof(x)/sizeof(CmdNode)), x
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
ULONG CMD_Reg(CmdNode *pCmd);
ULONG PDT_Init();

#endif