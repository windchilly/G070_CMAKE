#include "SGUI_ItemsBase.h"

#define     ITEMS_SENECT_IDX(ITEMS_PTR)         (ITEMS_PTR->stSelection.iIndex)
#define     ITEMS_SENECT_ITEM(ITEMS_PTR)            (ITEMS_PTR->stSelection.pstItem)
#define     ITEMS_VISIBLE_START_IDX(ITEMS_PTR)  (ITEMS_PTR->stVisibleStart.iIndex)
#define     ITEMS_VISIBLE_START_ITEM(ITEMS_PTR) (ITEMS_PTR->stVisibleStart.pstItem)
#define     ITEMS_VISIBLE_END_IDX(ITEMS_PTR)    (ITEMS_PTR->stVisibleEnd.iIndex)
#define     ITEMS_VISIBLE_END_ITEM(ITEMS_PTR)   (ITEMS_PTR->stVisibleEnd.pstItem)
#define     ITEMS_FIRST_ITEM(ITEMS_PTR)         (ITEMS_PTR->pstFirstItem)
#define     ITEMS_LAST_ITEM(ITEMS_PTR)          (ITEMS_PTR->pstLastItem)
#define     ITEMS_VISIBLE_ITEMS(ITEMS_PTR)      (ITEMS_PTR->iVisibleItems)

void SGUI_ItemsBase_Initialize(SGUI_ITEMS_BASE* pstObj, const SGUI_RECT* cpstLayout, const SGUI_FONT_RES* pstFontRes, SGUI_ITEMS_ITEM* pstItemsData, SGUI_INT iItemsCount)
{
    SGUI_INT                iItemIndex;
    SGUI_ITEMS_ITEM*        pstItem;

    if((NULL != pstObj) && (NULL != pstFontRes))
    {
        /* Paint font. */
        pstObj->pstFontRes = pstFontRes;
        /* Layout */
        SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), cpstLayout, sizeof(SGUI_RECT));
        /* Calculate the number of visible items. */
        ITEMS_VISIBLE_ITEMS(pstObj) = (pstObj->stLayout.iHeight-1)/ITEM_HEIGHT(pstObj->pstFontRes)+1;
        /* Items count. */
        pstObj->iCount = iItemsCount;
        /* Item paint offset. */
        pstObj->iItemPaintOffset = 0;
        /* Items data. */
        if(iItemsCount > 0)
        {
            /* Initialize for first item. */
            ITEMS_FIRST_ITEM(pstObj) = pstItemsData;
            pstItem = ITEMS_FIRST_ITEM(pstObj);
            pstItem->pstPrev = NULL;
            pstItem->pstNext = NULL;
            /* Loop for remaining items if existed. */
            for(iItemIndex=1; iItemIndex<iItemsCount; iItemIndex++)
            {
                (pstItemsData+iItemIndex)->pstPrev = pstItem;
                (pstItemsData+iItemIndex)->pstNext = NULL;
                pstItem->pstNext = (pstItemsData+iItemIndex);
                pstItem = pstItemsData+iItemIndex;
            }
            ITEMS_LAST_ITEM(pstObj) = pstItem;
            /* For initialize selection */
            ITEMS_SENECT_IDX(pstObj) = 0;
            ITEMS_SENECT_ITEM(pstObj) = ITEMS_FIRST_ITEM(pstObj);
            /* For first visible item. */
            ITEMS_VISIBLE_START_IDX(pstObj) = 0;
            ITEMS_VISIBLE_START_ITEM(pstObj) = ITEMS_FIRST_ITEM(pstObj);
            /* For last visible item.  */
            ITEMS_VISIBLE_END_IDX(pstObj) = ITEMS_VISIBLE_START_IDX(pstObj) + ITEMS_VISIBLE_ITEMS(pstObj) - 1;
            ITEMS_VISIBLE_END_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_FIRST_ITEM(pstObj), ITEMS_VISIBLE_ITEMS(pstObj)-1);
        }
        else
        {
            ITEMS_FIRST_ITEM(pstObj) = NULL;
            ITEMS_LAST_ITEM(pstObj) = NULL;
            ITEMS_SENECT_IDX(pstObj) = SGUI_INVALID_INDEX;
            ITEMS_SENECT_ITEM(pstObj) = NULL;
        }
    }
}

SGUI_ITEMS_ITEM* SGUI_ItemsBase_JumpItem(SGUI_ITEMS_ITEM* pstBaseItem, SGUI_INT iSteps)
{
    SGUI_ITEMS_ITEM*        pstFoundedItem;
    SGUI_INT                iStepCount;

    iStepCount =            iSteps;
    pstFoundedItem =        pstBaseItem;

    if(NULL != pstBaseItem)
    {
        if(iSteps < 0)
        {
            iStepCount = (~iStepCount)+1;
            while((iStepCount > 0) && (NULL != pstFoundedItem))
            {
                pstFoundedItem = pstFoundedItem->pstPrev;
                iStepCount--;
            }
        }
        else
        {
            iStepCount = iSteps;
            while((iStepCount > 0) && (NULL != pstFoundedItem))
            {
                pstFoundedItem = pstFoundedItem->pstNext;
                iStepCount--;
            }
        }
    }
    return pstFoundedItem;
}

void SGUI_ItemsBase_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_ITEMS_BASE* pstObj)
{
    SGUI_RECT               stItemPaintArea;
    SGUI_POINT              stItemTextPos;
    SGUI_ITEMS_ITEM*        pstPaintingItem;
    SGUI_CSZSTR             cszItemText;

    stItemTextPos.iX =      1;

    if((NULL != pstDeviceIF) && (NULL != pstObj))
    {
        /* Clear background */
        if(pstObj->iCount < pstObj->iVisibleItems)
        {
            SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stLayout.iX, pstObj->stLayout.iY, pstObj->stLayout.iWidth, pstObj->stLayout.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
        }

        if(pstObj->iCount > 0)
        {
            /* Judge selection index is valid. */
            if(ITEMS_SENECT_IDX(pstObj) < 0)
            {
                SGUI_ItemsBase_SelecteFirst(pstObj);
            }
            if(ITEMS_SENECT_IDX(pstObj) >= pstObj->iCount)
            {
                SGUI_ItemsBase_SelecteLast(pstObj);
            }
            /* Judge visible area. */
            if(ITEMS_SENECT_IDX(pstObj) < ITEMS_VISIBLE_START_IDX(pstObj))
            {
                SGUI_SystemIF_MemoryCopy(&(pstObj->stVisibleStart), &(pstObj->stSelection), sizeof(SGUI_ITEM_SELECTION));
                ITEMS_VISIBLE_END_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_VISIBLE_START_ITEM(pstObj), ITEMS_VISIBLE_ITEMS(pstObj) - 1);
                ITEMS_VISIBLE_END_IDX(pstObj) = ITEMS_VISIBLE_START_IDX(pstObj) + ITEMS_VISIBLE_ITEMS(pstObj) - 1;
            }
            if(ITEMS_SENECT_IDX(pstObj) > ITEMS_VISIBLE_END_IDX(pstObj))
            {
                SGUI_SystemIF_MemoryCopy(&(pstObj->stVisibleEnd), &(pstObj->stSelection), sizeof(SGUI_ITEM_SELECTION));
                ITEMS_VISIBLE_START_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_VISIBLE_END_ITEM(pstObj), 1 - ITEMS_VISIBLE_ITEMS(pstObj));
                ITEMS_VISIBLE_START_IDX(pstObj) = ITEMS_VISIBLE_END_IDX(pstObj) - ITEMS_VISIBLE_ITEMS(pstObj) + 1;
            }
            /* Initialize paint area. */
            stItemPaintArea.iX = pstObj->stLayout.iX;
            stItemPaintArea.iY = pstObj->stLayout.iY;
            stItemPaintArea.iWidth = pstObj->stLayout.iWidth;
            /* Judge items painting offset and calculate the height of the first visible item. */
            if((ITEMS_SENECT_IDX(pstObj) == ITEMS_VISIBLE_START_IDX(pstObj)) || (NULL == ITEMS_VISIBLE_END_ITEM(pstObj)))
            {
                pstObj->iItemPaintOffset = 0;
                stItemPaintArea.iHeight = (pstObj->iVisibleItems>1)?(ITEM_HEIGHT(pstObj->pstFontRes)):(pstObj->stLayout.iHeight);
            }
            else if(ITEMS_SENECT_IDX(pstObj) == ITEMS_VISIBLE_END_IDX(pstObj))
            {
                pstObj->iItemPaintOffset = (((pstObj->stLayout.iHeight)%ITEM_HEIGHT(pstObj->pstFontRes))-ITEM_HEIGHT(pstObj->pstFontRes))%ITEM_HEIGHT(pstObj->pstFontRes);
                //pstObj->iItemPaintOffset = ((pstObj->stLayout.iHeight)%ITEM_HEIGHT(pstObj->pstFontRes))-ITEM_HEIGHT(pstObj->pstFontRes);
                stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes)+pstObj->iItemPaintOffset;
            }
            else
            {
                stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
            }

            /* Paint first visible item. */
            pstPaintingItem = ITEMS_VISIBLE_START_ITEM(pstObj);
            stItemTextPos.iY = pstObj->iItemPaintOffset+1;
            cszItemText = (NULL == pstPaintingItem->szVariableText) ? (pstPaintingItem->cszLabelText) : (pstPaintingItem->szVariableText);
            SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, pstPaintingItem==ITEMS_SENECT_ITEM(pstObj)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
            /* Paint remaining items if existed. */
            pstPaintingItem = pstPaintingItem->pstNext;
            stItemPaintArea.iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
            if(pstObj->iVisibleItems > 1)
            {
                stItemPaintArea.iY += ITEM_HEIGHT(pstObj->pstFontRes)+(pstObj->iItemPaintOffset);
                stItemTextPos.iY = 1;
                while((pstPaintingItem != ITEMS_VISIBLE_END_ITEM(pstObj)) && (NULL != pstPaintingItem))
                {
                    cszItemText = (NULL == pstPaintingItem->szVariableText) ? (pstPaintingItem->cszLabelText) : (pstPaintingItem->szVariableText);
                    SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, pstPaintingItem==ITEMS_SENECT_ITEM(pstObj)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
                    stItemPaintArea.iY += ITEM_HEIGHT(pstObj->pstFontRes);
                    pstPaintingItem = pstPaintingItem->pstNext;
                }
                /* Paint last visible item. */
                if(NULL != pstPaintingItem)
                {
                    /* Paint last visible item. */
                    stItemPaintArea.iHeight = (0==pstObj->iItemPaintOffset)?(pstObj->stLayout.iHeight%ITEM_HEIGHT(pstObj->pstFontRes)):(ITEM_HEIGHT(pstObj->pstFontRes));
                    /* Correct last visible item height when items area height is an integer multiple of item height. */
                    stItemPaintArea.iHeight = (0==stItemPaintArea.iHeight)?ITEM_HEIGHT(pstObj->pstFontRes):stItemPaintArea.iHeight;
                    cszItemText = (NULL == pstPaintingItem->szVariableText) ? (pstPaintingItem->cszLabelText) : (pstPaintingItem->szVariableText);
                    SGUI_Text_DrawText(pstDeviceIF, cszItemText, pstObj->pstFontRes, &stItemPaintArea, &stItemTextPos, pstPaintingItem==ITEMS_SENECT_ITEM(pstObj)?SGUI_DRAW_REVERSE:SGUI_DRAW_NORMAL);
                }
            }
        }
    }
}

SGUI_ITEMS_ITEM* SGUI_ItemsBase_GetItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iIndex)
{
    SGUI_ITEMS_ITEM*            pstSelectedItem;
    SGUI_INT                    iLoopIdx;

    pstSelectedItem =           pstObj->pstFirstItem;
    iLoopIdx =                  0;

    while((iLoopIdx != iIndex) && (NULL != pstSelectedItem))
    {
        pstSelectedItem = pstSelectedItem->pstNext;
        iLoopIdx++;
    }

    return pstSelectedItem;
}

void SGUI_ItemsBase_GetItemExtent(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelection, SGUI_RECT* pstItemExtent)
{
    if((NULL != pstObj) && (iSelection > SGUI_INVALID_INDEX) && (iSelection < pstObj->iCount) && (NULL != pstItemExtent))
    {
        pstItemExtent->iX = pstObj->stLayout.iX;
        pstItemExtent->iWidth = pstObj->stLayout.iWidth;
        /* Item is not visible. */
        if((iSelection < ITEMS_VISIBLE_START_IDX(pstObj)) || (iSelection > ITEMS_VISIBLE_END_IDX(pstObj)))
        {
            pstItemExtent->iY = 0;
            pstItemExtent->iHeight = 0;
        }
        else
        {
            if((0 == pstObj->iItemPaintOffset) && (ITEMS_SENECT_ITEM(pstObj) == ITEMS_VISIBLE_END_ITEM(pstObj)))
            {
                pstItemExtent->iY = pstObj->stLayout.iY+(ITEM_HEIGHT(pstObj->pstFontRes)*(pstObj->iVisibleItems-1));
                pstItemExtent->iHeight = (0==pstObj->iItemPaintOffset)?(pstObj->stLayout.iHeight%ITEM_HEIGHT(pstObj->pstFontRes)):(ITEM_HEIGHT(pstObj->pstFontRes));
                /* Correct last visible item height when items area height is an integer multiple of item height. */
                pstItemExtent->iHeight = (0==pstItemExtent->iHeight)?ITEM_HEIGHT(pstObj->pstFontRes):pstItemExtent->iHeight;
            }
            else if((0 != pstObj->iItemPaintOffset) && (ITEMS_SENECT_ITEM(pstObj) == ITEMS_VISIBLE_START_ITEM(pstObj)))
            {
                pstItemExtent->iY = pstObj->stLayout.iY;
                pstItemExtent->iHeight = ITEM_HEIGHT(pstObj->pstFontRes)+pstObj->iItemPaintOffset;
            }
            else
            {
                pstItemExtent->iY = pstObj->stLayout.iY+((ITEMS_SENECT_IDX(pstObj) - ITEMS_VISIBLE_START_IDX(pstObj))*ITEM_HEIGHT(pstObj->pstFontRes))+pstObj->iItemPaintOffset;
                pstItemExtent->iHeight = ITEM_HEIGHT(pstObj->pstFontRes);
            }
        }
    }
}

void SGUI_ItemsBase_Resize(SGUI_ITEMS_BASE* pstObj, const SGUI_RECT* cpstNewLayout)
{
    /* Set new layout. */
    SGUI_SystemIF_MemoryCopy(&(pstObj->stLayout), cpstNewLayout, sizeof(SGUI_RECT));
    /* Calculate visible items number in new size. */
    ITEMS_VISIBLE_ITEMS(pstObj) = (pstObj->stLayout.iHeight-1)/ITEM_HEIGHT(pstObj->pstFontRes)+1;
    /* Visible items number is greater then items count. */
    if(ITEMS_VISIBLE_ITEMS(pstObj) > pstObj->iCount)
        {
        ITEMS_VISIBLE_START_ITEM(pstObj) = ITEMS_FIRST_ITEM(pstObj);
        ITEMS_VISIBLE_START_IDX(pstObj) = 0;
        ITEMS_VISIBLE_END_ITEM(pstObj) = NULL;
        ITEMS_VISIBLE_END_IDX(pstObj) = ITEMS_VISIBLE_ITEMS(pstObj) - 1;
        pstObj->iItemPaintOffset = 0;
    }
    /* Visible items number is NOT less then items count. */
    else
    {
        ITEMS_VISIBLE_END_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_VISIBLE_START_ITEM(pstObj), ITEMS_VISIBLE_ITEMS(pstObj)-1);
        if(NULL == ITEMS_VISIBLE_END_ITEM(pstObj))
        {
            ITEMS_VISIBLE_END_ITEM(pstObj) = ITEMS_LAST_ITEM(pstObj);
            ITEMS_VISIBLE_END_IDX(pstObj) = pstObj->iCount - 1;
            ITEMS_VISIBLE_START_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_VISIBLE_END_ITEM(pstObj), 1-ITEMS_VISIBLE_ITEMS(pstObj));
            ITEMS_VISIBLE_START_IDX(pstObj) = pstObj->iCount - ITEMS_VISIBLE_ITEMS(pstObj);
            pstObj->iItemPaintOffset = 0;
        }
        else
        {
            ITEMS_VISIBLE_END_IDX(pstObj) = ITEMS_VISIBLE_START_IDX(pstObj) + ITEMS_VISIBLE_ITEMS(pstObj) - 1;
        }
    }
}

void SGUI_ItemsBase_Selecte(SGUI_ITEMS_BASE* pstObj, SGUI_INT iSelectIndex)
{
    SGUI_INT                iSteps;

    iSteps =                iSelectIndex - SGUI_ItemsBase_GetSelection(pstObj)->iIndex;

    if(iSteps != 0)
    {
        ITEMS_SENECT_ITEM(pstObj) = SGUI_ItemsBase_JumpItem(ITEMS_SENECT_ITEM(pstObj), iSteps);
        if(NULL == ITEMS_SENECT_ITEM(pstObj))
        {
            ITEMS_SENECT_IDX(pstObj) = SGUI_INVALID_INDEX;
        }
        else
        {
            ITEMS_SENECT_IDX(pstObj) = iSelectIndex;
        }
    }
}

SGUI_ITEMS_ITEM* SGUI_ItemsBase_RemoveItem(SGUI_ITEMS_BASE* pstObj, SGUI_INT iRemoveIndex)
{
    SGUI_ITEMS_ITEM*        pstRemovedItem;

    pstRemovedItem =        SGUI_ItemsBase_JumpItem(pstObj->pstFirstItem, iRemoveIndex);

    if(NULL != pstRemovedItem)
    {
        /* Count changes. */
        --(pstObj->iCount);
        /* Judge first and last. */
        if(pstObj->pstFirstItem == pstRemovedItem)
        {
            pstObj->pstFirstItem = pstRemovedItem->pstNext;
        }
        if(pstObj->pstLastItem == pstRemovedItem)
        {
            pstObj->pstLastItem = pstRemovedItem->pstPrev;
        }
        /* Remove node from linked-list */
        if(NULL != pstRemovedItem->pstNext)
        {
            pstRemovedItem->pstNext->pstPrev = pstRemovedItem->pstPrev;
        }
        if(NULL != pstRemovedItem->pstPrev)
        {
            pstRemovedItem->pstPrev->pstNext = pstRemovedItem->pstNext;
        }
        /* Removed item is visible */
        if((iRemoveIndex >= ITEMS_VISIBLE_START_IDX(pstObj)) && (iRemoveIndex <= ITEMS_VISIBLE_END_IDX(pstObj)))
        {
            /* Reset selection when removed item is selected. */
            if(pstRemovedItem == pstObj->stSelection.pstItem)
            {
                if(NULL != pstRemovedItem->pstNext)
                {
                    ITEMS_SENECT_ITEM(pstObj) = pstRemovedItem->pstNext;
                    if(NULL == ITEMS_SENECT_ITEM(pstObj))
                    {
                        ITEMS_SENECT_IDX(pstObj) = SGUI_INVALID_INDEX;
                    }
                }
                else
                {
                    ITEMS_SENECT_ITEM(pstObj) = pstRemovedItem->pstPrev;
                    --ITEMS_SENECT_IDX(pstObj);
                }
            }
            /* When removed items is first visible. */
            if(pstRemovedItem == ITEMS_VISIBLE_START_ITEM(pstObj))
            {
                ITEMS_VISIBLE_START_ITEM(pstObj) = ITEMS_VISIBLE_START_ITEM(pstObj)->pstNext;
            }
            /* Set last visible item. */
            if(NULL != ITEMS_VISIBLE_START_ITEM(pstObj))
            {
                if(NULL != ITEMS_VISIBLE_END_ITEM(pstObj))
                {
                    if((NULL == ITEMS_VISIBLE_END_ITEM(pstObj)->pstNext) && (NULL != ITEMS_VISIBLE_START_ITEM(pstObj)->pstPrev))
                    {
                        ITEMS_VISIBLE_START_ITEM(pstObj) = ITEMS_VISIBLE_START_ITEM(pstObj)->pstPrev;
                        --ITEMS_VISIBLE_START_IDX(pstObj);
                        --ITEMS_VISIBLE_END_IDX(pstObj);
                    }
                    else
                    {
                        ITEMS_VISIBLE_END_ITEM(pstObj) = ITEMS_VISIBLE_END_ITEM(pstObj)->pstNext;
                    }
                }
            }
        }

        pstRemovedItem->pstNext = NULL;
        pstRemovedItem->pstPrev = NULL;
    }

    return pstRemovedItem;
}

SGUI_ITEMS_ITEM* SGUI_ItemsBase_InsertItem(SGUI_ITEMS_BASE* pstObj, SGUI_ITEMS_ITEM* pstNewItem, SGUI_INT iIndex)
{
    SGUI_ITEMS_ITEM*        pstInsertedItem;

    /* New items cannot greater then count of items. */
    if((iIndex > SGUI_INVALID_INDEX) && (iIndex <= pstObj->iCount))
    {
        pstNewItem->pstNext = NULL;
        pstNewItem->pstPrev = NULL;

        if(iIndex < pstObj->iCount)
        {
            /* Find the base items by given index. */
            pstInsertedItem = SGUI_ItemsBase_JumpItem(pstObj->pstFirstItem, iIndex);
            /* Insert new items to linked list. */
            pstNewItem->pstNext = pstInsertedItem;
            pstNewItem->pstPrev = pstInsertedItem->pstPrev;
            if(NULL != pstInsertedItem->pstPrev)
            {
                pstInsertedItem->pstPrev->pstNext = pstNewItem;
            }
            pstInsertedItem->pstPrev = pstNewItem;
            if(iIndex <= ITEMS_SENECT_IDX(pstObj))
            {
                ++ITEMS_SENECT_IDX(pstObj);
            }
            /* Insert to first item. */
            if(ITEMS_FIRST_ITEM(pstObj) == pstInsertedItem)
            {
                ITEMS_FIRST_ITEM(pstObj) = pstNewItem;
            }
            /* Insert before first visible item. */
            if(iIndex < ITEMS_VISIBLE_START_IDX(pstObj))
            {
                ++ITEMS_VISIBLE_START_IDX(pstObj);
                ++ITEMS_VISIBLE_END_IDX(pstObj);
            }
            /* Insert after last visible item. */
            else if(iIndex > ITEMS_VISIBLE_END_IDX(pstObj))
            {
                /* Do nothing. */
            }
            /* Insert to visible area. */
            else
            {
                /* Insert to first visible item. */
                if(iIndex == ITEMS_VISIBLE_START_IDX(pstObj))
                {
                    ITEMS_VISIBLE_START_ITEM(pstObj) = pstNewItem;
                }
                if(NULL != ITEMS_VISIBLE_END_ITEM(pstObj))
                {
                    ITEMS_VISIBLE_END_ITEM(pstObj) = ITEMS_VISIBLE_END_ITEM(pstObj)->pstPrev;
                }
                else if(ITEMS_VISIBLE_ITEMS(pstObj) == (SGUI_ItemsBase_Count(pstObj)+1))
                {
                    ITEMS_VISIBLE_END_ITEM(pstObj) = ITEMS_LAST_ITEM(pstObj);
                }
            }
        }
        /* Insert the new item to last of linked list. */
        else
        {
            /* Insert first item. */
            if(NULL == ITEMS_FIRST_ITEM(pstObj))
            {
                ITEMS_FIRST_ITEM(pstObj) = pstNewItem;
                ITEMS_LAST_ITEM(pstObj) = pstNewItem;
                ITEMS_VISIBLE_START_ITEM(pstObj) = pstNewItem;
                ITEMS_VISIBLE_START_IDX(pstObj) = 0;
                ITEMS_VISIBLE_END_ITEM(pstObj) = NULL;
                ITEMS_VISIBLE_END_IDX(pstObj) = ITEMS_VISIBLE_ITEMS(pstObj)-1;
            }
            else
            {
                ITEMS_LAST_ITEM(pstObj)->pstNext = pstNewItem;
                pstNewItem->pstPrev = ITEMS_LAST_ITEM(pstObj);
                ITEMS_LAST_ITEM(pstObj) = pstNewItem;
                if(ITEMS_VISIBLE_ITEMS(pstObj) == (SGUI_ItemsBase_Count(pstObj)+1))
                {
                    ITEMS_VISIBLE_END_ITEM(pstObj) = pstNewItem;
                }
            }
        }
        /* Count changes. */
        pstObj->iCount += 1;
    }
    /* Return NULL when inserted index is invalid. */
    else
    {
        pstNewItem = NULL;
    }

    return pstNewItem;
}

SGUI_BOOL SGUI_ItemsBase_CanScrollUp(SGUI_ITEMS_BASE* pstObj)
{
    SGUI_BOOL               bReturn;

    bReturn = ((ITEMS_VISIBLE_START_ITEM(pstObj) != ITEMS_FIRST_ITEM(pstObj)) || (pstObj->iItemPaintOffset != 0));

    return bReturn;
}

SGUI_BOOL SGUI_ItemsBase_CanScrollDown(SGUI_ITEMS_BASE* pstObj)
{
    SGUI_BOOL               bReturn;

    if(0 == (pstObj->stLayout.iHeight)%ITEM_HEIGHT(pstObj->pstFontRes))
    {
        /* When the list height is an integer multiple of the item height. */
        bReturn = (ITEMS_VISIBLE_END_ITEM(pstObj) != ITEMS_LAST_ITEM(pstObj));
    }
    else
    {
        /* When the list height is not an integer multiple of the item height. */
        bReturn = ((ITEMS_VISIBLE_END_ITEM(pstObj) != ITEMS_LAST_ITEM(pstObj)) || ((pstObj->iItemPaintOffset == 0)));
    }

    return bReturn;
}
