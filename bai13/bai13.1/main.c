#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "stm32f10x_exti.h"

TaskHandle_t xBlinkLedTaskHandle = NULL;
TaskHandle_t xWarningTaskHandle = NULL;
SemaphoreHandle_t xWarningSemaphore = NULL;

void vBlinkLedTask(void *pvParameters)
{
    for(;;)
    {
        uint8_t state = GPIO_ReadOutputDataBit(GPIOC, GPIO_Pin_13);
        if (state == Bit_SET)
        {
            GPIO_ResetBits(GPIOC, GPIO_Pin_13);
        }
        else
        {
            GPIO_SetBits(GPIOC, GPIO_Pin_13);
        }
        
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void vWarningTask(void *pvParameters)
{
    for(;;)
    {
        if (xSemaphoreTake(xWarningSemaphore, portMAX_DELAY) == pdTRUE)
        {
            GPIO_SetBits(GPIOA, GPIO_Pin_5);
            vTaskDelay(pdMS_TO_TICKS(3000));
            GPIO_ResetBits(GPIOA, GPIO_Pin_5);
        }
    }
}

void EXTI0_IRQHandler(void)
{
    if (EXTI_GetITStatus(EXTI_Line0) != RESET)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        xSemaphoreGiveFromISR(xWarningSemaphore, &xHigherPriorityTaskWoken);
        
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
        
        EXTI_ClearITPendingBit(EXTI_Line0);
    }
}

void Config_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    EXTI_InitTypeDef EXTI_InitStruct;
    NVIC_InitTypeDef NVIC_InitStruct;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
		NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 
	
    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOC, &GPIO_InitStruct);


    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
    GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOA, &GPIO_InitStruct);


    GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);

    EXTI_InitStruct.EXTI_Line = EXTI_Line0;
    EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling; 
    EXTI_InitStruct.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruct);

    NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn; 
    NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 5; 
    NVIC_InitStruct.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);
}

int main(void)
{

    Config_Init();

    xWarningSemaphore = xSemaphoreCreateBinary();	

    xTaskCreate(vBlinkLedTask, "Blink Task", 128, NULL, 1, &xBlinkLedTaskHandle);
    xTaskCreate(vWarningTask, "Warning Task", 128, NULL, 2, &xWarningTaskHandle);

    vTaskStartScheduler();
    
    while (1)
    {
    }
}
