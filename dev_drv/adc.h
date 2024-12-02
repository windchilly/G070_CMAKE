#ifndef _ADC_H_
#define _ADC_H_

#include "cdos.h"

struct _adc_device;
typedef struct _adc_ops
{
	cd_uint32_t (*read)(struct _adc_device *dev);
	cd_err_t (*control)(struct _adc_device *dev, cd_uint16_t cmd);
}adc_ops_t;

typedef struct _adc_device
{
	struct _adc_device *next;

	char name[CD_DEV_NAME_MAX];
	adc_ops_t ops;
	void *param;
}adc_device_t;

adc_device_t *cd_adc_device_find(char *name);
cd_err_t cd_adc_device_control(adc_device_t *dev, cd_uint16_t cmd);
cd_uint32_t cd_adc_device_read(adc_device_t *dev);

cd_err_t cd_adc_device_register(adc_device_t *dev, char *name);

#endif
