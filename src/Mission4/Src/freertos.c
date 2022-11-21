/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */
extern uint32_t aRGB;
extern TIM_HandleTypeDef htim5;
static uint8_t alpha;
/* USER CODE END Variables */
osThreadId led_bHandle;
osThreadId led_gHandle;
osThreadId led_rHandle;
osThreadId recieveHandle;
osThreadId emissionHandle;

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void const * argument);
void led_b_task(void const * argument);
void led_g_task(void const * argument);
void led_r_task(void const * argument);
void recieve_task(void const * argument);
void emission_task(void const * argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/* GetIdleTaskMemory prototype (linked to static allocation support) */
void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize );

/* USER CODE BEGIN GET_IDLE_TASK_MEMORY */
static StaticTask_t xIdleTaskTCBBuffer;
static StackType_t xIdleStack[configMINIMAL_STACK_SIZE];

void vApplicationGetIdleTaskMemory( StaticTask_t **ppxIdleTaskTCBBuffer, StackType_t **ppxIdleTaskStackBuffer, uint32_t *pulIdleTaskStackSize )
{
  *ppxIdleTaskTCBBuffer = &xIdleTaskTCBBuffer;
  *ppxIdleTaskStackBuffer = &xIdleStack[0];
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  /* place for user code */
}
/* USER CODE END GET_IDLE_TASK_MEMORY */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */

  /* definition and creation of led_b */
  osThreadDef(led_b, led_b_task, osPriorityNormal, 0, 128);
  led_bHandle = osThreadCreate(osThread(led_b), NULL);

  /* definition and creation of led_g */
  osThreadDef(led_g, led_g_task, osPriorityNormal, 0, 128);
  led_gHandle = osThreadCreate(osThread(led_g), NULL);

  /* definition and creation of led_r */
  osThreadDef(led_r, led_r_task, osPriorityNormal, 0, 128);
  led_rHandle = osThreadCreate(osThread(led_r), NULL);

  /* definition and creation of recieve */
  osThreadDef(recieve, recieve_task, osPriorityAboveNormal, 0, 128);
  recieveHandle = osThreadCreate(osThread(recieve), NULL);

  /* definition and creation of emission */
  osThreadDef(emission, emission_task, osPriorityHigh, 0, 128);
  emissionHandle = osThreadCreate(osThread(emission), NULL);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */

/* USER CODE BEGIN Header_led_b_task */
/**
* @brief Function implementing the led_b thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_b_task */
void led_b_task(void const * argument)
{
  /* USER CODE BEGIN led_b_task */
  /* Infinite loop */
  static uint16_t blue;
  while(1)
  {
    blue = ((aRGB & 0x000000FF) >> 0) * alpha;
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_1,blue);
  }
  /* USER CODE END led_b_task */
}

/* USER CODE BEGIN Header_led_g_tasl */
/**
* @brief Function implementing the led_g thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_g_tasl */
void led_g_task(void const * argument)
{
  /* USER CODE BEGIN led_g_task */
  /* Infinite loop */
  static uint16_t green;
  while(1)
  {
    green = ((aRGB & 0x0000FF00) >> 8) * alpha;
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_2,green);
  }
  /* USER CODE END led_g_task */
}

/* USER CODE BEGIN Header_led_r_task */
/**
* @brief Function implementing the led_r thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_led_r_task */
void led_r_task(void const * argument)
{
  /* USER CODE BEGIN led_r_task */
  /* Infinite loop */
  static uint16_t red;
  while(1)
  {
    red = ((aRGB & 0x00FF0000) >> 16) * alpha;
    __HAL_TIM_SetCompare(&htim5,TIM_CHANNEL_3,red);
    vTaskResume(recieveHandle);
  }
  /* USER CODE END led_r_task */
}

/* USER CODE BEGIN Header_recieve_task */
/**
* @brief Function implementing the recieve thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_recieve_task */
void recieve_task(void const * argument)
{
  /* USER CODE BEGIN recieve_task */
  /* Infinite loop */
  while(1)
  {
    if(HAL_GPIO_ReadPin(REC_GPIO_Port,REC_Pin))
    {
      alpha =(aRGB & 0xFF000000) >> 24;
      while(alpha<=255)
      {
        vTaskSuspend(recieveHandle);
        alpha++;
      }
      while(alpha>=0)
      {
        vTaskSuspend(recieveHandle);
        alpha--;
      }
      vTaskResume(emissionHandle);
    }
  }
  /* USER CODE END recieve_task */
}

/* USER CODE BEGIN Header_emission_task */
/**
* @brief Function implementing the emission thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_emission_task */
void emission_task(void const * argument)
{
  /* USER CODE BEGIN emission_task */
  /* Infinite loop */
  while(1)
  {
    HAL_GPIO_WritePin(EMI_GPIO_Port,EMI_Pin,GPIO_PIN_RESET);//发送红外信号

    vTaskSuspend(NULL);

    HAL_Delay(3000);
    HAL_GPIO_WritePin(EMI_GPIO_Port,EMI_Pin,GPIO_PIN_SET);
    HAL_Delay(3000);
  }
  /* USER CODE END emission_task */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */
