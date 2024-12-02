#include "demo.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"
#include "Resource.h"
#include "stdio.h"

typedef struct _master_page
{
	const SGUI_RECT shape[5];
	SGUI_CSZSTR value[5];
	const uint8_t index;
	char* var_val;
}master_page_t;

static master_page_t text[] = 
{
	{
		.shape[0] = {30, 0, 128, 12+1},			//title
		.shape[1] = {40, 30, 80 ,12+1},			//content
		.shape[2] = {100, 30, 20, 12+1},		//unit
		.shape[3] = {0, 51, 30, 12+1},			//prev
		.shape[4] = {98, 51, 30, 12+1},			//next
		.value[0] = "systick",
		.value[1] = "中文",
		.value[2] = "s",
		.value[3] = "PREV",
		.value[4] = "NEXT",
		.index = 0,
	},
	{
		.shape[0] = {30, 0, 128, 12+1},			//title
		.shape[1] = {40, 30, 80 ,12+1},			//content
		.shape[2] = {100, 30, 20, 12+1},		//unit
		.shape[3] = {0, 51, 30, 12+1},			//prev
		.shape[4] = {98, 51, 30, 12+1},			//next
		.value[0] = "time",
		.value[1] = "12:20",
		.value[2] = "h",
		.value[3] = "PREV",
		.value[4] = "NEXT",
		.index = 1,
	},
};

HMI_ENGINE_RESULT master_init(SGUI_SCR_DEV* pstDeviceIF)
{
	uint8_t num = sizeof(text) / sizeof(text[0]);
	
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT master_refresh(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{

	if(pstParameters != NULL)
	{
		uint8_t *num = (uint8_t *)(pstParameters);

	}

	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT master_prep(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    SGUI_POINT stInnerPos = {0};
	
    if(pstDeviceIF->fnClear != NULL)
        pstDeviceIF->fnClear();
    else
    {
		for(uint8_t i = 0; i < 5; i++)
			SGUI_Text_DrawText(pstDeviceIF, text[0].value[i], &GB2312_FZXS12, &text[0].shape[i], &stInnerPos, SGUI_DRAW_NORMAL);
    }

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT master_process(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    HMI_ENGINE_RESULT eProcessResult;
    SGUI_INT iProcessAction;
    KEY_PRESS_EVENT* pstKeyEvent;

    eProcessResult = HMI_RET_NORMAL;
    iProcessAction = HMI_DEMO_PROC_NO_ACT;
    pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;

    if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
        eProcessResult = HMI_RET_INVALID_DATA;

	switch(pstKeyEvent->Data.uiKeyValue)
	{
		case UP:
		{
            HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_MENU, NULL);
			break;
		}
		case DOWN:
		{
			HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_PAINT_TEXT, NULL);
			break;
		}
		case LEFT:
		{
			HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_LIST, NULL);
			break;
		}
		case RIGHT:
		{
			HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_VARIABLE_BOX, NULL);
			break;
		}
	}

    if(NULL != piActionID)
        *piActionID = iProcessAction;

    return eProcessResult;
}

HMI_ENGINE_RESULT master_postprocess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}

HMI_SCREEN_ACTION master_actions = 
{
	master_init,
	master_prep,
	master_refresh,
	master_process,
	master_postprocess
};

HMI_SCREEN_OBJECT g_master = {HMI_SCREEN_ID_MASTER, &master_actions};
