#include <stm32f4xx_hal.h>
#include "fan.h"

static TIM_HandleTypeDef s_timer;

void fan_set_period( uint32_t desired_freq_hz )
{
	TIM_ClockConfigTypeDef clock_config;
	TIM_MasterConfigTypeDef master_config;
	TIM_OC_InitTypeDef oc_config;
	uint32_t clk_int;
	
	s_timer.Instance = TIM4;
	s_timer.Init.Prescaler = 0;
	s_timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	s_timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	s_timer.Init.Period = (uint32_t)((float)HAL_RCC_GetPCLK2Freq() / (float)desired_freq_hz);
	
	HAL_TIM_PWM_Init(&s_timer);

	oc_config.OCMode = TIM_OCMODE_PWM1;
	oc_config.Pulse = 0;
	oc_config.OCPolarity = TIM_OCPOLARITY_HIGH;
	oc_config.OCFastMode = TIM_OCFAST_DISABLE;
	oc_config.OCIdleState = TIM_CCx_ENABLE;
	
	HAL_TIM_PWM_ConfigChannel(&s_timer, &oc_config, TIM_CHANNEL_1);
	HAL_TIM_PWM_ConfigChannel(&s_timer, &oc_config, TIM_CHANNEL_2);
	HAL_TIM_PWM_ConfigChannel(&s_timer, &oc_config, TIM_CHANNEL_3);
	HAL_TIM_PWM_ConfigChannel(&s_timer, &oc_config, TIM_CHANNEL_4);	
}


void fan_set_pwm( uint32_t fan, float compare)
{
//	__HAL_TIM_SET_COUNTER(&htim4, 0);
	__HAL_TIM_SET_COMPARE(&s_timer, fan, (uint16_t)(((float)s_timer.Init.Period)*compare) );   
//	__HAL_TIM_SET_AUTORELOAD(&htim4, 3000);
	HAL_TIM_PWM_Start(&s_timer, fan);
	
}