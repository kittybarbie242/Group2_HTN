#include "main.h"
#include "FreeRTOS.h"
#include "task.h"

typedef struct {
    GPIO_TypeDef *Port;
    uint16_t Pin;
    float Frequency;
} LED_Params_t;

void LED_Task(void *pvParameters);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();

    static LED_Params_t led1 = {GPIOA, GPIO_PIN_0, 3.0f};
    static LED_Params_t led2 = {GPIOA, GPIO_PIN_1, 10.0f};
    static LED_Params_t led3 = {GPIOA, GPIO_PIN_2, 25.0f};

    xTaskCreate(LED_Task, "LED1", 128, &led1, 1, NULL);
    xTaskCreate(LED_Task, "LED2", 128, &led2, 1, NULL);
    xTaskCreate(LED_Task, "LED3", 128, &led3, 1, NULL);

    vTaskStartScheduler();

    while (1)
    {
    }
}

void LED_Task(void *pvParameters)
{
    LED_Params_t *params = (LED_Params_t *)pvParameters;
    float period_ms = (1000.0f / params->Frequency) / 2.0f;

    for (;;)
    {
        HAL_GPIO_TogglePin(params->Port, params->Pin);
        vTaskDelay(pdMS_TO_TICKS(period_ms));
    }
}

static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    __HAL_RCC_GPIOA_CLK_ENABLE();

    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void SystemClock_Config(void)
{
    RCC_OscInitTypeDef RCC_OscInitStruct = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
    HAL_RCC_OscConfig(&RCC_OscInitStruct);

    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                  RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
    HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2);
}

void Error_Handler(void)
{
    __disable_irq();
    while (1);
}
