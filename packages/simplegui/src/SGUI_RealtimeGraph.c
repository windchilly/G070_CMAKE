#include "SGUI_RealtimeGraph.h"

static SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue);

void SGUI_RealtimeGraph_Initialize(SGUI_RTGRAPH* pstRTGraph)
{
    SGUI_INT                    iValueIndex;

    if(NULL != pstRTGraph)
    {
        // yAxisMax must be greater then yAxisMin;
        if(pstRTGraph->stControl.yAxisMax < pstRTGraph->stControl.yAxisMin)
        {
            SGUI_SWAP(pstRTGraph->stControl.yAxisMax, pstRTGraph->stControl.yAxisMin);
        }
        // X-axis step in pixel must be greater then 2.
        if(pstRTGraph->stData.stRecorder.iSize > 2)
        {
            pstRTGraph->stControl.xAxisStepPixel = pstRTGraph->stLayout.iWidth / (pstRTGraph->stData.stRecorder.iSize-1);
            if(pstRTGraph->stControl.xAxisStepPixel < 2)
            {
                pstRTGraph->stControl.xAxisStepPixel = 2;
            }
        }
        else
        {
            pstRTGraph->stControl.xAxisStepPixel = 2;
        }

        pstRTGraph->stControl.ValueArea = pstRTGraph->stControl.yAxisMax - pstRTGraph->stControl.yAxisMin+1;

        // Zero point value must NOT greater then yAxisMax and NOT less then yAxisMin.
        if(pstRTGraph->stData.BaseLineValue > pstRTGraph->stControl.yAxisMax)
        {
            pstRTGraph->stData.BaseLineValue = pstRTGraph->stControl.yAxisMax;
        }
        if(pstRTGraph->stData.BaseLineValue < pstRTGraph->stControl.yAxisMin)
        {
            pstRTGraph->stData.BaseLineValue = pstRTGraph->stControl.yAxisMin;
        }

        // Initialize value array.
        for(iValueIndex=0; iValueIndex<pstRTGraph->stData.stRecorder.iSize; iValueIndex++)
        {
            pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstRTGraph->stData.BaseLineValue);;
        }
    }
}

void SGUI_RealtimeGraph_Repaint(SGUI_SCR_DEV* pstDeviceIF, SGUI_RTGRAPH* pstRTGraph)
{
    SGUI_INT                    iValueIndex;
    SGUI_INT                    iPixelCoordinateStartX, iPixelCoordinateEndX;
    SGUI_INT                    iBaseLineCoordinateY;

    if((NULL != pstRTGraph) && (NULL != pstDeviceIF))
    {
        // Draw frame
        SGUI_Basic_DrawRectangle(pstDeviceIF, SGUI_RECT_X_START(pstRTGraph->stLayout), SGUI_RECT_Y_START(pstRTGraph->stLayout),
                                    SGUI_RECT_WIDTH(pstRTGraph->stLayout), SGUI_RECT_HEIGHT(pstRTGraph->stLayout), SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);

        if(SGUI_TRUE == pstRTGraph->stControl.EnableBaseline)
        {
            iBaseLineCoordinateY = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstRTGraph->stData.BaseLineValue);
            SGUI_Basic_DrawHorizontalLine(pstDeviceIF, SGUI_RECT_X_START(pstRTGraph->stLayout), SGUI_RECT_X_END(pstRTGraph->stLayout), iBaseLineCoordinateY, SGUI_COLOR_FRGCLR);
        }

        if(pstRTGraph->stData.stRecorder.iSize > 1)
        {
            for(iValueIndex=1; iValueIndex<pstRTGraph->stData.stRecorder.iSize; iValueIndex++)
            {
                iPixelCoordinateStartX = ((iValueIndex-1) * pstRTGraph->stControl.xAxisStepPixel) + SGUI_RECT_X_START(pstRTGraph->stLayout);
                iPixelCoordinateEndX = (iValueIndex * pstRTGraph->stControl.xAxisStepPixel) + SGUI_RECT_X_START(pstRTGraph->stLayout);
                SGUI_Basic_DrawLine(pstDeviceIF, iPixelCoordinateStartX, pstRTGraph->stData.stRecorder.arrValue[iValueIndex-1].iYCoord,
                                        iPixelCoordinateEndX, pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord, SGUI_COLOR_FRGCLR);
            }
        }
    }
}

void SGUI_RealtimeGraph_PushRear(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue)
{
    SGUI_INT                    iValueIndex;
    SGUI_INT                    iLimitedValue;

    if(NULL != pstRTGraph)
    {
        // Push value to front.
        //      <<--
        // [0]--------[N]
        for(iValueIndex=0; iValueIndex<pstRTGraph->stData.stRecorder.iSize-1; iValueIndex++)
        {
            pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iValue = pstRTGraph->stData.stRecorder.arrValue[iValueIndex+1].iValue;
            pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord = pstRTGraph->stData.stRecorder.arrValue[iValueIndex+1].iYCoord;
        }
        // Append new value.
        iValueIndex = pstRTGraph->stData.stRecorder.iSize-1;
        pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iValue = iNewValue;
        iLimitedValue = iNewValue;
        if(iLimitedValue > pstRTGraph->stControl.yAxisMax)
        {
            iLimitedValue = pstRTGraph->stControl.yAxisMax;
        }
        if(iLimitedValue < pstRTGraph->stControl.yAxisMin)
        {
            iLimitedValue = pstRTGraph->stControl.yAxisMin;
        }
        pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, iLimitedValue);
    }
}

void SGUI_RealtimeGraph_PushFront(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iNewValue)
{
    SGUI_INT                    iValueIndex;
    SGUI_INT                    iLimitedValue;

    if(NULL != pstRTGraph)
    {
        // Push value to rear.
        //      -->>
        // [0]--------[N]
        for(iValueIndex=pstRTGraph->stData.stRecorder.iSize-1; iValueIndex>0; iValueIndex--)
        {
            pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iValue = pstRTGraph->stData.stRecorder.arrValue[iValueIndex-1].iValue;
            pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord = pstRTGraph->stData.stRecorder.arrValue[iValueIndex-1].iYCoord;
        }
        // Append new value.
        pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iValue = iNewValue;
        iLimitedValue = iNewValue;
        if(iLimitedValue > pstRTGraph->stControl.yAxisMax)
        {
            iLimitedValue = pstRTGraph->stControl.yAxisMax;
        }
        if(iLimitedValue < pstRTGraph->stControl.yAxisMin)
        {
            iLimitedValue = pstRTGraph->stControl.yAxisMin;
        }
        pstRTGraph->stData.stRecorder.arrValue[iValueIndex].iYCoord = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, iLimitedValue);
    }
}

SGUI_INT SGUI_RealtimeGraph_GetValuePointYCoordinate(SGUI_RTGRAPH* pstRTGraph, SGUI_INT iValue)
{
    SGUI_INT                    iDisplayValuePointAreaHeight;
    SGUI_INT                    iAbsoluteValue;
    SGUI_INT                    iValuePointCoordinate;

    iValuePointCoordinate =     -1;

    if(NULL != pstRTGraph)
    {
        iDisplayValuePointAreaHeight = SGUI_RECT_HEIGHT(pstRTGraph->stLayout);

        //Make sure the value is within the valid range
        if(iValue > pstRTGraph->stControl.yAxisMax)
        {
            iValue = pstRTGraph->stControl.yAxisMax;
        }
        if(iValue < pstRTGraph->stControl.yAxisMin)
        {
            iValue = pstRTGraph->stControl.yAxisMin;
        }
        iAbsoluteValue = iValue - pstRTGraph->stControl.yAxisMin;

        iValuePointCoordinate = iAbsoluteValue*iDisplayValuePointAreaHeight/pstRTGraph->stControl.ValueArea;

        iValuePointCoordinate = iDisplayValuePointAreaHeight-iValuePointCoordinate;

        iValuePointCoordinate = iValuePointCoordinate + SGUI_RECT_Y_START(pstRTGraph->stLayout)-1;
    }

    return iValuePointCoordinate;
}

void SGUI_RealtimeGraph_Cleanup(SGUI_RTGRAPH* pstRTGraph)
{
    if(pstRTGraph)
    {
        // Clean recorder data.
        SGUI_SystemIF_MemorySet(pstRTGraph->stData.stRecorder.arrValue, 0x00, sizeof(SGUI_RTGRAPH_POINT)*pstRTGraph->stData.stRecorder.iSize);
        // Reinitialize recorder data to baseline data.
        for(SGUI_INT iDataIdx=0; iDataIdx<pstRTGraph->stData.stRecorder.iSize; iDataIdx++)
        {
            pstRTGraph->stData.stRecorder.arrValue[iDataIdx].iValue = pstRTGraph->stData.BaseLineValue;
            pstRTGraph->stData.stRecorder.arrValue[iDataIdx].iYCoord = SGUI_RealtimeGraph_GetValuePointYCoordinate(pstRTGraph, pstRTGraph->stData.BaseLineValue);

        }
    }
}
