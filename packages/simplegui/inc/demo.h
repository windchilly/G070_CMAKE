#ifndef __DEMO_H__
#define __DEMO_H__

#include <stdlib.h>
#include <stdio.h>
#include "SGUI_Typedef.h"
#include "SGUI_Common.h"
#include "HMI_Engine.h"
//#include "Resource.h"

typedef enum
{
    EVENT_ID_UNKNOW,
    EVENT_ID_KEY_PRESS,
    EVENT_ID_TIMER,
    EVENT_ID_RTC,
    EVENT_ID_MAX
}EVENT_ID;

typedef enum
{
    EVENT_TYPE_ANY,
    EVENT_TYPE_ACTION,
    EVENT_TYPE_DATA,
}EVENT_TYPE;

enum DEMO_PROC_ACTION_ID
{
    HMI_DEMO_PROC_NO_ACT,
    HMI_DEMO_PROC_CONFIRM,
    HMI_DEMO_PROC_CANCEL,
};

enum Key_Action
{
	DOWN  = 0x01,
	RIGHT = 0x02,
	LEFT  = 0x04,
	UP    = 0x08,
};

typedef struct
{
    SGUI_UINT16     uiKeyValue;
}KEY_EVENT_DATA;

typedef struct
{
    SGUI_INT        iValue;
}DUMMY_DATA_EVENT_DATA;

HMI_EVENT_TYPE_DECLARE(KEY_PRESS_EVENT, KEY_EVENT_DATA)
HMI_EVENT_TYPE_DECLARE(DATA_EVENT, DUMMY_DATA_EVENT_DATA);

#define HMI_SCREEN_ID_DEMO_SCROLLING_TEXT           (1001)
#define HMI_SCREEN_ID_DEMO_PAINT_TEXT               (1002)
#define HMI_SCREEN_ID_DEMO_LIST                     (1003)
#define HMI_SCREEN_ID_DEMO_TEXT_NOTICE              (1004)
#define HMI_SCREEN_ID_DEMO_BASIC_PAINT              (1005)
#define HMI_SCREEN_ID_DEMO_VARIABLE_BOX             (1006)
#define HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH          (1007)
#define HMI_SCREEN_ID_DEMO_MENU                     (1008)
#define HMI_SCREEN_ID_DEMO_CURVE                    (1009)
#define HMI_SCREEN_ID_DEMO_POLOR_COORD              (1010)

#define HMI_SCREEN_ID_MASTER 						(1100)

extern HMI_ENGINE_OBJECT g_engine;
extern HMI_SCREEN_OBJECT g_stHMIDemo_ScrollingText;
extern HMI_SCREEN_OBJECT g_stHMIDemo_List;
extern HMI_SCREEN_OBJECT g_stHMIDemo_Notice;
extern HMI_SCREEN_OBJECT g_stHMIDemo_RTCNotice;
extern HMI_SCREEN_OBJECT g_stHMIDemo_VariableBox;
extern HMI_SCREEN_OBJECT g_stHMIDemo_RealtimeGraph;
extern HMI_SCREEN_OBJECT g_stHMIDemo_Menu;
extern HMI_SCREEN_OBJECT g_stHMIDemo_BasicPaint;
extern HMI_SCREEN_OBJECT g_demo_text;
extern HMI_SCREEN_OBJECT g_stHMIDemo_Curve;
extern HMI_SCREEN_OBJECT g_stHMIDemo_PolorCoord;

extern HMI_SCREEN_OBJECT g_master;

#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
HMI_ENGINE_RESULT   InitializeHMIEngineObj(void);
void                DemoMainProcess(void);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __INCLUDE_DEMO_PROC_H__
