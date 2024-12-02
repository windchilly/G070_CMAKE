#include "cdos.h"
#include "adc.h"

static adc_device_t *adc_head = CD_NULL;

cd_err_t cd_adc_device_register(adc_device_t *dev, char *name)
{
	CD_ASSERT(dev);
	CD_ASSERT(name);

	if(CD_NULL == adc_head)
	{
		adc_head = dev;
		dev->next = CD_NULL;
	}
	else
	{
		adc_device_t *iter = adc_head;
		while(iter->next != CD_NULL){iter = iter->next;}
		iter->next = dev;
		dev->next = CD_NULL;
	}

	cd_strncpy(dev->name, name, CD_DEV_NAME_MAX);

	return CD_SUCCESS;
}

adc_device_t *cd_adc_device_find(char *name)
{
	CD_ASSERT(name);

	adc_device_t *iter = adc_head;

	while(iter != CD_NULL)
	{
		if(0 == cd_strcmp(name, iter->name))
		{
			return iter;
		}
		iter = iter->next;
	}

	return CD_NULL;
}

cd_uint32_t cd_adc_device_read(adc_device_t *dev)
{
	CD_ASSERT(dev);

	if(dev->ops.read != CD_NULL)
	{
		return dev->ops.read(dev);
	}

	return CD_ERROR;
}

void list_adc(void)
{
	adc_device_t *iter = adc_head;

	while(iter != CD_NULL)
	{
		cd_printf("%s", iter->name);
		iter = iter->next;
	}
}
SHELL_EXPORT_CMD(SHELL_CMD_PERMISSION(0)|SHELL_CMD_TYPE(SHELL_TYPE_CMD_FUNC)
|SHELL_CMD_DISABLE_RETURN, list_adc, list_adc, system);
