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
#include "usart.h"
#include <stdio.h>
#include "string.h"
#include "vofa.h"
#include "motor.h"
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

/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for LED */
osThreadId_t LEDHandle;
const osThreadAttr_t LED_attributes = {
  .name = "LED",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for myTimer */
osTimerId_t myTimerHandle;
const osTimerAttr_t myTimer_attributes = {
  .name = "myTimer"
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);
void LED_Task(void *argument);
void myTimer_Callback(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

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

  /* Create the timer(s) */
  /* creation of myTimer */
  myTimerHandle = osTimerNew(myTimer_Callback, osTimerPeriodic, NULL, &myTimer_attributes);

  /* USER CODE BEGIN RTOS_TIMERS */
    /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
    /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* creation of LED */
  LEDHandle = osThreadNew(LED_Task, NULL, &LED_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
    /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
    /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
    osTimerStart(myTimerHandle, 1);
    /* Infinite loop */
    for (;;) {

        osDelay(1);
    }
  /* USER CODE END StartDefaultTask */
}

/* USER CODE BEGIN Header_LED_Task */
/**
* @brief Function implementing the LED thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_LED_Task */
void LED_Task(void *argument)
{
  /* USER CODE BEGIN LED_Task */

    /* Infinite loop */
    for (;;) {
        if (recv_end_flag == 1) //接收完成标志
        {
            float value;
            value = VofaModifyValue(rx_buffer, rx_len);
            //printf("%.2f\r\n", value);

            //HAL_UART_Transmit_DMA(&huart8, rx_buffer, rx_len);
            rx_len = 0;        //清除计数
            recv_end_flag = 0; //清除接收结束标志位
            memset(rx_buffer, 0, rx_len);
            HAL_UART_Receive_DMA(&huart8, rx_buffer, BUFFER_SIZE); //重新打开DMA接收
        }
        osDelay(1);
//      HAL_GPIO_TogglePin(LED_R_GPIO_Port, LED_R_Pin);
//      osDelay(200);
        //printf("usart ok\r\n");
    }
  /* USER CODE END LED_Task */
}

/* myTimer_Callback function */
void myTimer_Callback(void *argument)
{
  /* USER CODE BEGIN myTimer_Callback */

    static uint32_t run_times;

    if (run_times % 50 == 0)
    {
        //MOTORC_SetVel(50);
//			MOTORC_SetVelWithRamp(50);
//            MOTORC_SetAngle(360);
    }
    if (run_times == 500)
    {
        HAL_GPIO_TogglePin(LED_G_GPIO_Port, LED_G_Pin);
        run_times = 0;
    }
    MOTORC_InfoUpdateLoop();//1ms更新一次
    if (run_times % 10 == 0) {
        MOTORC_Test();
    }

    ++run_times;

  /* USER CODE END myTimer_Callback */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

