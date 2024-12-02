#include "cdos.h"
#include "stm32g0xx.h"
#include "adc.h"

static ADC_HandleTypeDef hadc1;
static adc_device_t adc[] = 
{
	{.param = &hadc1},
};

static void MX_ADC1_Init(void)
{
	ADC_ChannelConfTypeDef sConfig = {0};

	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV2;
	hadc1.Init.Resolution = ADC_RESOLUTION_12B;
	hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.LowPowerAutoPowerOff = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	hadc1.Init.NbrOfConversion = 1;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	hadc1.Init.DMAContinuousRequests = DISABLE;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.SamplingTimeCommon1 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.SamplingTimeCommon2 = ADC_SAMPLETIME_1CYCLE_5;
	hadc1.Init.OversamplingMode = DISABLE;
	hadc1.Init.TriggerFrequencyMode = ADC_TRIGGER_FREQ_HIGH;
	CD_ASSERT(HAL_ADC_Init(&hadc1) == HAL_OK);

	sConfig.Channel = ADC_CHANNEL_1;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLINGTIME_COMMON_1;
	CD_ASSERT(HAL_ADC_ConfigChannel(&hadc1, &sConfig) == HAL_OK);

	CD_ASSERT(HAL_ADCEx_Calibration_Start(&hadc1) == HAL_OK);
}

static cd_uint32_t adc_conv(adc_device_t *dev)
{
	cd_err_t res;
	ADC_HandleTypeDef *adc;

	adc = (ADC_HandleTypeDef *)dev->param;
	HAL_ADC_Start(adc);
	res = HAL_ADC_PollForConversion(adc, 50);
	CD_ASSERT(HAL_OK == res);

	if(HAL_IS_BIT_SET(HAL_ADC_GetState(adc), HAL_ADC_STATE_REG_EOC))
	{
		return HAL_ADC_GetValue(adc);
	}

	return CD_ERROR;
}

cd_err_t adc_control(adc_device_t *dev, cd_uint16_t cmd)
{
	
}

static adc_ops_t ops = {adc_conv, adc_control};

static void adc_hw_init(void)
{
	MX_ADC1_Init();

	for(cd_uint8_t i = 0; i < sizeof(adc) / sizeof(adc[0]); i++)
	{
		adc[i].ops = ops;
	}

	cd_adc_device_register(&adc[0], "adc1");
}
INIT_BOARD_EXPORT(adc_hw_init);
