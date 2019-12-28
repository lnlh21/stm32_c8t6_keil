 /*
  文 件 名   : Cmd_Main.c
  作    者   : linhao
*/

#include "stdio.h"
#include "stdlib.h"
#include "ccb_def.h"
#include "ccb_cmd.h"

CmdCtrl g_cmdCtrl;
ULONG g_cmdOverFlow = 0;
ULONG g_cmdOverFlowCount = 0;
UCHAR g_cmdOverFlowContext[2];

/*
 功能描述  : CMD模块初始化
 */
VOID CMD_Init()
{
    memset(&g_cmdCtrl, 0, sizeof(g_cmdCtrl));
	g_cmdCtrl.rootCmd.sub = g_cmdCtrl.rootCmdG;
}

/*
 功能描述  : CMD模块输入函数
*/
VOID CMD_Rx(UCHAR ch)
{
    ULONG i; 

    if (g_cmdOverFlow)
    {
        g_cmdOverFlowContext[g_cmdOverFlowCount] = ch;
        g_cmdOverFlowCount++;

		//特殊处理
        if (CMD_KEY_FUN_LEN == g_cmdOverFlowCount)
        {
            g_cmdOverFlow = 0;
            g_cmdOverFlowCount = 0;

            if (CMD_KEY_UP == g_cmdOverFlowContext[0])
            {
                if (0 == g_cmdCtrl.cmdLenOld)
                {
                    return;
                }
                
                for (i = 0; i< g_cmdCtrl.cmdLen; i++)
                {
                    printf("\b\b  \b\b");
                }
				
                memcpy(g_cmdCtrl.cmdBuf, g_cmdCtrl.cmdBufOld, 64);
                g_cmdCtrl.cmdLen = g_cmdCtrl.cmdLenOld;
                g_cmdCtrl.cmdBufOld[g_cmdCtrl.cmdLenOld] = 0;
                printf("%s", g_cmdCtrl.cmdBufOld);
            }
        }
        return;
    }
        
    /* 溢出 */
    if (ch == CMD_KEY_FUNC)
    {
        g_cmdOverFlow = 1;
        g_cmdOverFlowCount = 0;
		return;
    }

    /* 回车 */
    else if (ch == 0x0D)
    {
        printf("\r");
        CMD_Proc();
    }

    /* 退格 */
    else if (ch == 0x08)
    {
        if (g_cmdCtrl.cmdLen > 0)
        {
            g_cmdCtrl.cmdLen--;
            printf("\b \b");
        }
    }

    /* 帮助 */
    else if (ch == '?')
    {
        if (g_cmdCtrl.cmdLen < CMD_BUF_LEN)
        {
            g_cmdCtrl.cmdBuf[g_cmdCtrl.cmdLen] = ch;
            g_cmdCtrl.cmdLen++;
            CMD_Proc();
        }
    }
    /* 直接显示 */
    else if (ch >= 0x20)
    {
        if (g_cmdCtrl.cmdLen < CMD_BUF_LEN)
        {
            printf("%c", ch);
            g_cmdCtrl.cmdBuf[g_cmdCtrl.cmdLen] = ch;
            g_cmdCtrl.cmdLen++;
        }
    }
}

/*
 功能描述  : CMD模块获取命令名
*/
VOID CMD_GetCmdName(UCHAR *ucCmdName, ULONG *ulCmdNameLen)
{
    UCHAR *pucPosTemp;
    ULONG ulLenTemp;

    pucPosTemp = g_cmdCtrl.curPos;
    ulLenTemp = 0;
    
    while (*pucPosTemp != ' ' && *pucPosTemp != 0)
    {
        ucCmdName[ulLenTemp] = *pucPosTemp;
        ulLenTemp++;
        pucPosTemp++;
    }

    ucCmdName[ulLenTemp] = 0;

    /* 去掉多余空格 */
    while (*pucPosTemp==' ')
    {
        pucPosTemp++;
    }

    *ulCmdNameLen = ulLenTemp;
    g_cmdCtrl.curPos = pucPosTemp;
    
}

/*
 功能描述  : CMD模块查找命令
 */
CmdNode * CMD_Find(UCHAR *pucCmdName, CmdNode *pstCurCmd)
{
    ULONG i;
    CmdNode *pstCmdSub;
    CmdNode *pstCmdFind = NULL;
    ULONG ulCmdNameLen1;
    ULONG ulCmdNameLen2;

    ulCmdNameLen1 = strlen((char *)pucCmdName);
    pstCmdSub = (CmdNode *)pstCurCmd->sub;
    
    for (i = 0; i < pstCurCmd->num; i++)
    {
        ulCmdNameLen2 = strlen((char *)pstCmdSub[i].cmd);

        if (ulCmdNameLen1 > ulCmdNameLen2)
        {
            continue;
        }
 
        if (0 == (memcmp(pucCmdName, pstCmdSub[i].cmd, ulCmdNameLen1)))
        {
            pstCmdFind = &pstCmdSub[i];

            if (ulCmdNameLen1 == ulCmdNameLen2)
            {
                return pstCmdFind;
            }
        }
    }

    return pstCmdFind;
}

/*
 功能描述  : CMD模块显示帮助命令
 */
VOID CMD_Help(CmdNode *pCmd)
{
    CmdNode * pCmdSub;
    ULONG i;
    
    if (pCmd->num == 0)
    {
         printf("\r\n %-20s  %s", pCmd->cmd, pCmd->desc);
    }
    else
    {
        pCmdSub = (CmdNode *)pCmd->sub;
        for (i = 0; i < pCmd->num; i++)
        {
            printf("\r\n %-20s  %s", pCmdSub[i].cmd, pCmdSub[i].desc);
        }
    }
}

ULONG CMD_Reg(CmdNode *pCmd)
{
    if (g_cmdCtrl.cmdCount >= CMD_ROOT_CMD_NUM)
    {
		return COM_ERR;
    }

	memcpy(&g_cmdCtrl.rootCmdG[g_cmdCtrl.cmdCount], pCmd, sizeof(CmdNode));
	g_cmdCtrl.cmdCount++;
	g_cmdCtrl.rootCmd.num = g_cmdCtrl.cmdCount;
}

/*
 功能描述  : CMD模块命令处理函数
 */
VOID CMD_Proc()
{
    CmdNode * pCmd;
    UCHAR aucCmdName[CMD_NAME_LEN];
    ULONG ulCmdNameLen;
    CMD_FUN aa;
    ULONG ulCount = 0;
    ULONG i;

    g_cmdCtrl.cmdBuf[g_cmdCtrl.cmdLen] = 0;
    g_cmdCtrl.curPos = g_cmdCtrl.cmdBuf;
    pCmd = &g_cmdCtrl.rootCmd;
                
    while (1)
    {
        CMD_GetCmdName(aucCmdName, &ulCmdNameLen);

        if (0 == (strcmp((char *)aucCmdName, "?")))
        {
            CMD_Help(pCmd);
            break;
        }
        
        if (0 == ulCmdNameLen)
        {
            if (0!=ulCount)
            {
                printf("\r\n cmd format error");
                CMD_Help(pCmd);
            }
            break;
        }

        pCmd = CMD_Find(aucCmdName, pCmd);
        if (NULL == pCmd)
        {
            printf("\r\n cmd not found");
            break;
        }

        if (pCmd->num != 0)
        {
            ulCount++;
            continue;
        }

        printf("\r\n");

        memcpy(g_cmdCtrl.cmdBufOld, g_cmdCtrl.cmdBuf, 64);
        g_cmdCtrl.cmdLenOld = g_cmdCtrl.cmdLen;
        aa = (CMD_FUN)(pCmd->sub);
        aa();
        
        break;
    }
    printf("\r\nLNLH>");
    g_cmdCtrl.cmdLen = 0;
    return;
}

#if DESC("获得参数部分")
ULONG CMD_GetULONG(ULONG *pulPara)
{
    ULONG ulCmdNameLen;
    UCHAR ucCmdName[CMD_NAME_LEN];
    ULONG ulJz = 10;
    UCHAR * pucPos = 0;
    ULONG ulTemp;
    
    CMD_GetCmdName(ucCmdName, &ulCmdNameLen);
    if (0 == ulCmdNameLen || ulCmdNameLen > 10)
    {
        return COM_ERR;
    }

    //CMD_ChangeBit(ucCmdName);

    pucPos = ucCmdName;
    if ('0' == ucCmdName[0] && 'X' == ucCmdName[1])
    {
        ulJz = 16;
        pucPos+=2;
    }

    while(*pucPos)
    {
        if ((*pucPos >= '0')
        && (*pucPos <= '9'))
        {
            ulTemp = ulTemp * ulJz + (*pucPos - '0');
        }
        else if ((*pucPos >= 'A')
        && (*pucPos <= 'F'))
        {
            ulTemp = ulTemp * ulJz + (*pucPos - 'A' + 10);
        }
        pucPos++;
    }
    *pulPara = ulTemp;
    return COM_OK;
}

ULONG CMD_GetIp(UCHAR *ucIp)
{
    ULONG ulCmdNameLen;
    UCHAR ucCmdName[CMD_NAME_LEN];
    UCHAR * pucPos = 0;
    ULONG i;
    ULONG ulIpPos;
    ULONG ulIpValue;
    CMD_GetCmdName(ucCmdName, &ulCmdNameLen);
    if (0 == ulCmdNameLen || ulCmdNameLen > 16)
    {
        return COM_ERR;
    }

    pucPos = ucCmdName;
    ulIpPos = 0;
    ulIpValue = 0;
    for (i = 0; i < ulCmdNameLen; i++)
    {
        if (ucCmdName[i] >='0' && ucCmdName[i] <= '9')
        {
            ulIpValue *= 10;
            ulIpValue += (ucCmdName[i] - '0');
            if (ulIpValue > 255)
            {
                return CMD_FORMAT_ERR;
            }
        }
        else if (ucCmdName[i] == '.')
        {
        }
        else
        {
            return CMD_FORMAT_ERR;
        }
        
        if (ucCmdName[i] == '.' || i == (ulCmdNameLen-1))
        {
            ucIp[ulIpPos] = ulIpValue;
            ulIpValue = 0;
            ulIpPos++;
            if (ulIpPos > 4)
            {
                return CMD_FORMAT_ERR;
            }        
        }
    }
   
    if (ulIpPos != 4)
    {
        return CMD_FORMAT_ERR;
    }
    
    return COM_OK;
}

ULONG CMD_GetMac(UCHAR *ucMac)
{
    ULONG ulCmdNameLen;
    UCHAR ucCmdName[CMD_NAME_LEN];
    UCHAR * pucPos = 0;
    ULONG i;
    ULONG ulMacPos;
    ULONG ulMACValue;
    //CMD_GetCmdName(ucCmdName, &ulCmdNameLen);
    
    if (ulCmdNameLen != 14)
    {
        return COM_ERR;
    }

    if ((ucCmdName[4]!=':')|| (ucCmdName[9]!=':'))
    {
        return COM_ERR;
    }

    //CMD_ChangeBit(ucCmdName);

    pucPos = ucCmdName;
    ulMacPos = 0;
    ulMACValue = 0;
    for (i = 0; i < ulCmdNameLen; i++)
    {
        if (ucCmdName[i] >='0' && ucCmdName[i] <= '9')
        {
            ulMACValue *= 16;
            ulMACValue += (ucCmdName[i] - '0');
            if (ulMACValue > 0xffff)
            {
                return CMD_FORMAT_ERR;
            }
        }
        else if (ucCmdName[i] >='A' && ucCmdName[i] <= 'F')
        {
            ulMACValue *= 16;
            ulMACValue += (ucCmdName[i] - 'A' + 10);
            if (ulMACValue > 0xffff)
            {
                return CMD_FORMAT_ERR;
            }
        }
        else if (ucCmdName[i] ==':')
        {
            
        }
        else
        {
            return CMD_FORMAT_ERR;
        }

        if (ucCmdName[i] == ':' || i == (ulCmdNameLen-1))
        {
            ucMac[ulMacPos*2] = ulMACValue >> 8;
            ucMac[ulMacPos*2 + 1] = ulMACValue;
            ulMACValue = 0;
            ulMacPos++;
            if (ulMacPos > 3)
            {
                return CMD_FORMAT_ERR;
            }        
        }
    }
   
    if (ulMacPos != 3)
    {
        return CMD_FORMAT_ERR;
    }
    
    return COM_OK;
}

ULONG CMD_GetString(UCHAR *pucString)
{
    ULONG ulCmdNameLen;
    UCHAR ucCmdName[CMD_NAME_LEN];
    UCHAR * pucPos = 0;
    ULONG i;
    ULONG ulMacPos;
    ULONG ulMACValue;
    
    CMD_GetCmdName(ucCmdName, &ulCmdNameLen);
    
    memcpy(pucString, ucCmdName, ulCmdNameLen);
    pucString[ulCmdNameLen] = 0;
    
    return COM_OK;
}

#endif