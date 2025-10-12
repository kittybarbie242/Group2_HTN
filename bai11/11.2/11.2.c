#include "stm32f10x.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    GPIO_TypeDef *port;
    uint16_t pin;
    uint32_t frequency;   
} LED_Config_t;

void GPIO_Config(void);
void LED_Task(void *pvParameters);

int main(void)
{
    GPIO_Config();

    static LED_Config_t led1 = {GPIOA, GPIO_Pin_0, 3};   
    static LED_Config_t led2 = {GPIOA, GPIO_Pin_1, 10};  
    static LED_Config_t led3 = {GPIOA, GPIO_Pin_2, 25};  

    xTaskCreate(LED_Task, "LED1", 128, &led1, 1, NULL);
    xTaskCreate(LED_Task, "LED2", 128, &led2, 1, NULL);
    xTaskCreate(LED_Task, "LED3", 128, &led3, 1, NULL);

    vTaskStartScheduler();

    while(1);
}

void GPIO_Config(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitTypeDef gpio;
    gpio.GPIO_Mode = GPIO_Mode_Out_PP;       
    gpio.GPIO_Speed = GPIO_Speed_2MHz;
    gpio.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2;
    GPIO_Init(GPIOA, &gpio);

    GPIO_ResetBits(GPIOA, GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2);
}

void LED_Task(void *pvParameters)
{
    LED_Config_t *cfg = (LED_Config_t *)pvParameters;
    TickType_t xLastWakeTime = xTaskGetTickCount();

    const TickType_t delay = pdMS_TO_TICKS(1000 / (cfg->frequency * 2));

    for(;;)
    {
        GPIO_WriteBit(cfg->port, cfg->pin, (BitAction)!GPIO_ReadOutputDataBit(cfg->port, cfg->pin));
        vTaskDelayUntil(&xLastWakeTime, delay);
    }
}
