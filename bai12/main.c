#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <stdlib.h>

#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

typedef struct {
    uint32_t frequency;
    uint8_t  dutyCycle;
} LedConfig_t;

#define LED_ON()  GPIO_ResetBits(GPIOC, GPIO_Pin_13)
#define LED_OFF() GPIO_SetBits(GPIOC, GPIO_Pin_13)

QueueHandle_t xLedConfigQueue;

void GPIO_Init_SPL(void);
void vLedBlinkTask(void *pvParameters);
void vDataGenerateTask(void *pvParameters);

int main(void)
{
  GPIO_Init_SPL();

  xLedConfigQueue = xQueueCreate(5, sizeof(LedConfig_t));

  xTaskCreate(vLedBlinkTask, "LedBlink", 128, NULL, 1, NULL);
  xTaskCreate(vDataGenerateTask, "DataGenerate", 128, NULL, 1, NULL);

  vTaskStartScheduler();

  while (1)
  {
  }
}

void GPIO_Init_SPL(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void vLedBlinkTask(void *pvParameters)
{
    LedConfig_t currentConfig;
		currentConfig.frequency =1;
		currentConfig.dutyCycle =50;
    uint32_t period_ms, ton_ms, toff_ms;

    for (;;)
    {
        xQueueReceive(xLedConfigQueue, &currentConfig, pdMS_TO_TICKS(10));

        period_ms = 1000 / currentConfig.frequency;
        ton_ms = period_ms * currentConfig.dutyCycle / 100;
        toff_ms = period_ms - ton_ms;

        if (ton_ms > 0)
        {
            LED_ON();
            vTaskDelay(pdMS_TO_TICKS(ton_ms));
        }

        if (toff_ms > 0)
        {
            LED_OFF();
            vTaskDelay(pdMS_TO_TICKS(toff_ms));
        }
    }
}

void vDataGenerateTask(void *pvParameters)
{
    LedConfig_t newConfig;

    for (;;)
    {
        vTaskDelay(pdMS_TO_TICKS(2000));

        newConfig.frequency = (rand() % 10) + 1;
        newConfig.dutyCycle = (rand() % 81) + 10;

        xQueueSend(xLedConfigQueue, (void *)&newConfig, 0);
    }
}

