#include "SGUI_VariableBox.h"

static void SGUI_TextVariableBox_UpdateFocusCharIndex(SGUI_TEXT_VARBOX_STRUCT* pstObj);

void SGUI_NumberVariableBox_Initialize(SGUI_NUM_VARBOX_STRUCT* pstObj, const SGUI_NUM_VARBOX_PARAM* pcstInitParam)
{
    if((NULL != pstObj) && (NULL != pcstInitParam))
    {
        SGUI_SystemIF_MemorySet(pstObj, 0x00, sizeof(SGUI_NUM_VARBOX_STRUCT));
        SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_NUM_VARBOX_PARAM));
    }
}

void SGUI_NumberVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_NUM_VARBOX_STRUCT* pstObj, SGUI_DRAW_MODE eMode)
{
    SGUI_AREA_SIZE              stTextExtentSize;
    SGUI_POINT                  stTextInnerPos;
    SGUI_COLOR                  eBackColor;
    SGUI_CHAR                   szTextBuffer[VARBOX_TEXT_BUFFER_SIZE];

    SGUI_SystemIF_MemorySet(szTextBuffer, 0x00, VARBOX_TEXT_BUFFER_SIZE);
    eBackColor =                ((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);

    if(NULL != pstObj)
    {
        // Draw edge
        SGUI_Basic_DrawRectangle(pstDeviceIF, LAYOUT(pstObj).iX, LAYOUT(pstObj).iY, LAYOUT(pstObj).iWidth, LAYOUT(pstObj).iHeight, eBackColor, eBackColor);

        // Convert number to string
        (void)SGUI_Common_IntegerToString(pstObj->stData.iValue, szTextBuffer, 10, -1, ' ');
        SGUI_Text_GetTextExtent(szTextBuffer, pstObj->stParam.pstFontRes, &stTextExtentSize);
        switch(pstObj->stParam.eAlignment)
        {
            case SGUI_RIGHT:
            {
                stTextInnerPos.iX = LAYOUT(pstObj).iWidth - stTextExtentSize.iWidth;
                break;
            }
            case SGUI_CENTER:
            {
                stTextInnerPos.iX = (LAYOUT(pstObj).iWidth - stTextExtentSize.iWidth) / 2;
                break;
            }
            default:
            {
                stTextInnerPos.iX = 0;
            }
        }
        stTextInnerPos.iY = 0;
        SGUI_Text_DrawText(pstDeviceIF, szTextBuffer, pstObj->stParam.pstFontRes, &(LAYOUT(pstObj)), &stTextInnerPos, eMode);
    }
}

void SGUI_TextVariableBox_Initialize(SGUI_TEXT_VARBOX_STRUCT* pstObj, const SGUI_TEXT_VARBOX_PARAM* pcstInitParam, SGUI_SZSTR szTextBuffer)
{
    if((NULL != pstObj) && (NULL != pcstInitParam))
    {
        SGUI_SystemIF_MemorySet(pstObj, 0x00, sizeof(SGUI_TEXT_VARBOX_STRUCT));
        SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_TEXT_VARBOX_PARAM));
        pstObj->stData.szValue = szTextBuffer;
        pstObj->stData.iVisibleCharNum = (pstObj->stParam.stLayout.iWidth - 1)/pstObj->stParam.pstFontRes->iHalfWidth + 1;
        pstObj->stData.iFirstVisibleIndex = 0;
		pstObj->stData.iLastVisibleIndex = pstObj->stData.iVisibleCharNum - 1;
		if(pstObj->stData.iLastVisibleIndex >= pstObj->stParam.iTextLength)
		{
			pstObj->stData.iLastVisibleIndex = pstObj->stParam.iTextLength - 1;
		}
        pstObj->stData.iFocusCharIndex = -1;
        for(SGUI_INT iCharIdx=0; iCharIdx<pstObj->stParam.stCharSet.iSize; iCharIdx++)
        {
            if(pstObj->stData.szValue[0] == pstObj->stParam.stCharSet.cszCharSet[iCharIdx])
            {
                pstObj->stData.iFocusCharIndex = iCharIdx;
                break;
            }
        }
		pstObj->stData.iOffset = 0;
    }
}

static void SGUI_TextVariableBox_UpdateFocusCharIndex(SGUI_TEXT_VARBOX_STRUCT* pstObj)
{
    pstObj->stData.iFocusCharIndex = VARBOX_TEXT_INVALID_CHAR_IDX;
    for(SGUI_INT iCharIdx=0; iCharIdx<pstObj->stParam.stCharSet.iSize; iCharIdx++)
    {
        if(pstObj->stParam.stCharSet.cszCharSet[iCharIdx] == pstObj->stData.szValue[pstObj->stData.iFocusIndex])
        {
            pstObj->stData.iFocusCharIndex = iCharIdx;
            break;
        }
    }
}

void SGUI_TextVariableBox_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_TEXT_VARBOX_STRUCT* pstObj, SGUI_CHAR cMask, SGUI_DRAW_MODE eMode)
{
    SGUI_COLOR              eBackColor = ((eMode==SGUI_DRAW_NORMAL)?SGUI_COLOR_BKGCLR:SGUI_COLOR_FRGCLR);
    SGUI_POINT              stCharacterPos;
    SGUI_INT				iCharIdx;
    SGUI_CHAR				cPaintChar;

    if((NULL != pstObj) && (NULL != pstObj->stData.szValue))
    {
        // Clear background.
        SGUI_Basic_DrawRectangle(pstDeviceIF, LAYOUT(pstObj).iX, LAYOUT(pstObj).iY, LAYOUT(pstObj).iWidth, LAYOUT(pstObj).iHeight, eBackColor, eBackColor);

        if(pstObj->stData.iFocusIndex >= pstObj->stParam.iTextLength)
		{
			/* Focus character index cannot greater then ending index. */
			pstObj->stData.iFocusIndex = pstObj->stParam.iTextLength - 1;
		}
		else if(pstObj->stData.iFocusIndex < 0)
		{
			pstObj->stData.iFocusIndex = 0;
		}
		/* Update first & last visible character index. */
		if(pstObj->stData.iFocusIndex > pstObj->stData.iLastVisibleIndex)
		{
			pstObj->stData.iLastVisibleIndex = pstObj->stData.iFocusIndex;
			pstObj->stData.iFirstVisibleIndex = pstObj->stData.iLastVisibleIndex - pstObj->stData.iVisibleCharNum + 1;

		}
        else if(pstObj->stData.iFocusIndex < pstObj->stData.iFirstVisibleIndex)
		{
			pstObj->stData.iFirstVisibleIndex = pstObj->stData.iFocusIndex;
			pstObj->stData.iLastVisibleIndex = pstObj->stData.iFirstVisibleIndex + pstObj->stData.iVisibleCharNum - 1;
		}
		/* Setup display offset. */
		if(pstObj->stParam.iTextLength < pstObj->stData.iVisibleCharNum)
		{
			pstObj->stData.iOffset = 0;
		}
		else if(pstObj->stData.iFocusIndex == pstObj->stData.iLastVisibleIndex)
		{
			pstObj->stData.iOffset = ((pstObj->stParam.stLayout.iWidth % pstObj->stParam.pstFontRes->iHalfWidth) - pstObj->stParam.pstFontRes->iHalfWidth)%pstObj->stParam.pstFontRes->iHalfWidth;
		}
		else if (pstObj->stData.iFocusIndex == pstObj->stData.iFirstVisibleIndex)
		{
			pstObj->stData.iOffset = 0;
		}
		/* Prepare paint text position. */
        stCharacterPos.iY = 0;
        stCharacterPos.iX = pstObj->stData.iOffset;

        /* Loop for paint each visible character. */
        iCharIdx = pstObj->stData.iFirstVisibleIndex;
        while((pstObj->stData.szValue[iCharIdx] != '\0') && (stCharacterPos.iX < SGUI_RECT_X_END(LAYOUT(pstObj))))
		{
			cPaintChar = ('\0' == cMask)?pstObj->stData.szValue[iCharIdx]:(iCharIdx == pstObj->stData.iFocusIndex)?pstObj->stData.szValue[iCharIdx]:cMask;
			if(eMode==SGUI_DRAW_NORMAL)
			{
				SGUI_Text_DrawASCIICharacter(pstDeviceIF, cPaintChar, pstObj->stParam.pstFontRes, &LAYOUT(pstObj), &stCharacterPos, (iCharIdx == pstObj->stData.iFocusIndex)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
			}
			else
			{
				SGUI_Text_DrawASCIICharacter(pstDeviceIF, cPaintChar, pstObj->stParam.pstFontRes, &LAYOUT(pstObj), &stCharacterPos, (iCharIdx == pstObj->stData.iFocusIndex)?SGUI_DRAW_NORMAL:SGUI_DRAW_REVERSE);
			}
			stCharacterPos.iX += pstObj->stParam.pstFontRes->iHalfWidth;
			iCharIdx++;
		}
    }
}

void SGUI_TextVariableBox_SetFocusIndex(SGUI_TEXT_VARBOX_STRUCT* pstObj, SGUI_INT iNewFocus)
{
    if((NULL != pstObj) && (NULL != pstObj->stData.szValue))
    {
        if(iNewFocus < pstObj->stParam.iTextLength)
        {
            pstObj->stData.iFocusIndex = iNewFocus;
            SGUI_TextVariableBox_UpdateFocusCharIndex(pstObj);
        }
    }
}

void SGUI_TextVariableBox_IncreaseChar(SGUI_TEXT_VARBOX_STRUCT* pstObj)
{
    if((NULL != pstObj) && (NULL != pstObj->stData.szValue))
    {
        if(VARBOX_TEXT_INVALID_CHAR_IDX == pstObj->stData.iFocusCharIndex)
        {
            pstObj->stData.iFocusCharIndex = 0;

        }
        else
        {
            ++(pstObj->stData.iFocusCharIndex);
            pstObj->stData.iFocusCharIndex = pstObj->stData.iFocusCharIndex % pstObj->stParam.stCharSet.iSize;
        }
        pstObj->stData.szValue[pstObj->stData.iFocusIndex] = pstObj->stParam.stCharSet.cszCharSet[pstObj->stData.iFocusCharIndex];
    }
}

void SGUI_TextVariableBox_DecreaseChar(SGUI_TEXT_VARBOX_STRUCT* pstObj)
{
    if((NULL != pstObj) && (NULL != pstObj->stData.szValue))
    {
        if(VARBOX_TEXT_INVALID_CHAR_IDX == pstObj->stData.iFocusCharIndex)
        {
            pstObj->stData.iFocusCharIndex = pstObj->stParam.stCharSet.iSize - 1;
            pstObj->stData.szValue[pstObj->stData.iFocusIndex] = pstObj->stParam.stCharSet.cszCharSet[pstObj->stData.iFocusCharIndex];
        }
        else
        {
            if(0 == pstObj->stData.iFocusCharIndex)
            {
                pstObj->stData.iFocusCharIndex = pstObj->stParam.stCharSet.iSize-1;
            }
            else
            {
                --(pstObj->stData.iFocusCharIndex);
            }
        }
        pstObj->stData.szValue[pstObj->stData.iFocusIndex] = pstObj->stParam.stCharSet.cszCharSet[pstObj->stData.iFocusCharIndex];
    }
}
