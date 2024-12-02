#include "SGUI_Notice.h"

#define SGUI_NOTICE_MARGIN                              (5)

SGUI_SIZE SGUI_Notice_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_NOTICT_BOX* pstObject, const SGUI_FONT_RES* pstFontRes, SGUI_INT uiTextOffset)
{
    SGUI_SIZE               uiTextLines;
    SGUI_RECT               stIconDisplayArea;
    SGUI_POINT              stIconPosition;
    SGUI_RECT               stTextDisplayArea;

    if((NULL != pstObject) && (NULL != pstObject->cszNoticeText))
    {
        // Draw edgeNOTICE_BOX_MARGIN
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObject->stLayout.iX, pstObject->stLayout.iY, pstObject->stLayout.iWidth, pstObject->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);

        stTextDisplayArea.iY = pstObject->stLayout.iY+2;
        stTextDisplayArea.iHeight = pstObject->stLayout.iHeight-4;
        if(NULL == pstObject->pstIcon)
        {
            stTextDisplayArea.iX = pstObject->stLayout.iX+2;
            stTextDisplayArea.iWidth = pstObject->stLayout.iWidth-4;
        }
        else
        {
            stTextDisplayArea.iX = pstObject->stLayout.iX+pstObject->pstIcon->iWidth+4;
            stTextDisplayArea.iWidth = pstObject->stLayout.iWidth-pstObject->pstIcon->iWidth-6;

            stIconDisplayArea.iX = pstObject->stLayout.iX+2;
            stIconDisplayArea.iY = pstObject->stLayout.iY+2;
            stIconDisplayArea.iWidth = pstObject->pstIcon->iWidth;
            stIconDisplayArea.iHeight = pstObject->pstIcon->iHeight;
            stIconPosition.iX = 0;
            stIconPosition.iY = 0;
            // Paint icon.
            SGUI_Basic_DrawBitMap(pstDeviceIF, &stIconDisplayArea, &stIconPosition, pstObject->pstIcon, SGUI_DRAW_NORMAL);
        }
        // Draw text;
        uiTextLines = SGUI_Text_DrawMultipleLinesText(pstDeviceIF, pstObject->cszNoticeText, pstFontRes, &stTextDisplayArea, uiTextOffset, SGUI_DRAW_NORMAL);
    }
    return uiTextLines;
}

void SGUI_Notice_FitArea(SGUI_SCR_DEV* pstDeviceIF, SGUI_RECT* pstFitArea)
{
    if((NULL != pstDeviceIF) && (NULL != pstFitArea))
    {
        pstFitArea->iX = SGUI_NOTICE_MARGIN;
        pstFitArea->iY = SGUI_NOTICE_MARGIN;
        pstFitArea->iWidth = pstDeviceIF->stSize.iWidth-(SGUI_NOTICE_MARGIN<<1);
        pstFitArea->iHeight = pstDeviceIF->stSize.iHeight-(SGUI_NOTICE_MARGIN<<1);
    }
}
