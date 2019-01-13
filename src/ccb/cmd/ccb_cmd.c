 /******************************************************************************

              ��Ȩ���� (C)2010, ���������˳�����Ϣ�������޹�˾

 ******************************************************************************
  �� �� ��   : Cmd_Main.c
  �� �� ��   : ����
  ��    ��   : linhao
  ��������   : 2013��9��10��
  ����޸�   :
  ��������   : CMDģ�����ļ�
  �����б�   :
              
  �޸���ʷ   :
  1.��    ��   : 2013��9��10��
    ��    ��   : linhao
    �޸�����   : �����ļ�

******************************************************************************/
#include "stdio.h"
#include "stdlib.h"
#include "ccb_def.h"
#include "ccb_cmd.h"

CMD_CTRL_S g_stCmdCtrl;

/*****************************************************************************
 �� �� ��  : CMD_Init
 ��������  : CMDģ���ʼ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID CMD_Init()
{
    memset(&g_stCmdCtrl, 0, sizeof(g_stCmdCtrl));

	g_stCmdCtrl.stRootCmd.pFunOrNext = g_stCmdCtrl.astRootCmdGroup;
}

/*****************************************************************************
 �� �� ��  : CMD_Rx
 ��������  : CMDģ�����뺯��
 �������  : unsigned char ch  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
ULONG g_ulCmdOverFlow = 0;
ULONG g_ulCmdOverFlowCount = 0;
UCHAR g_aucCmdOverFlowContext[2];

VOID CMD_Rx(UCHAR ch)
{
    ULONG i; 

    if (g_ulCmdOverFlow)
    {
        g_aucCmdOverFlowContext[g_ulCmdOverFlowCount] = ch;
        g_ulCmdOverFlowCount++;

		//���⴦��
        if (CMD_KEY_FUN_LEN == g_ulCmdOverFlowCount)
        {
            g_ulCmdOverFlow = 0;
            g_ulCmdOverFlowCount = 0;

            if (CMD_KEY_UP == g_aucCmdOverFlowContext[0])
            {
                if (0 == g_stCmdCtrl.ucCmdLenOld)
                {
                    return;
                }
                
                for (i = 0; i< g_stCmdCtrl.ucCmdLen; i++)
                {
                    printf("\b\b  \b\b");
                }
				
                memcpy(g_stCmdCtrl.ucCmdBuf, g_stCmdCtrl.ucCmdBufOld, 64);
                g_stCmdCtrl.ucCmdLen = g_stCmdCtrl.ucCmdLenOld;
                g_stCmdCtrl.ucCmdBufOld[g_stCmdCtrl.ucCmdLenOld] = 0;
                printf("%s", g_stCmdCtrl.ucCmdBufOld);
            }
        }
        return;
    }
        
    /* ��� */
    if (ch == CMD_KEY_FUNC)
    {
        g_ulCmdOverFlow = 1;
        g_ulCmdOverFlowCount = 0;
		return;
    }

    /* �س� */
    else if (ch == 0x0D)
    {
        printf("\r");
        CMD_Proc();
    }

    /* �˸� */
    else if (ch == 0x08)
    {
        if (g_stCmdCtrl.ucCmdLen > 0)
        {
            g_stCmdCtrl.ucCmdLen--;
            printf("\b \b");
        }
    }

    /* ���� */
    else if (ch == '?')
    {
        if (g_stCmdCtrl.ucCmdLen < CMD_BUF_LEN)
        {
            g_stCmdCtrl.ucCmdBuf[g_stCmdCtrl.ucCmdLen] = ch;
            g_stCmdCtrl.ucCmdLen++;
            CMD_Proc();
        }
    }
    /* ֱ����ʾ */
    else if (ch >= 0x20)
    {
        if (g_stCmdCtrl.ucCmdLen < CMD_BUF_LEN)
        {
            printf("%c", ch);
            g_stCmdCtrl.ucCmdBuf[g_stCmdCtrl.ucCmdLen] = ch;
            g_stCmdCtrl.ucCmdLen++;
        }
    }
}

/*****************************************************************************
 �� �� ��  : CMD_GetCmdName
 ��������  : CMDģ���ȡ������
 �������  : UCHAR *pucBuf        
             UCHAR *ucCmdName     
             ULONG *ulCmdNameLen  
 �������  : ��
 �� �� ֵ  : UCHAR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID CMD_GetCmdName(UCHAR *ucCmdName, ULONG *ulCmdNameLen)
{
    UCHAR *pucPosTemp;
    ULONG ulLenTemp;

    pucPosTemp = g_stCmdCtrl.pucCurPos;
    ulLenTemp = 0;
    
    while (*pucPosTemp != ' ' && *pucPosTemp != 0)
    {
        ucCmdName[ulLenTemp] = *pucPosTemp;
        ulLenTemp++;
        pucPosTemp++;
    }

    ucCmdName[ulLenTemp] = 0;

    /* ȥ������ո� */
    while (*pucPosTemp==' ')
    {
        pucPosTemp++;
    }

    *ulCmdNameLen = ulLenTemp;
    g_stCmdCtrl.pucCurPos = pucPosTemp;
    
}

/*****************************************************************************
 �� �� ��  : CMD_Find
 ��������  : CMDģ���������
 �������  : UCHAR *pucCmdName  
             CMD_S *pstCurCmd   
 �������  : ��
 �� �� ֵ  : CMD_S
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
CMD_S * CMD_Find(UCHAR *pucCmdName, CMD_S *pstCurCmd)
{
    ULONG i;
    CMD_S *pstCmdSub;
    CMD_S *pstCmdFind = NULL;
    ULONG ulCmdNameLen1;
    ULONG ulCmdNameLen2;

    ulCmdNameLen1 = strlen((char *)pucCmdName);
    pstCmdSub = (CMD_S *)pstCurCmd->pFunOrNext;
    
    for (i = 0; i < pstCurCmd->ulNum; i++)
    {
        ulCmdNameLen2 = strlen((char *)pstCmdSub[i].ucCmd);

        if (ulCmdNameLen1 > ulCmdNameLen2)
        {
            continue;
        }
 
        if (0 == (memcmp(pucCmdName, pstCmdSub[i].ucCmd, ulCmdNameLen1)))
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

/*****************************************************************************
 �� �� ��  : CMD_Help
 ��������  : CMDģ����ʾ��������
 �������  : CMD_S *pCmd  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID CMD_Help(CMD_S *pCmd)
{
    CMD_S * pCmdSub;
    ULONG i;
    
    if (pCmd->ulNum == 0)
    {
         printf("\r\n %-20s  %s", pCmd->ucCmd, pCmd->ucDesc);
    }
    else
    {
        pCmdSub = (CMD_S *)pCmd->pFunOrNext;
        for (i = 0; i < pCmd->ulNum; i++)
        {
            printf("\r\n %-20s  %s", pCmdSub[i].ucCmd, pCmdSub[i].ucDesc);
        }
    }
}

ULONG CMD_Reg(CMD_S *pCmd)
{
    if (g_stCmdCtrl.ulCmdCount >= CMD_ROOT_CMD_NUM)
    {
		return COM_ERR;
    }

	memcpy(&g_stCmdCtrl.astRootCmdGroup[g_stCmdCtrl.ulCmdCount], pCmd, sizeof(CMD_S));
	g_stCmdCtrl.ulCmdCount++;
	g_stCmdCtrl.stRootCmd.ulNum = g_stCmdCtrl.ulCmdCount;
}

/*****************************************************************************
 �� �� ��  : CMD_Proc
 ��������  : CMDģ���������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��9��18��
    ��    ��   : linhao
    �޸�����   : �����ɺ���

*****************************************************************************/
VOID CMD_Proc()
{
    CMD_S * pCmd;
    UCHAR aucCmdName[CMD_NAME_LEN];
    ULONG ulCmdNameLen;
    CMD_FUN aa;
    ULONG ulCount = 0;
    ULONG i;

    g_stCmdCtrl.ucCmdBuf[g_stCmdCtrl.ucCmdLen] = 0;
    g_stCmdCtrl.pucCurPos = g_stCmdCtrl.ucCmdBuf;
    pCmd = &g_stCmdCtrl.stRootCmd;
                
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
                printf("\r\n �����ʽ����");
                CMD_Help(pCmd);
            }
            break;
        }

        pCmd = CMD_Find(aucCmdName, pCmd);
        if (NULL == pCmd)
        {
            printf("\r\n û���ҵ�������");
            break;
        }

        if (pCmd->ulNum != 0)
        {
            ulCount++;
            continue;
        }

        printf("\r\n");

        memcpy(g_stCmdCtrl.ucCmdBufOld, g_stCmdCtrl.ucCmdBuf, 64);
        g_stCmdCtrl.ucCmdLenOld = g_stCmdCtrl.ucCmdLen;
        aa = (CMD_FUN)(pCmd->pFunOrNext);
        aa();
        
        break;
    }
    printf("\r\nLNLH>");
    g_stCmdCtrl.ucCmdLen = 0;
    return;
}

#if DESC("��ò�������")
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

    CMD_ChangeBit(ucCmdName);

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
    CMD_GetCmdName(ucCmdName, &ulCmdNameLen);
    
    if (ulCmdNameLen != 14)
    {
        return COM_ERR;
    }

    if ((ucCmdName[4]!=':')|| (ucCmdName[9]!=':'))
    {
        return COM_ERR;
    }

    CMD_ChangeBit(ucCmdName);

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