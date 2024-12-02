#include "SGUI_Basic.h"
#include "HMI_Engine.h"
#include "demo.h"
#include "oled.h"

uint32_t tick;

#define BMP_DATA_BUFFER_SIZE (512)

HMI_ENGINE_OBJECT g_engine;
SGUI_BYTE bmp_buf[BMP_DATA_BUFFER_SIZE];
SGUI_SCR_DEV g_device_interface;

HMI_SCREEN_OBJECT* g_screens[] =
{
	&g_master,
	&g_demo_text,
	&g_stHMIDemo_List,
	&g_stHMIDemo_VariableBox,
	&g_stHMIDemo_Menu,
};

HMI_ENGINE_RESULT InitializeHMIEngineObj(void)
{
    HMI_ENGINE_RESULT eProcessResult;
    int iIndex;
    eProcessResult = HMI_RET_NORMAL;

    SGUI_SystemIF_MemorySet(&g_device_interface, 0x00, sizeof(SGUI_SCR_DEV));
    SGUI_SystemIF_MemorySet(&g_engine, 0x00, sizeof(HMI_ENGINE_OBJECT));

    g_device_interface.stSize.iWidth = 128;
	g_device_interface.stSize.iHeight = 64;
	g_device_interface.fnSetPixel = setpixel;
	g_device_interface.fnClear = clear;
	g_device_interface.fnSyncBuffer = refresh;
    g_device_interface.stBuffer.pBuffer = bmp_buf;
    g_device_interface.stBuffer.sSize = BMP_DATA_BUFFER_SIZE;

    do
    {
        g_engine.ScreenCount = sizeof(g_screens)/sizeof(*g_screens);
        g_engine.ScreenObjPtr = g_screens;
        g_engine.Interface = &g_device_interface;

        if(NULL != g_engine.ScreenObjPtr)
        {
            for(iIndex=0; iIndex<g_engine.ScreenCount; iIndex++)
            {
                if( (NULL != g_engine.ScreenObjPtr[iIndex])
                    && (NULL != g_engine.ScreenObjPtr[iIndex]->pstActions)
                    && (NULL != g_engine.ScreenObjPtr[iIndex]->pstActions->Initialize)
                    )
                {
                    g_engine.ScreenObjPtr[iIndex]->pstActions->Initialize(&g_device_interface);
                    g_engine.ScreenObjPtr[iIndex]->pstPrevious = NULL;
                }
            }
        }
        else
        {
			return HMI_RET_INVALID_DATA;
        }

        eProcessResult = HMI_ActiveEngine(&g_engine, HMI_SCREEN_ID_DEMO_LIST);
        if(HMI_PROCESS_FAILED(eProcessResult))
            break;
        eProcessResult = HMI_StartEngine(NULL);
        if(HMI_PROCESS_FAILED(eProcessResult))
            break;
    }while(0);

    return eProcessResult;
}

void DemoMainProcess(void)
{
	static uint8_t flag;
	if(!flag)
		InitializeHMIEngineObj();
	flag = 1;

	DATA_EVENT stEvent;
	HMI_EVENT_INIT(stEvent);

	stEvent.Head.iID = EVENT_ID_KEY_PRESS;
//	stEvent.Data.iValue = button;

	tick = HAL_GetTick();
	HMI_ProcessEvent((HMI_EVENT_BASE*)&stEvent);
}
