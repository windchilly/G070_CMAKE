#include "user_task.h"

const event_id_t EVE_MSG_TEST = 0xF002;

event_id_t EVE_TEST = 1 << 0;

static void task_test_init(task_id_t task_id)
{
	return;
}

static cd_uint16_t task_test_event_process(task_id_t task_id, event_asb_t events)
{
	if(events & EVE_TEST)
	{
		static cd_uint16_t times = 0;
		
		times++;

		LOG_D("TASK_TEST: %d", times);

		return events ^ EVE_TEST;
	}

    return 0;
}

static cd_task_ops_t test_ops = {task_test_init, task_test_event_process};
static void test_task(void)
{
	cd_task_create(&test_ops, TASK_TEST);
	LOG_I("TASK TEST CREATE SUCCESS");
}
INIT_APP_EXPORT(test_task);
