#include "SGUI_ScrollBar.h"

void SGUI_ScrollBar_Initialize(SGUI_SCROLLBAR_STRUCT* pstObj, const SGUI_SCROLLBAR_PARAM* pcstInitParam)
{
    if((NULL != pstObj) && (NULL != pcstInitParam))
    {
        SGUI_SystemIF_MemorySet(pstObj, 0x00, sizeof(SGUI_SCROLLBAR_STRUCT));
        SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_SCROLLBAR_PARAM));
    }
}

void SGUI_ScrollBar_SetValue(SGUI_SCROLLBAR_STRUCT* pstObj, SGUI_SIZE sNewValue)
{
    if(NULL != pstObj)
    {
        if(sNewValue > pstObj->stParam.sMaxValue)
        {
            pstObj->stData.sValue = pstObj->stParam.sMaxValue;
        }
        else
        {
            pstObj->stData.sValue = sNewValue;
        }
    }
}

void SGUI_ScrollBar_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_SCROLLBAR_STRUCT* pstObj)
{
    SGUI_INT                    uiScrollBlockPos;
    SGUI_SIZE                   uiScrollBlockSize;

    if(SGUI_SCROLLBAR_VERTICAL == pstObj->stParam.eDirection)
    {
        uiScrollBlockSize = pstObj->stParam.stLayout.iWidth-2;
    }
    else
    {
        uiScrollBlockSize = pstObj->stParam.stLayout.iHeight-2;
    }

    if(((pstObj->stParam.stLayout.iHeight > 2) && (pstObj->stParam.stLayout.iWidth > 2)) && (pstObj->stParam.stLayout.iHeight != pstObj->stParam.stLayout.iWidth))
    {
        // Check new value must be less then max value.
        if(pstObj->stData.sValue > pstObj->stParam.sMaxValue)
        {
            pstObj->stData.sValue = pstObj->stParam.sMaxValue;
        }
        // Draw scroll bar edge.
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX, pstObj->stParam.stLayout.iY,
                                    pstObj->stParam.stLayout.iWidth, pstObj->stParam.stLayout.iHeight,
                                    SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);

        if(SGUI_SCROLLBAR_VERTICAL == pstObj->stParam.eDirection)
        {
            // Value lower limit is 0, scroll blocks must be greater then 0.
            if(pstObj->stParam.sMaxValue > 0)
            {
                uiScrollBlockPos = pstObj->stParam.stLayout.iY+1+((pstObj->stParam.stLayout.iHeight-uiScrollBlockSize-2)*pstObj->stData.sValue/pstObj->stParam.sMaxValue);
                // Redraw process block
                SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX+1, uiScrollBlockPos,
                                        uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
            }
            else
            {
                SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX+1, pstObj->stParam.stLayout.iY+1,
                                        uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
            }
        }
        else // Horizontal
        {
            // Value lower limit is 0, scroll blocks must be greater then 0.
            if(pstObj->stParam.sMaxValue > 0)
            {
                uiScrollBlockPos = pstObj->stParam.stLayout.iX+1+((pstObj->stParam.stLayout.iWidth-uiScrollBlockSize-2)*pstObj->stData.sValue/pstObj->stParam.sMaxValue);
                // Redraw process block
                SGUI_Basic_DrawRectangle(pstDeviceIF, uiScrollBlockPos, pstObj->stParam.stLayout.iY+1,
                                        uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
            }
            else
            {
                SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX+1, pstObj->stParam.stLayout.iY+1,
                                        uiScrollBlockSize, uiScrollBlockSize, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
            }
        }
    }
}
