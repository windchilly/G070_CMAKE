#include "user_task.h"

event_id_t EVE_PRINTSTR = 1 << 0;

static void print_init(task_id_t task_id)
{
    cd_timer_startre(task_id, EVE_PRINTSTR, 100);
}

static cd_uint16_t print_process(task_id_t task_id, event_asb_t events)
{
    if(events & EVE_PRINTSTR)
    {
        static cd_uint16_t print_count = 0;
		cd_msg_bdy_t *msg;

        print_count++;
		msg = (cd_msg_bdy_t*)cd_msg_alloc(sizeof(cd_msg_bdy_t));
		msg->data = (cd_uint8_t *)&print_count;

		if(msg != NULL)
		{
			msg->hdr.event_id = EVE_MSG_PRINTSTAT;
			msg->hdr.status = 0;
			cd_msg_send(TASK_STAT, msg);
		}
		cd_task_seteve(TASK_TEST, EVE_TEST);

        return events ^ EVE_PRINTSTR;
    }

    return 0;
}

static cd_task_ops_t print_ops = {print_init, print_process};
static void print_task(void)
{
	cd_task_create(&print_ops, TASK_PRINT);
}
INIT_APP_EXPORT(print_task);
