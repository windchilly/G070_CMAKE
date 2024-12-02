#include "demo.h"
#include "SGUI_List.h"
#include "SGUI_FontResource.h"
#include "Resource.h"

#define NOTICE_TEXT_BUFFER_SIZE (64)

static HMI_ENGINE_RESULT HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT HMI_DemoList_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

static const SGUI_RECT s_arrstLayouts[] =
{ 
	{0, 0, 128, 64},
	{0, 0, 192, 96},
	{0, 0, 192, 128},
};

static SGUI_SIZE s_iLayoutIndex = 1;

static SGUI_ITEMS_ITEM s_arrstListItems[] =
{
	{SCR1_LIST_ITEM1, NULL},
	{SCR1_LIST_ITEM2, NULL},
	{SCR1_LIST_ITEM3, NULL},
	{SCR1_LIST_ITEM4, NULL},
	{SCR1_LIST_ITEM5, NULL},
	{SCR1_LIST_ITEM6, NULL},
	{SCR1_LIST_ITEM7, NULL},
	{SCR1_LIST_ITEM8, NULL}
};

typedef struct
{
	SGUI_ITEMS_ITEM stItem;
	SGUI_BOOL bUsed;
}SGUI_TEST_ITEM;

static SGUI_TEST_ITEM s_arrstAppendListItems[] =  
{   
	{{SCR1_LIST_APPEND1, NULL}, SGUI_FALSE},
	{{SCR1_LIST_APPEND2, NULL}, SGUI_FALSE},
	{{SCR1_LIST_APPEND3, NULL}, SGUI_FALSE},
	{{SCR1_LIST_APPEND4, NULL}, SGUI_FALSE},
	{{SCR1_LIST_APPEND5, NULL}, SGUI_FALSE},
	{{SCR1_LIST_APPEND6, NULL}, SGUI_FALSE}
};

static SGUI_LIST s_stDemoListObject = {0};

HMI_SCREEN_ACTION s_stDemoListActions = 
{   
	HMI_DemoList_Initialize,
	HMI_DemoList_Prepare,
	HMI_DemoList_RefreshScreen,
	HMI_DemoList_ProcessEvent,
	HMI_DemoList_PostProcess
};

HMI_SCREEN_OBJECT g_stHMIDemo_List = 
{   
	HMI_SCREEN_ID_DEMO_LIST,
	&s_stDemoListActions
};

HMI_ENGINE_RESULT HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
    SGUI_RECT stListLayout;
    SGUI_SystemIF_MemorySet(&s_stDemoListObject, 0x00, sizeof(SGUI_LIST));
    stListLayout.iX = 0;
    stListLayout.iY = 0;
    stListLayout.iWidth = pstDeviceIF->stSize.iWidth;
    stListLayout.iHeight = pstDeviceIF->stSize.iHeight;
    SGUI_List_Initialize(&s_stDemoListObject, &stListLayout, &GB2312_FZXS12, SCR1_TITLE, s_arrstListItems, sizeof(s_arrstListItems)/sizeof(SGUI_ITEMS_ITEM));
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_INT                    iProcessAction;
	KEY_PRESS_EVENT*            pstKeyEvent;
	
    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;
	pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;
	
	switch(pstKeyEvent->Data.uiKeyValue)
	{
		case UP:
		{
			if(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex < (s_stDemoListObject.stItems.iCount))
				SGUI_List_Selecte(&s_stDemoListObject, SGUI_List_GetSelection(&s_stDemoListObject)->iIndex-1);
			SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
			break;
		}
		case DOWN:
		{
			if(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex < (s_stDemoListObject.stItems.iCount-1))
				SGUI_List_Selecte(&s_stDemoListObject, SGUI_List_GetSelection(&s_stDemoListObject)->iIndex+1);
			SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
			break;
		}
		case LEFT:
		{
			iProcessAction = HMI_DEMO_PROC_CONFIRM;
			break;
		}
		case RIGHT:
		{
			iProcessAction = HMI_DEMO_PROC_CANCEL;
			break;
		}
	}

	if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_DEMO_PROC_CONFIRM == iActionID)
    {
        switch(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex)
        {
            case 0:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_BASIC_PAINT, NULL);
                break;
            }
            case 1:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_PAINT_TEXT, NULL);
                break;
            }
            case 2:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_VARIABLE_BOX, NULL);
                break;
            }
            case 3:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH, NULL);
                break;
            }
            case 4:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_MENU, NULL);
                break;
            }
            case 5:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_TEXT_NOTICE, SCR7_NOTICE_TEXT);
                break;
            }
            case 6:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_CURVE, NULL);
                break;
            }
            case 7:
            {
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_POLOR_COORD, NULL);
                break;
            }
            default:
            {
                /* do nothing. */
            }
        }
    }
    else if(HMI_DEMO_PROC_CANCEL == iActionID)
    {
        HMI_GoBack(NULL);
    }

    return HMI_RET_NORMAL;
}
