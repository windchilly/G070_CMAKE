#include "user_task.h"

const event_id_t	EVE_MSG_PRINTSTAT	= 0xF001;

static void statistics_init(task_id_t task_id)
{
	
}

static cd_uint16_t statistics_process(task_id_t task_id, event_asb_t events)
{
    if(events & SYS_EVE_MSG)
    {
        cd_msg_bdy_t *msg;
        msg = (cd_msg_bdy_t *)cd_msg_receive(task_id);

		if(EVE_MSG_PRINTSTAT == msg->hdr.event_id)
		{
			cd_uint16_t *count;
			count = (cd_uint16_t *)msg->data;
			LOG_D("TASK_STAT: %d\r\n", *count);
			LOG_D("MAX USED SIZE is %d\r\n\r\n", heapTOTAL_HEAP_SIZE - xHeapGetMinimumEverFreeSize());
		}

		cd_msg_free(msg);

        return events ^ SYS_EVE_MSG;
    }

    return 0;
}

static cd_task_ops_t stat_ops = {statistics_init, statistics_process};
static void stat_task(void)
{
	cd_task_create(&stat_ops, TASK_STAT);
}
INIT_APP_EXPORT(stat_task);
