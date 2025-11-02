#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "stm32f10x_usart.h"
#include "task.h"
#include "semphr.h"
#include <stdio.h>

#define BIT_TASKA   (1 << 0)
#define BIT_TASKB   (1 << 1)
#define BIT_TASKC   (1 << 2)

EventGroupHandle_t xEventGroup;
SemaphoreHandle_t xUartMutex;

void RCC_Config(void);
void USART1_Config(void);
void UART1_SendString(char *str);
void TaskMain(void *pvParameters);
void TaskA(void *pvParameters);
void TaskB(void *pvParameters);
void TaskC(void *pvParameters);

int main(void)
{
    RCC_Config();
    USART1_Config();

    xEventGroup = xEventGroupCreate();
    xUartMutex = xSemaphoreCreateMutex();

    xTaskCreate(TaskMain, "Main", 128, NULL, 3, NULL);
    xTaskCreate(TaskA, "A", 128, NULL, 2, NULL);
    xTaskCreate(TaskB, "B", 128, NULL, 2, NULL);
    xTaskCreate(TaskC, "C", 128, NULL, 2, NULL);

    vTaskStartScheduler();

    while(1);
}

void TaskMain(void *pvParameters)
{
    while (1)
    {
        xSemaphoreTake(xUartMutex, portMAX_DELAY);
        UART1_SendString("Kich hoat tung Task \r\n");
        xSemaphoreGive(xUartMutex);

        xEventGroupSetBits(xEventGroup, BIT_TASKA);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xEventGroupSetBits(xEventGroup, BIT_TASKB);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xEventGroupSetBits(xEventGroup, BIT_TASKC);
        vTaskDelay(pdMS_TO_TICKS(1000));

        xSemaphoreTake(xUartMutex, portMAX_DELAY);
        UART1_SendString("Kich hoat dong thoi 3 Task \r\n");
        xSemaphoreGive(xUartMutex);

        xEventGroupSetBits(xEventGroup, BIT_TASKA | BIT_TASKB | BIT_TASKC);
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

void TaskA(void *pvParameters)
{
    for(;;)
    {
        xEventGroupWaitBits(xEventGroup, BIT_TASKA, pdTRUE, pdFALSE, portMAX_DELAY);
        xSemaphoreTake(xUartMutex, portMAX_DELAY);
        UART1_SendString("Task A is running\r\n");
        xSemaphoreGive(xUartMutex);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void TaskB(void *pvParameters)
{
    for(;;)
    {
        xEventGroupWaitBits(xEventGroup, BIT_TASKB, pdTRUE, pdFALSE, portMAX_DELAY);
        xSemaphoreTake(xUartMutex, portMAX_DELAY);
        UART1_SendString("Task B is running\r\n");
        xSemaphoreGive(xUartMutex);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void TaskC(void *pvParameters)
{
    for(;;)
    {
        xEventGroupWaitBits(xEventGroup, BIT_TASKC, pdTRUE, pdFALSE, portMAX_DELAY);
        xSemaphoreTake(xUartMutex, portMAX_DELAY);
        UART1_SendString("Task C is running\r\n");
        xSemaphoreGive(xUartMutex);
        vTaskDelay(pdMS_TO_TICKS(200));
    }
}

void RCC_Config(void)
{
    SystemInit();
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
}

void USART1_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART1, &USART_InitStructure);

    USART_Cmd(USART1, ENABLE);
}

void UART1_SendString(char *str)
{
    while (*str)
    {
        while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET);
        USART_SendData(USART1, *str++);
    }
}
