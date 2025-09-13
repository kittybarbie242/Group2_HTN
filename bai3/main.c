#include "stm32f10x.h"

void GPIO_Config(void);
void EXTI_Config(void);
void delay_ms(uint32_t time);

volatile uint8_t led2_state = 0;

int main(void)
{
    GPIO_Config();
    EXTI_Config();

    while (1)
    {
        
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        delay_ms(500);
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        delay_ms(500);
    }
}


void GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);
	
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
    GPIO_Init(GPIOA, &GPIO_InitStructure);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}


void EXTI_Config(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource0);


    EXTI_InitStructure.EXTI_Line = EXTI_Line0;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
				led2_state =! led2_state;
				GPIO_WriteBit(GPIOA, GPIO_Pin_1, (BitAction) led2_state);

        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void delay_ms(uint32_t time)
{
    uint32_t i, j;
    for (i = 0; i < time; i++)
    {
        for (j = 0; j < 0x2AFF; j++);
    }
}

