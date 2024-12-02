#include "demo.h"
#include "Resource.h"
#include "SGUI_Notice.h"
#include "SGUI_VariableBox.h"
#include "SGUI_FontResource.h"
#include "SGUI_IconResource.h"

extern uint32_t tick;

#define                     VARIABLE_BOX_WIDTH                  (100)
#define                     VARIABLE_NUMBER_BOX_HEIGHT          (8)
#define                     VARIABLE_TEXT_BOX_HEIGHT            (12)
#define                     VARIABLE_BOX_POSX                   (10)
#define                     VARIABLE_BOX_NUMBER_POSY            (40)
#define                     VARIABLE_BOX_TEXT_POSY              (15)
#define                     VARIABLE_MASK_CHARACTER             NULL		//('*')

static HMI_ENGINE_RESULT    HMI_DemoVariableBox_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoVariableBox_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);
static void                 HMI_DemoVariableBox_DrawFrame(SGUI_SCR_DEV* pstDeviceIF, SGUI_SZSTR szTitle);

static SGUI_NUM_VARBOX_STRUCT   s_stNumberVariableBox =     {0x00};
static const SGUI_CHAR          ASCII_CHAR_SET[] = {" !\"#$%&,()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[/]^_'abcdefghijklmnopqrstuvwxyz{|}`"};
static SGUI_CHAR                s_szTextVariableBuffer[] =  {"ABCDEFG1234567890+-*/OPQ"};
static SGUI_TEXT_VARBOX_STRUCT  s_stTextVariableBox =       {0x00};
static SGUI_SZSTR               s_szFrameTitle =            "TITLE";
static SGUI_INT                 s_uiFocusedFlag;
HMI_SCREEN_ACTION               s_stDemoVariableBoxActions = {
                                                                HMI_DemoVariableBox_Initialize,
                                                                HMI_DemoVariableBox_Prepare,
                                                                HMI_DemoVariableBox_RefreshScreen,
                                                                HMI_DemoVariableBox_ProcessEvent,
                                                                HMI_DemoVariableBox_PostProcess,
                                                            };

HMI_SCREEN_OBJECT               g_stHMIDemo_VariableBox =   {   HMI_SCREEN_ID_DEMO_VARIABLE_BOX,
                                                                &s_stDemoVariableBoxActions
                                                            };

HMI_ENGINE_RESULT HMI_DemoVariableBox_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
    SGUI_TEXT_VARBOX_PARAM  stTextBoxInitParam;

    s_uiFocusedFlag = 0;

    stTextBoxInitParam.pstFontRes = &SGUI_DEFAULT_FONT_16;
    stTextBoxInitParam.stLayout.iX = VARIABLE_BOX_POSX+2;
    stTextBoxInitParam.stLayout.iY = VARIABLE_BOX_TEXT_POSY+12;
    stTextBoxInitParam.stLayout.iWidth = pstDeviceIF->stSize.iWidth-(VARIABLE_BOX_POSX*2)-4;
    stTextBoxInitParam.stLayout.iHeight = SGUI_DEFAULT_FONT_16.iHeight;
    stTextBoxInitParam.iTextLength = SGUI_SystemIF_StringLength(s_szTextVariableBuffer);
    stTextBoxInitParam.stCharSet.cszCharSet = ASCII_CHAR_SET;
    stTextBoxInitParam .stCharSet.iSize = 96;

    SGUI_TextVariableBox_Initialize(&s_stTextVariableBox, &stTextBoxInitParam, s_szTextVariableBuffer);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    HMI_DemoVariableBox_DrawFrame(pstDeviceIF, (SGUI_SZSTR)s_szFrameTitle);
	HMI_DemoVariableBox_RefreshScreen(pstDeviceIF, NULL);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    // Draw frame
    HMI_DemoVariableBox_DrawFrame(pstDeviceIF, (SGUI_SZSTR)s_szFrameTitle);
	// sprintf(tp, "%d", tick);
	// s_stTextVariableBox.stData.szValue = tp;
    SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_NORMAL);

    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_UINT16                 uiKeyValue;
    KEY_PRESS_EVENT*            pstKeyEvent;
    SGUI_INT                    iProcessAction;

    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;
	pstKeyEvent = (KEY_PRESS_EVENT*)pstEvent;

	switch(pstKeyEvent->Data.uiKeyValue)
	{
		case UP:
		{
			s_uiFocusedFlag = ((s_uiFocusedFlag+1)%2);
			if(0 == s_uiFocusedFlag)
			{
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_NORMAL);
			}
			else
			{
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			break;
		}
		case DOWN:
		{
			if(1 == s_uiFocusedFlag)
			{
				SGUI_TextVariableBox_DecreaseIndex(&s_stTextVariableBox);
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			break;
		}
		case LEFT:
		{
			if(1 == s_uiFocusedFlag)
			{
				SGUI_TextVariableBox_IncreaseChar(&s_stTextVariableBox);
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			else
			{
				SGUI_NumberVariableBox_SetValue(&s_stNumberVariableBox, SGUI_NumberVariableBox_GetValue(&s_stNumberVariableBox)+1);
				SGUI_NumberVariableBox_Repaint(pstDeviceIF, &s_stNumberVariableBox, SGUI_DRAW_REVERSE);
			}
			break;
		}
		case 103:
		{
			if(1 == s_uiFocusedFlag)
			{
				SGUI_TextVariableBox_IncreaseIndex(&s_stTextVariableBox);
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			break;
		}
		case 102:
		{
			if(1 == s_uiFocusedFlag)
			{
				SGUI_TextVariableBox_DecreaseChar(&s_stTextVariableBox);
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			else
			{
				SGUI_NumberVariableBox_SetValue(&s_stNumberVariableBox, SGUI_NumberVariableBox_GetValue(&s_stNumberVariableBox)-1);
				SGUI_NumberVariableBox_Repaint(pstDeviceIF, &s_stNumberVariableBox, SGUI_DRAW_REVERSE);
			}
			break;
		}
		case RIGHT:
		{
			if(1 == s_uiFocusedFlag)
			{
				s_szFrameTitle = SGUI_TextVariableBox_GetText(&s_stTextVariableBox);
				HMI_DemoVariableBox_DrawFrame(pstDeviceIF, (SGUI_SZSTR)s_szFrameTitle);
				// Draw number box
				SGUI_Basic_DrawRectangle(pstDeviceIF, VARIABLE_BOX_POSX, VARIABLE_BOX_NUMBER_POSY, s_stNumberVariableBox.stParam.stLayout.iWidth+4,
										 s_stNumberVariableBox.stParam.stLayout.iHeight+4, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
				SGUI_NumberVariableBox_Repaint(pstDeviceIF, &s_stNumberVariableBox, SGUI_DRAW_NORMAL);
				// Draw text box
				SGUI_Basic_DrawRectangle(pstDeviceIF, VARIABLE_BOX_POSX, VARIABLE_BOX_TEXT_POSY, s_stTextVariableBox.stParam.stLayout.iWidth+4,
										 s_stTextVariableBox.stParam.stLayout.iHeight+4, SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
				SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_REVERSE);
			}
			break;
		}
		default:
		{
			char *tp;
			sprintf(tp, "%d", tick / 1000);
			s_stTextVariableBox.stData.szValue = tp;
			SGUI_TextVariableBox_Repaint(pstDeviceIF, &s_stTextVariableBox, VARIABLE_MASK_CHARACTER, SGUI_DRAW_NORMAL);
			break;
		}
	}

    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoVariableBox_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
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

void HMI_DemoVariableBox_DrawFrame(SGUI_SCR_DEV* pstDeviceIF, SGUI_SZSTR szTitle)
{
    SGUI_RECT               stTextDisplayArea;
    SGUI_POINT              stInnerPos;

    stTextDisplayArea.iX = 4;
    stTextDisplayArea.iY = 4;
    stTextDisplayArea.iHeight = 12;
    stInnerPos.iX = 0;
    stInnerPos.iY = 0;

    if(NULL != pstDeviceIF)
    {
        stTextDisplayArea.iWidth = pstDeviceIF->stSize.iWidth-8;
        SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, SGUI_RECT_WIDTH(pstDeviceIF->stSize), SGUI_RECT_HEIGHT(pstDeviceIF->stSize), SGUI_COLOR_FRGCLR, SGUI_COLOR_BKGCLR);
        SGUI_Basic_DrawRectangle(pstDeviceIF, 2, 2, SGUI_RECT_WIDTH(pstDeviceIF->stSize)-4, SGUI_RECT_HEIGHT(pstDeviceIF->stSize)-4, SGUI_COLOR_FRGCLR, SGUI_COLOR_TRANS);
        SGUI_Basic_DrawLine(pstDeviceIF, 3, 17, 124, 17, SGUI_COLOR_FRGCLR);
        SGUI_Text_DrawText(pstDeviceIF, szTitle, &GB2312_FZXS12, &stTextDisplayArea, &stInnerPos, SGUI_DRAW_NORMAL);
    }
}
