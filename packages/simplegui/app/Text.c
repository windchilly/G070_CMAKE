#include "demo.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"
#include "Resource.h"
#include "stdio.h"

typedef struct
{
	const SGUI_RECT shape[5];
	SGUI_CSZSTR value[5];
	const uint8_t index;
	char* var_val;
}text_page_t;

extern uint32_t tick;
char tp[10];
text_page_t text[] = 
{
	{
		.shape[0] = {30, 0, 128, 12+1},			//title
		.shape[1] = {40, 30, 80 ,12+1},			//content
		.shape[2] = {100, 30, 20, 12+1},		//unit
		.shape[3] = {0, 51, 30, 12+1},			//prev
		.shape[4] = {98, 51, 30, 12+1},			//next
		.value[0] = "systick",
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

HMI_ENGINE_RESULT demo_text_init(SGUI_SCR_DEV* pstDeviceIF)
{	
	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT demo_text_refresh(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    SGUI_POINT stInnerPos = {0};

	uint8_t *num = (uint8_t *)(pstParameters);
//    sprintf(tp, "%.1f", (float)tick / 1000);
//	text[0].value[1] = tp;

	for(uint8_t i = 0; i < 5; i++)
	{
		SGUI_Text_DrawText(pstDeviceIF, text[*num].value[i], &GB2312_FZXS12, &text[*num].shape[i], &stInnerPos, SGUI_DRAW_NORMAL);
	}

	return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT demo_text_prep(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    if(NULL != pstDeviceIF->fnClear)
    {
        pstDeviceIF->fnClear();
    }
    else
    {
        SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, pstDeviceIF->stSize.iWidth, pstDeviceIF->stSize.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
    }
    demo_text_refresh(pstDeviceIF, NULL);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT demo_text_process(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_INT                    iProcessAction;
    KEY_PRESS_EVENT*            pstKeyEvent;

    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;
    pstKeyEvent =               (KEY_PRESS_EVENT*)pstEvent;

    // Check event is valid.
    if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
    {
        // Event data is invalid.
        eProcessResult = HMI_RET_INVALID_DATA;
    }
	switch(pstKeyEvent->Data.uiKeyValue)
	{
		case RIGHT:
		{
            demo_text_refresh(pstDeviceIF, NULL);
			// iProcessAction = HMI_DEMO_PROC_CANCEL;
			break;
		}
		case DOWN:
		{
//			demo_text_refresh(pstDeviceIF, );
			break;
		}
		case UP:
		{
//			demo_text_refresh(pstDeviceIF, );
			break;
		}
	}

    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT demo_text_postprocess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
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

HMI_SCREEN_ACTION demo_text_actions = 
{  
	demo_text_init,
	demo_text_prep,
	demo_text_refresh,
	demo_text_process,
	demo_text_postprocess
};

HMI_SCREEN_OBJECT g_demo_text = {HMI_SCREEN_ID_DEMO_PAINT_TEXT, &demo_text_actions};
