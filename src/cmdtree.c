#include "ccb_def.h"
#include "ccb_cmd.h"
#include "stm32f10x.h"

extern void ShowMpu6050();
void CMD_FUN_ShowMsg(void)
{
    printf("\r\n CPU        : STM32");
    printf("\r\n Flash Size : %d K",
            *(__IO uint16_t*)(0x1FFFF7E0));

	printf("\r\n UID        : %08x-%08x-%08x",
            *(__IO uint32_t*)(0x1FFFF7E8),
            *(__IO uint32_t*)(0x1FFFF7Ec),
            *(__IO uint32_t*)(0x1FFFF7F0));

	SystemCoreClockUpdate();
    printf("\r\n Core Clock : %dHz",
            SystemCoreClock);
//	printf("\r\n CPU Usage  : %d%", OSCPUUsage);  

}

#if DESC("��ʾCPU����")
VOID CMD_ChangeBit(UCHAR * pucStr)
{
    while(*pucStr)
    {
        if ((*pucStr >= 'a')
            && (*pucStr <= 'z'))
        {
            *pucStr = *pucStr - ('a' - 'A');
        }
        pucStr++;
    }
}

VOID CMD_DisplayCpuMem(VOID)
{
    ULONG ulAddr;
    ULONG ulRet;
    ULONG ulLen;
    ULONG i;

    /* ��ȡ��ַ */
    ulRet = CMD_GetULONG(&ulAddr);
    if (COM_OK != ulRet)
    {
        printf("��������");
        return;
    }

    ulRet = CMD_GetULONG(&ulLen);
    if (COM_OK != ulRet)
    {
        printf("��������");
        return;
    }

    if (ulLen > 64)
    {
        ulLen = 64;
    }

    for (i = 0; i <= ulLen; i+=4)
    {
        printf("\r\n Addr(%08x) = (%08x)", (ulAddr+i), *(ULONG *)(ulAddr+i));
    }
}

/* display����ע�� */
CMD_S g_stCmdDisplayCpu[] = 
{
    {
     "mem",
     "��ѯCPU��ַ",
     CMD_FUN(CMD_DisplayCpuMem),
    },
};

#endif

#if DESC("��ʾ����")

VOID CMD_DisplayVersion(VOID)
{
    printf("\r\n Compile Time : "__DATE__ " - " __TIME__);
	printf("\r\n STDPERIPH_VERSION : %d.%d.%d",
            __STM32F10X_STDPERIPH_VERSION_MAIN,
            __STM32F10X_STDPERIPH_VERSION_SUB1,
            __STM32F10X_STDPERIPH_VERSION_SUB2);
}

/* display����ע�� */
CMD_S g_stCmdDisplayGroup[] = 
{
    {
     "version",
     "��ѯ�汾��",
     CMD_FUN(CMD_DisplayVersion),
    },
    {
     "sys",
     "��ѯϵͳ��Ϣ",
     CMD_FUN(CMD_FUN_ShowMsg),
    },
    {
     "cpu",
     "��ʾCPU����",
     SUB_CMD(g_stCmdDisplayCpu),
    }
};
#endif

#if DESC("������")
/* ������ע�� */
CMD_S g_stCmdDisplay[] = 
{
    {   
        "display",
        "��ʾ����",
        SUB_CMD(g_stCmdDisplayGroup),
    },
};

void exit();
VOID CMD_Exit(VOID)
{
    //exit();
}

CMD_S g_stCmdExit[] = 
{
    {   
        "exit",
        "�˳�ϵͳ",
        CMD_FUN(CMD_Exit),
    },
};

ULONG CMDTREE_Init()
{
	CMD_Reg(g_stCmdDisplay);
	CMD_Reg(g_stCmdExit);
}

#endif

