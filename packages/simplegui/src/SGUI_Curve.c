#include "SGUI_Curve.h"

void SGUI_Curve_Initialize(SGUI_CURVE_STRUCT* pstObj, const SGUI_CURVE_PARAM* pcstInitParam)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstObj) && (NULL != pcstInitParam))
    {
        SGUI_SystemIF_MemoryCopy(&(pstObj->stParam), (void*)pcstInitParam, sizeof(SGUI_CURVE_PARAM));
        SGUI_SystemIF_MemorySet(&(pstObj->stData), 0x00, sizeof(SGUI_CURVE_DATA));
    }
}

void SGUI_Curve_Repaint(SGUI_SCR_DEV* pstDeviceIF, const SGUI_CURVE_STRUCT* pstObj)
{
    SGUI_CURVE_POINT*       pcstLineStartPoint;
    SGUI_CURVE_POINT*       pcstLineEndPoint;

    if((NULL != pstDeviceIF) && (NULL != pstObj))
    {
        /* Clean up background. */
        SGUI_Basic_DrawRectangle(pstDeviceIF, pstObj->stParam.stLayout.iX, pstObj->stParam.stLayout.iY, pstObj->stParam.stLayout.iWidth, pstObj->stParam.stLayout.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
        if(pstObj->stData.stPoints.sCount > 1)
        {
#if 1
            pcstLineStartPoint = pstObj->stData.stPoints.pstHead;
            pcstLineEndPoint = pcstLineStartPoint->pstNext;
            while(NULL != pcstLineEndPoint)
            {
                /* Paint visible line. */
                if(SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY) ||
                    SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY))
                {
                    SGUI_Basic_DrawLine(pstDeviceIF, pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY,
                                            pcstLineEndPoint->stPosition.iX, pcstLineEndPoint->stPosition.iY, SGUI_COLOR_FRGCLR);
                }
                pcstLineStartPoint = pcstLineStartPoint->pstNext;
                pcstLineEndPoint = pcstLineStartPoint->pstNext;
            }
#else
            pcstLineStartPoint = pstObj->stData.stPoints.pstEnd;
            pcstLineEndPoint = pcstLineStartPoint->pstPrev;
            while(NULL != pcstLineEndPoint)
            {
                /* Paint visible line. */
                if(SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineStartPoint->stPosition)) || SGUI_Basic_PointIsInArea(&(pstObj->stParam.stLayout), &(pcstLineEndPoint->stPosition)))
                {
                    SGUI_Basic_DrawLine(pstDeviceIF, pcstLineStartPoint->stPosition.iX, pcstLineStartPoint->stPosition.iY,
                                            pcstLineEndPoint->stPosition.iX, pcstLineEndPoint->stPosition.iY, SGUI_COLOR_FRGCLR);
                }
                pcstLineStartPoint = pcstLineStartPoint->pstPrev;
                pcstLineEndPoint = pcstLineStartPoint->pstPrev;
            }
#endif
        }
    }
}

void SGUI_Curve_InitializePoint(SGUI_CURVE_POINT* pstNewPoint, SGUI_INT iX, SGUI_INT iY)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstNewPoint)
    {
        SGUI_SystemIF_MemorySet(pstNewPoint, 0x00, sizeof(SGUI_CURVE_POINT));
        pstNewPoint->stPoint.iX = iX;
        pstNewPoint->stPoint.iY = iY;
    }
}

void SGUI_Curve_AddPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if((NULL != pstObj) && (NULL != pstNewPoint))
    {
        if(0 == pstObj->stData.stPoints.sCount)
        {
            pstObj->stData.stPoints.pstHead = pstNewPoint;
            pstObj->stData.stPoints.pstEnd = pstNewPoint;
        }
        else
        {
            pstObj->stData.stPoints.pstEnd->pstNext = pstNewPoint;
            pstNewPoint->pstPrev = pstObj->stData.stPoints.pstEnd;
            pstObj->stData.stPoints.pstEnd = pstNewPoint;
        }
        SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
        pstObj->stData.stPoints.sCount++;
    }
}

SGUI_BOOL SGUI_Curve_InsertBeforePoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
    SGUI_CURVE_POINT*       pstBasePoint;
    SGUI_BOOL               bReturn;

    if((NULL == pstObj) || (NULL == pstNewPoint))
    {
        /* Invalid parameter. */
        bReturn = SGUI_FALSE;
    }
    else
    {
        pstBasePoint = pstObj->stData.pstFocused;
        do
        {
            if(NULL == pstBasePoint)
            {
                /* Current focused point must be set. */
                break;
            }
            if(pstBasePoint == pstObj->stData.stPoints.pstHead)
            {
                /* Cannot insert before head. */
                bReturn = SGUI_FALSE;
                break;
            }
            /* Judge new point X value. */
            if(pstNewPoint->stPoint.iX > pstBasePoint->stPoint.iX)
            {
                pstNewPoint->stPoint.iX = pstBasePoint->stPoint.iX;
            }
            if(pstNewPoint->stPoint.iX < pstBasePoint->pstPrev->stPoint.iX)
            {
                pstNewPoint->stPoint.iX = pstBasePoint->pstNext->stPoint.iX;
            }

            pstBasePoint->pstPrev->pstNext = pstNewPoint;
            pstNewPoint->pstPrev = pstBasePoint->pstPrev;
            pstNewPoint->pstNext = pstBasePoint;
            pstBasePoint->pstPrev = pstNewPoint;
            SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
            pstObj->stData.stPoints.sCount++;
            bReturn = SGUI_TRUE;
        }while(0);
    }
    return bReturn;
}

SGUI_BOOL SGUI_Curve_InsertAfterPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstNewPoint)
{
    SGUI_CURVE_POINT*       pstBasePoint;
    SGUI_BOOL               bReturn;

    if((NULL == pstObj) || (NULL == pstNewPoint))
    {
        /* Invalid parameter. */
        bReturn = SGUI_FALSE;
    }
    else
    {
        pstBasePoint = pstObj->stData.pstFocused;
        do
        {
            if(NULL == pstBasePoint)
            {
                /* Current focused point must be set. */
                break;
            }
            if(pstBasePoint == pstObj->stData.stPoints.pstEnd)
            {
                /* Cannot insert after end. */
                bReturn = SGUI_FALSE;
                break;
            }
            /* Judge new point X value. */
            if(pstNewPoint->stPoint.iX < pstBasePoint->stPoint.iX)
            {
                pstNewPoint->stPoint.iX = pstBasePoint->stPoint.iX;
            }
            if(pstNewPoint->stPoint.iX > pstBasePoint->pstNext->stPoint.iX)
            {
                pstNewPoint->stPoint.iX = pstBasePoint->pstNext->stPoint.iX;
            }
            pstBasePoint->pstNext->pstPrev = pstNewPoint;
            pstNewPoint->pstNext = pstBasePoint->pstNext;
            pstNewPoint->pstPrev = pstBasePoint;
            pstBasePoint->pstNext = pstNewPoint;
            SGUI_Curve_CalculatePointPosition(pstObj, pstNewPoint);
            pstObj->stData.stPoints.sCount++;
            bReturn = SGUI_TRUE;
        }while(0);
    }
    return bReturn;
}

SGUI_BOOL SGUI_Curve_RemovePoint(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstPoint)
{
    SGUI_BOOL               bReturn;

    if((NULL != pstObj) && (NULL != pstPoint) && (pstObj->stData.stPoints.sCount > 2))
    {
        if(NULL != pstPoint->pstNext)
        {
            pstPoint->pstNext->pstPrev =  pstPoint->pstPrev;
        }
        if(NULL != pstPoint->pstPrev)
        {
            pstPoint->pstPrev->pstNext = pstPoint->pstNext;
        }
        if(pstPoint == pstObj->stData.pstFocused)
        {
            pstObj->stData.pstFocused = NULL;
        }
        pstObj->stData.stPoints.sCount--;
        bReturn = SGUI_TRUE;
    }
    else
    {
        bReturn = SGUI_FALSE;
    }

    return bReturn;
}

void SGUI_Curve_CalculatePointPosition(SGUI_CURVE_STRUCT* pstObj, SGUI_CURVE_POINT* pstPoint)
{
    if((NULL != pstObj) && (NULL != pstPoint))
    {
        pstPoint->stPosition.iX = (SGUI_RECT_WIDTH(pstObj->stParam.stLayout)-1)*(pstPoint->stPoint.iX-pstObj->stParam.stXRange.iMin)/(SGUI_RANGE_SIZE(pstObj->stParam.stXRange)-1);
        pstPoint->stPosition.iY = (SGUI_RECT_HEIGHT(pstObj->stParam.stLayout)-1)*(pstPoint->stPoint.iY-pstObj->stParam.stYRange.iMin)/(SGUI_RANGE_SIZE(pstObj->stParam.stYRange)-1);
        pstPoint->stPosition.iX += pstObj->stParam.stLayout.iX;
        pstPoint->stPosition.iY = (SGUI_RECT_HEIGHT(pstObj->stParam.stLayout)-1)-pstPoint->stPosition.iY;
        pstPoint->stPosition.iY += pstObj->stParam.stLayout.iY;
    }
}

SGUI_CURVE_POINT* SGUI_Curve_FocusPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_INT iIndex)
{
    SGUI_CURVE_POINT*       pstFocusedPoint;
    pstFocusedPoint =       NULL;

    if(NULL != pstObj)
    {
        pstFocusedPoint = pstObj->stData.stPoints.pstHead;
        while((iIndex>0) && (NULL != pstFocusedPoint))
        {
            pstFocusedPoint = pstFocusedPoint->pstNext;
            iIndex--;
        }

        pstObj->stData.pstFocused = pstFocusedPoint;
    }
    return pstFocusedPoint;
}

SGUI_BOOL SGUI_Curve_UpdateFocusPoint(SGUI_CURVE_STRUCT* pstObj, SGUI_INT iX, SGUI_INT iY)
{
    SGUI_BOOL               bReturn;
    SGUI_CURVE_POINT*       pstFocusedPoint;
    bReturn =               SGUI_FALSE;

    if(NULL == pstObj)
    {
        /* Invalid parameter. */
        bReturn = SGUI_FALSE;
    }
    else
    {
        do
        {
            if(NULL == pstObj->stData.pstFocused)
            {
                bReturn = SGUI_FALSE;
                break;
            }
            pstFocusedPoint = pstObj->stData.pstFocused;
            /* Check point X value. */
            if((pstObj->stData.pstFocused == pstObj->stData.stPoints.pstHead) || (pstObj->stData.pstFocused == pstObj->stData.stPoints.pstEnd))
            {
                /* Cannot change X value when focused point is head or end. */
                iX = pstObj->stData.pstFocused->stPoint.iX;
            }
            else
            {
                if(NULL == pstObj->stData.pstFocused->pstPrev)
                {
                    iX = SGUI_MAX_OF(pstObj->stParam.stXRange.iMin, iX);
                }
                else
                {
                    iX = SGUI_MAX_OF(pstFocusedPoint->pstPrev->stPoint.iX, iX);
                }
                if(NULL == pstObj->stData.pstFocused->pstNext)
                {
                    iX = SGUI_MIN_OF(pstObj->stParam.stXRange.iMax, iX);
                }
                else
                {
                    iX = SGUI_MIN_OF(pstFocusedPoint->pstNext->stPoint.iX, iX);
                }
            }
            /* Check point Y value. */
            iY = SGUI_MAX_OF(pstObj->stParam.stYRange.iMin, iY);
            iY = SGUI_MIN_OF(pstObj->stParam.stYRange.iMax, iY);

            pstFocusedPoint->stPoint.iX = iX;
            pstFocusedPoint->stPoint.iY = iY;
            /* Recalculate point position. */
            SGUI_Curve_CalculatePointPosition(pstObj, pstFocusedPoint);
            bReturn = SGUI_TRUE;
        }while(0);
    }
    return bReturn;
}

void SGUI_Curve_HighlightFocus(SGUI_SCR_DEV* pstDeviceIF, SGUI_CURVE_STRUCT* pstObj)
{
    SGUI_RECT               stFocusMarkArea;

    if((NULL != pstDeviceIF) && (NULL != pstObj))
    {
        if(NULL != pstObj->stData.pstFocused)
        {
            stFocusMarkArea.iX = pstObj->stData.pstFocused->stPosition.iX-2;
            stFocusMarkArea.iY = pstObj->stData.pstFocused->stPosition.iY-2;
            stFocusMarkArea.iWidth = 5;
            stFocusMarkArea.iHeight = 5;

            if(SGUI_RECT_X_START(stFocusMarkArea) <= SGUI_RECT_X_START(pstObj->stParam.stLayout))
            {
                stFocusMarkArea.iWidth -= (SGUI_RECT_X_START(pstObj->stParam.stLayout)-stFocusMarkArea.iX);
                stFocusMarkArea.iX = SGUI_RECT_X_START(pstObj->stParam.stLayout);
            }

            if(SGUI_RECT_X_END(stFocusMarkArea) >= SGUI_RECT_X_END(pstObj->stParam.stLayout))
            {
                stFocusMarkArea.iWidth -= (SGUI_RECT_X_END(stFocusMarkArea)-SGUI_RECT_X_END(pstObj->stParam.stLayout));
            }

            if(SGUI_RECT_Y_START(stFocusMarkArea) <= SGUI_RECT_Y_START(pstObj->stParam.stLayout))
            {
                stFocusMarkArea.iHeight -= (SGUI_RECT_Y_START(pstObj->stParam.stLayout)-stFocusMarkArea.iY);
                stFocusMarkArea.iY = SGUI_RECT_Y_START(pstObj->stParam.stLayout);
            }

            if(SGUI_RECT_Y_END(stFocusMarkArea) >= SGUI_RECT_Y_END(pstObj->stParam.stLayout))
            {
                stFocusMarkArea.iHeight -= (SGUI_RECT_Y_END(stFocusMarkArea)-SGUI_RECT_Y_END(pstObj->stParam.stLayout));
            }
            /* Paint focused mark. */
            SGUI_Basic_DrawRectangle(pstDeviceIF, stFocusMarkArea.iX, stFocusMarkArea.iY, stFocusMarkArea.iWidth, stFocusMarkArea.iHeight, SGUI_COLOR_FRGCLR, SGUI_COLOR_FRGCLR);
        }
    }
}

SGUI_INT SGUI_Curve_GetDependentValue(SGUI_CURVE_STRUCT* pstObj, SGUI_INT iArgument)
{
    SGUI_INT            iDependent = 0;
    SGUI_CURVE_POINT*   pstAreaStart = NULL;
    SGUI_CURVE_POINT*   pstAreaEnd = NULL;

    if(pstObj)
    {
        pstAreaStart = pstObj->stData.stPoints.pstHead;
        pstAreaEnd = pstAreaStart->pstNext;

        while((NULL != pstAreaStart) && (NULL != pstAreaEnd))
        {
            if(iArgument > pstAreaEnd->stPoint.iX)
            {
                // Switch to next area if x-value is not in this area.
                pstAreaStart = pstAreaEnd;
                pstAreaEnd = pstAreaStart->pstNext;
            }
            else
            {
                iDependent = (iArgument-pstAreaStart->stPoint.iX) * (pstAreaEnd->stPoint.iY - pstAreaStart->stPoint.iY) / (pstAreaEnd->stPoint.iX - pstAreaStart->stPoint.iX) +  pstAreaStart->stPoint.iY;
                break;
            }
        }
    }
    return iDependent;
}
