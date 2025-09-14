#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "misc.h"

volatile uint8_t led_state = 0;

void GPIO_Init_LED(void)
{
    GPIO_InitTypeDef gpio;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    gpio.GPIO_Pin = GPIO_Pin_5;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOA, &gpio);
}

void Timer3_Init(void)
{
    TIM_TimeBaseInitTypeDef tim;
    NVIC_InitTypeDef nvic;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

    tim.TIM_Prescaler = 7199;            
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period = 4999;               
    tim.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM3, &tim);

    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

    nvic.NVIC_IRQChannel = TIM3_IRQn;
    nvic.NVIC_IRQChannelPreemptionPriority = 0;
    nvic.NVIC_IRQChannelSubPriority = 0;
    nvic.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&nvic);

    TIM_Cmd(TIM3, ENABLE);
}

void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
        led_state =! led_state;
				GPIO_WriteBit(GPIOA, GPIO_Pin_5, (BitAction) led_state);
    }
}

int main(void)
{
    GPIO_Init_LED();
    Timer3_Init();

    while(1)
    {
			
    }
}
