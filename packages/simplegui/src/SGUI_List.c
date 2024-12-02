#include "SGUI_List.h"

// User settings
#define     LIST_SCROLLBAR_WIDTH                        (5)
// Automatic calculation
#define     LIST_EDGE_SIZE                              1
#define     LIST_TITLE_HEIGHT(FONT)                     ((FONT)->iHeight+2)

void SGUI_List_Initialize(SGUI_LIST* pstObj, const SGUI_RECT* cpstLayout, const SGUI_FONT_RES* pstFontRes, SGUI_CSZSTR cszTitle, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
    SGUI_SCROLLBAR_PARAM    stScrollBarParam;
    SGUI_RECT               stSubLayout;

    SGUI_SystemIF_MemorySet(&stSubLayout, 0x00, sizeof(SGUI_RECT));

    // Copy layout.
    SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), cpstLayout, sizeof(SGUI_RECT));
    // Title
    pstObj->szTitle = cszTitle;
    // Initialize font resource.
    pstObj->pstFontRes = pstFontRes;
    // Initialize Items.
    stSubLayout.iX = pstObj->stLayout.iX+2;
    stSubLayout.iWidth = pstObj->stLayout.iWidth-4-LIST_SCROLLBAR_WIDTH;
    if(NULL == pstObj->szTitle)
    {
        stSubLayout.iY = pstObj->stLayout.iY+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-4;
    }
    else
    {
        stSubLayout.iY = pstObj->stLayout.iY+LIST_TITLE_HEIGHT(pstFontRes)+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-LIST_TITLE_HEIGHT(pstFontRes)-3;
    }
    SGUI_ItemsBase_Initialize(&(pstObj->stItems), &stSubLayout, pstFontRes, pstItemsData, iItemsCount);
    // Initialize scroll bar.
    stScrollBarParam.eDirection = SGUI_SCROLLBAR_VERTICAL;
    stScrollBarParam.stLayout.iX = pstObj->stItems.stLayout.iX+pstObj->stItems.stLayout.iWidth+1;
    stScrollBarParam.stLayout.iY = pstObj->stItems.stLayout.iY;
    stScrollBarParam.stLayout.iWidth = LIST_SCROLLBAR_WIDTH;
    stScrollBarParam.stLayout.iHeight = pstObj->stItems.stLayout.iHeight;
    stScrollBarParam.sMaxValue = (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0;
    SGUI_ScrollBar_Initialize(&(pstObj->stScrollBar), &stScrollBarParam);
}

void SGUI_List_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_LIST* pstObj)
{
    SGUI_RECT               stTitleTextDisplayArea;
    SGUI_POINT              stInnerPos;

    if(NULL != pstObj)
    {
        // Clear list item display area.
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
        // Paint title.
        if(NULL != pstObj->szTitle)
        {
            stTitleTextDisplayArea.iX = pstObj->stLayout.iX+2;
            stTitleTextDisplayArea.iY = pstObj->stLayout.iY+2;
            stTitleTextDisplayArea.iWidth = pstObj->stLayout.iWidth-4;
            stTitleTextDisplayArea.iHeight = pstObj->pstFontRes->iHeight;
            stInnerPos.iX = 0;
            stInnerPos.iY = 0;
            SGUI_Text_DrawText(pstDeviceIF, pstObj->szTitle, pstObj->pstFontRes, &stTitleTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
            SGUI_Basic_DrawLine(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, pstObj->stLayout.iX+pstObj->stLayout.iWidth-1, pstObj->stLayout.iY+pstObj->pstFontRes->iHeight+3, SGUI_COLOR_FRGCLR);
        }
        // Paint items.
        SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
        // Paint scroll bar.
        SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);
        SGUI_ScrollBar_Repaint(pstDeviceIF, &(pstObj->stScrollBar));
    }
}

void SGUI_List_RepaintItems(SGUI_SCR_DEV* pstDeviceIF, SGUI_LIST* pstObj)
{
    if(NULL != pstObj)
    {
        // Paint items.
        SGUI_ItemsBase_Repaint(pstDeviceIF, &(pstObj->stItems));
        // Paint scroll bar.
        SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);
        SGUI_ScrollBar_Repaint(pstDeviceIF, &(pstObj->stScrollBar));
    }
}

void SGUI_List_Resize(SGUI_LIST* pstObj, const SGUI_RECT* pstNewLayout)
{
    SGUI_RECT               stSubLayout;

    SGUI_SystemIF_MemorySet(&stSubLayout, 0x00, sizeof(SGUI_RECT));

    // Copy layout.
    SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), pstNewLayout, sizeof(SGUI_RECT));
    // Resize Items.
    stSubLayout.iX = pstObj->stLayout.iX+2;
    stSubLayout.iWidth = pstObj->stLayout.iWidth-4-LIST_SCROLLBAR_WIDTH;
    if(NULL == pstObj->szTitle)
    {
        stSubLayout.iY = pstObj->stLayout.iY+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-4;
    }
    else
    {
        stSubLayout.iY = stSubLayout.iY+LIST_TITLE_HEIGHT(pstObj->pstFontRes)+2;
        stSubLayout.iHeight = pstObj->stLayout.iHeight-LIST_TITLE_HEIGHT(pstObj->pstFontRes)-3;
    }
    SGUI_ItemsBase_Resize(&(pstObj->stItems), &stSubLayout);
    // Resize scroll bar.
    pstObj->stScrollBar.stParam.stLayout.iX = pstObj->stItems.stLayout.iX+pstObj->stItems.stLayout.iWidth+1;
    pstObj->stScrollBar.stParam.stLayout.iY = pstObj->stItems.stLayout.iY;
    pstObj->stScrollBar.stParam.stLayout.iWidth = LIST_SCROLLBAR_WIDTH;
    pstObj->stScrollBar.stParam.stLayout.iHeight = pstObj->stItems.stLayout.iHeight;
}

SGUI_ITEMS_ITEM* SGUI_List_RemoveItem(SGUI_LIST* pstObj, SGUI_INT iRemoveIndex)
{
    SGUI_ITEMS_ITEM*        pstRemovedItem;

    pstRemovedItem = SGUI_ItemsBase_RemoveItem(&(pstObj->stItems), iRemoveIndex);
    // Paint scroll bar.
    SGUI_ScrollBar_SetMax(&(pstObj->stScrollBar), (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0)
    SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);

    return pstRemovedItem;
}

SGUI_ITEMS_ITEM* SGUI_List_InsertItem(SGUI_LIST* pstObj, SGUI_ITEMS_ITEM* pstNewItem, SGUI_INT iIndex)
{
    SGUI_ITEMS_ITEM*        pstInsertedItem;

    pstInsertedItem = SGUI_ItemsBase_InsertItem(&(pstObj->stItems), pstNewItem, iIndex);

    // Paint scroll bar.
    SGUI_ScrollBar_SetMax(&(pstObj->stScrollBar), (pstObj->stItems.iCount > pstObj->stItems.iVisibleItems)?(pstObj->stItems.iCount - pstObj->stItems.iVisibleItems):0)
    SGUI_ScrollBar_SetValue(&(pstObj->stScrollBar), pstObj->stItems.stVisibleStart.iIndex);

    return pstInsertedItem;
}
