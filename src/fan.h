void fan_set_period(float desired_freq_hz);
void fan_set_pwm(uint32_t fan, float compare);

#define FAN_0	TIM_CHANNEL_1
#define FAN_1	TIM_CHANNEL_2
#define FAN_2	TIM_CHANNEL_3
#define FAN_3	TIM_CHANNEL_4
