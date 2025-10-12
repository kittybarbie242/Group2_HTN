#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

void GPIO_Config(void);
void Task1(void *pvParameters);
void Task2(void *pvParameters);
void Task3(void *pvParameters);

int main(void) {
    SystemInit();
    GPIO_Config();

    xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
    xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
    xTaskCreate(Task3, "Task3", 128, NULL, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

void GPIO_Config(void) {
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &gpio);
}

void Task1(void *pvParameters) {
    while(1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_0);
        vTaskDelay(pdMS_TO_TICKS(167));
        GPIO_ResetBits(GPIOA, GPIO_Pin_0);
        vTaskDelay(pdMS_TO_TICKS(167));
    }
}

void Task2(void *pvParameters) {
    while(1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_1);
        vTaskDelay(pdMS_TO_TICKS(50));
        GPIO_ResetBits(GPIOA, GPIO_Pin_1);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void Task3(void *pvParameters) {
    while(1) {
        GPIO_SetBits(GPIOA, GPIO_Pin_2);
        vTaskDelay(pdMS_TO_TICKS(20));
        GPIO_ResetBits(GPIOA, GPIO_Pin_2);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}
