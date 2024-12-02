#ifndef _USER_TASK_H_
#define _USER_TASK_H_

#include "cdos.h"
#include "cd_msg.h"
#include "cd_task.h"
#include "cd_timer.h"

//#include "log.h"

typedef enum USER_ID
{
	USER_ID_BTN = 0xA500,
	USER_ID_MOT,
	USER_ID_RTC,
	USER_ID_FPGA,
	USER_ID_ALARM,
	USER_ID_LIGHT,
	USER_ID_PRES,
	USER_ID_LOG,

	USER_ID_CALC,
	USER_ID_VALID,
	USER_ID_ZERO,
}user_id_t;

typedef struct _user_dat
{
	user_id_t id;
	void *value;
}user_data_t;

//TASK_XXX should be one and only in the system, it ranges from (0 to 0xFFFE)
//0xFFFF is reserved for SYS_TSK_INIT
//TASK_XXX is also used to schedule task in a decreasing order when event is triggered
enum TASK_ID
{
	TASK_PRINT,
	TASK_TEST,
	TASK_STAT,
	TASK_BUTTON,

	TASK_ID_END = SYS_TSK_INIT,
};

//EVE_XXX is tied to task events, total num should be no more than 15(same as event_id_t) for each task
//it ranges from (1<<0 to 1<<14), (1<<15) is reserved for SYS_EVE_MSG
//EVE_XXX could be the same for different tasks, as for one particular task it should be uinque
extern event_id_t EVE_TEST;
extern event_id_t EVE_BUTTON;
extern event_id_t EVE_PRINTSTR;

//EVE_MSG_XXX is carried alongside msg's body
//as long as a msg is send to a task, SYS_EVE_MSG will be set to the task
//the handler process of the task will be called to handle the msg,
//using its msg_body's EVE_MSG_XXX to deal with different situations
//it ranges from (0 to 0xFFFF), use different IDs as possible
extern const event_id_t EVE_MSG_PRINTSTAT;
extern const event_id_t EVE_MSG_TEST;

#endif
