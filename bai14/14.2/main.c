//DELAY

#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GPIO_Config(void);
void Task_Blink(void *pvParameters);

int main(void)
{
    SystemInit();
    GPIO_Config(); 

    xTaskCreate(Task_Blink, "Blink", 128, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1);
}

void Task_Blink(void *pvParameters)
{
    while(1)
    {
        GPIOC->ODR ^= (1 << 13);
        
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);
}


//SLEEP MODE
/*
#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

void GPIO_Config(void);
void Task_Blink(void *pvParameters);

void vApplicationIdleHook(void)
{
    __WFI(); 
}

int main(void)
{
    SystemInit();
    GPIO_Config(); 

    xTaskCreate(Task_Blink, "Blink", 128, NULL, 1, NULL);
    vTaskStartScheduler();

    while(1);
}

void Task_Blink(void *pvParameters)
{
    while(1)
    {
        GPIOC->ODR ^= (1 << 13);
        
        // Khi task nay delay, IDLE task se chay
        // và IDLE Hook (__WFI) se duoc goi, dua CPU vào Sleep
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_13;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &gpio);
}
*/
