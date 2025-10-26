#include "stm32f10x.h"                 
#include "FreeRTOS.h"                   
#include "task.h"                       
#include "stm32f10x_usart.h"  

void Task1(void *pvParameters);
void Task2(void *pvParameters);
void UART_Config(void);


int main(){
	
	UART_Config();
	xTaskCreate(Task1, "Task1", 128, NULL, 1, NULL);
	xTaskCreate(Task2, "Task2", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();
	
	while(1){	
	}
	
}

void UART_Config(void){
	
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin			= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed		= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Mode		= GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin			= GPIO_Pin_10;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	USART_InitStructure.USART_BaudRate = 9600;
  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
  USART_InitStructure.USART_StopBits = USART_StopBits_1;
  USART_InitStructure.USART_Parity = USART_Parity_No;
  USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;
  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
}

void USART1_SendChar(char c){
	while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET); 
	USART_SendData(USART1, c);
}
void USART1_SendString(char *str){
	while(*str != NULL){
		USART1_SendChar(*str++);
	}
}


void Task1(void * pvParameters)
{
	while(1)
	{
		USART1_SendString("VVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVVV\n");
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}

void Task2(void * pvParameters)
{
	while(1)
	{
		USART1_SendString("NNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNNN\n");
		vTaskDelay(pdMS_TO_TICKS(100));
	}
}
