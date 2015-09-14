#include <stm32f4xx_hal.h>
#include <stdbool.h>
#include "fan.h"

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

static void led_green(bool on)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_12, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}
static void led_orange(bool on)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_13, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void led_red(bool on)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_14, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void led_blue(bool on)
{
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, on ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

static void timer1_init(void)
{
	TIM_HandleTypeDef timer;
	TIM_ClockConfigTypeDef clock_config;
	TIM_MasterConfigTypeDef master_config;
	TIM_IC_InitTypeDef ic_config;

	timer.Instance = TIM1;
	timer.Init.Prescaler = 0;
	timer.Init.CounterMode = TIM_COUNTERMODE_UP;
	timer.Init.Period = 0;
	timer.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	timer.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&timer);

	clock_config.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
	HAL_TIM_ConfigClockSource(&timer, &clock_config);

	HAL_TIM_IC_Init(&timer);

	master_config.MasterOutputTrigger = TIM_TRGO_RESET;
	master_config.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
	HAL_TIMEx_MasterConfigSynchronization(&timer, &master_config);

	ic_config.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
	ic_config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	ic_config.ICPrescaler = TIM_ICPSC_DIV1;
	ic_config.ICFilter = 0;
	HAL_TIM_IC_ConfigChannel(&timer, &ic_config, TIM_CHANNEL_1);

	HAL_TIM_IC_ConfigChannel(&timer, &ic_config, TIM_CHANNEL_2);

	HAL_TIM_IC_ConfigChannel(&timer, &ic_config, TIM_CHANNEL_3);

	HAL_TIM_IC_ConfigChannel(&timer, &ic_config, TIM_CHANNEL_4);	
}

static void rcc_init(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_ini;

	__PWR_CLK_ENABLE();
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_ON;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLSOURCE_HSE; 
	osc_init.PLL.PLLM = 8;
	osc_init.PLL.PLLN = 336;
	osc_init.PLL.PLLP = RCC_PLLP_DIV2;
	osc_init.PLL.PLLQ = 7;
	HAL_RCC_OscConfig(&osc_init);

	clk_ini.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_ini.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	clk_ini.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_ini.APB1CLKDivider = RCC_HCLK_DIV4;
	clk_ini.APB2CLKDivider = RCC_HCLK_DIV2;

	HAL_RCC_ClockConfig(&clk_ini, FLASH_LATENCY_5);
}


static void pin_init(void)
{
	GPIO_InitTypeDef init;
	__HAL_RCC_TIM4_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	init.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
	init.Mode = GPIO_MODE_AF_PP;
	init.Pull = GPIO_NOPULL;
	init.Speed = GPIO_SPEED_HIGH;
	init.Alternate = GPIO_AF2_TIM4;
	HAL_GPIO_Init(GPIOD, &init);
}

int main(void)
{
	rcc_init();
	HAL_Init();
	pin_init();
	fan_set_period(25000);
	fan_set_pwm(FAN_0, 0.1);
	fan_set_pwm(FAN_1, 0.3);
	fan_set_pwm(FAN_2, 0.5);
	fan_set_pwm(FAN_3, 0.7);

	while(1);
}

