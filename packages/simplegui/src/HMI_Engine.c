#include "HMI_Engine.h"

static HMI_ENGINE_OBJECT*           g_pstActivedEngineObject;
static HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID);

HMI_ENGINE_RESULT HMI_ActiveEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstScreenObject;

    eProcessResult =            HMI_RET_NORMAL;
    pstScreenObject =           NULL;

    if(NULL != pstHMIEngineObject)
    {
        if(HMI_SCREEN_ID_ANY != iScreenID)
        {
            pstScreenObject = HMI_GetScreenObjectInEngine(pstHMIEngineObject, iScreenID);
        }
        else
        {
            pstScreenObject = pstHMIEngineObject->ScreenObjPtr[0];
        }
        if(NULL != pstScreenObject)
        {
            pstHMIEngineObject->CurrentScreenObject = pstScreenObject;
        }
        else
        {
            eProcessResult = HMI_RET_INVALID_DATA;
        }
    }
    else
    {
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    if(HMI_RET_NORMAL == eProcessResult)
    {
        g_pstActivedEngineObject = pstHMIEngineObject;
    }
    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_StartEngine(const void* pstParameters)
{
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstStartScreen;

    eProcessResult =            HMI_RET_NORMAL;
    pstStartScreen =            NULL;

    if(NULL != g_pstActivedEngineObject)
    {
        pstStartScreen = g_pstActivedEngineObject->CurrentScreenObject;
        if(NULL != pstStartScreen)
        {
            if(NULL != pstStartScreen->pstActions)
            {
                if(NULL != pstStartScreen->pstActions->Prepare)
                {
                    eProcessResult = pstStartScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                    g_pstActivedEngineObject->Interface->fnSyncBuffer();
                }
            }
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_ProcessEvent(const HMI_EVENT_BASE* pstEvent)
{
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstCurrentScreen;
    SGUI_INT                    iActionID;

    eProcessResult =            HMI_RET_NORMAL;
    pstCurrentScreen =          NULL;

    if(NULL != g_pstActivedEngineObject)
    {
        // Get current displayed screen object.
        pstCurrentScreen = g_pstActivedEngineObject->CurrentScreenObject;
        if( (NULL == pstCurrentScreen) ||
            (NULL == pstCurrentScreen->pstActions) ||
            (NULL == pstCurrentScreen->pstActions->ProcessEvent))
        {
            // Screen ID is invalid or screen object is invalid.
            eProcessResult = HMI_RET_INVALID_DATA;

        }
        else
        {
            // Process event.
            eProcessResult = pstCurrentScreen->pstActions->ProcessEvent(g_pstActivedEngineObject->Interface, pstEvent, &iActionID);
        }

        // Run post process.
        eProcessResult = pstCurrentScreen->pstActions->PostProcess(g_pstActivedEngineObject->Interface, eProcessResult, iActionID);
        if( (NULL != g_pstActivedEngineObject->Interface) &&
            (NULL != g_pstActivedEngineObject->Interface->fnSyncBuffer))
        {
            g_pstActivedEngineObject->Interface->fnSyncBuffer();
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_SwitchScreen(SGUI_INT iDestScreenID, const void* pstParameters)
{
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstDestScreen;

    eProcessResult =            HMI_RET_NORMAL;

    if(NULL != g_pstActivedEngineObject)
    {
        pstDestScreen = HMI_GetScreenObjectInEngine(g_pstActivedEngineObject, iDestScreenID);
        if(NULL == pstDestScreen)
        {
            /* Not find screen object by ID. */
            eProcessResult = HMI_RET_INVALID_DATA;
        }
        else if(NULL != pstDestScreen->pstPrevious)
        {
            /* Cannot reenter to a screen object. */
            eProcessResult = HMI_RET_ERROR;
        }
        else
        {
            pstDestScreen->pstPrevious = g_pstActivedEngineObject->CurrentScreenObject;
            g_pstActivedEngineObject->CurrentScreenObject = pstDestScreen;
            if(NULL != pstDestScreen->pstActions)
            {
                if(NULL != pstDestScreen->pstActions->Prepare)
                {
                    eProcessResult = pstDestScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                }
            }
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_GoBack(const void* pstParameters)
{
    HMI_ENGINE_RESULT           eProcessResult;
    HMI_SCREEN_OBJECT*          pstPreviousScreen;

    eProcessResult =            HMI_RET_NORMAL;

    if(NULL != g_pstActivedEngineObject)
    {
        pstPreviousScreen = g_pstActivedEngineObject->CurrentScreenObject->pstPrevious;
        g_pstActivedEngineObject->CurrentScreenObject->pstPrevious = NULL;

        if(NULL != pstPreviousScreen)
        {
            g_pstActivedEngineObject->CurrentScreenObject = pstPreviousScreen;
            if(NULL != pstPreviousScreen->pstActions)
            {
                if(NULL != pstPreviousScreen->pstActions->Prepare)
                {
                    eProcessResult = pstPreviousScreen->pstActions->Prepare(g_pstActivedEngineObject->Interface, pstParameters);
                }
            }
        }
        else
        {
            eProcessResult = HMI_RET_INVALID_DATA;
        }
    }
    else
    {
        // No activated HMI engine object.
        eProcessResult = HMI_RET_INVALID_DATA;
    }

    return eProcessResult;
}

HMI_SCREEN_OBJECT* HMI_GetScreenObjectInEngine(HMI_ENGINE_OBJECT* pstHMIEngineObject, SGUI_INT iScreenID)
{
    HMI_SCREEN_OBJECT*          pstScreenObject;
    SGUI_INT                    iIndex;

    pstScreenObject =           NULL;
    iIndex =                    0;

    if(NULL != pstHMIEngineObject)
    {
        while(iIndex < pstHMIEngineObject->ScreenCount)
        {
            if(iScreenID == pstHMIEngineObject->ScreenObjPtr[iIndex]->iScreenID)
            {
                pstScreenObject = pstHMIEngineObject->ScreenObjPtr[iIndex];
                break;
            }
            else
            {
                iIndex++;
            }
        }
    }

    return pstScreenObject;
}

HMI_ENGINE_RESULT HMI_SetDeviceObject(SGUI_SCR_DEV* pstDeviceObj)
{
    HMI_ENGINE_RESULT           eProcessResult;

    eProcessResult =            HMI_RET_NORMAL;

    if((NULL == pstDeviceObj) || (NULL == g_pstActivedEngineObject))
    {
        eProcessResult = HMI_RET_INVALID_DATA;
    }
    else
    {
        g_pstActivedEngineObject->Interface = pstDeviceObj;
    }

    return eProcessResult;
}
