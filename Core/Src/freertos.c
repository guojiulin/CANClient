/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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

/* USER CODE END Variables */
/* Definitions for canTask */
osThreadId_t canTaskHandle;
const osThreadAttr_t canTask_attributes = {
  .name = "canTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CPUTask */
osThreadId_t CPUTaskHandle;
const osThreadAttr_t CPUTask_attributes = {
  .name = "CPUTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void can_Task(void *argument);
void CPU_Task(void *argument);

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

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of canTask */
  canTaskHandle = osThreadNew(can_Task, NULL, &canTask_attributes);

  /* creation of CPUTask */
  CPUTaskHandle = osThreadNew(CPU_Task, NULL, &CPUTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_can_Task */
/**
  * @brief  Function implementing the canTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_can_Task */
void can_Task(void *argument)
{
  /* USER CODE BEGIN can_Task */
  /* Infinite loop */
  for(;;)
  {
		CAN_Send_Message();
    osDelay(1);
		
  }
  /* USER CODE END can_Task */
}

/* USER CODE BEGIN Header_CPU_Task */
/**
* @brief Function implementing the CPUTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_CPU_Task */
void CPU_Task(void *argument)
{
  /* USER CODE BEGIN CPU_Task */
  /* Infinite loop */
  for(;;)
  {
   uint8_t CPU_RunInfo[400];		//保存任务运行时间信息
    while (1)
    {
        memset(CPU_RunInfo,0,400);				//信息缓冲区清零
    
        vTaskList((char *)&CPU_RunInfo);  //获取任务运行时间信息
    
        printf("---------------------------------------------\r\n");
        printf("任务名      任务状态 优先级   剩余栈 任务序号\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n");
    
        memset(CPU_RunInfo,0,400);				//信息缓冲区清零
    
        vTaskGetRunTimeStats((char *)&CPU_RunInfo);
    
        printf("任务名       运行计数         利用率\r\n");
        printf("%s", CPU_RunInfo);
        printf("---------------------------------------------\r\n\n");
        vTaskDelay(1000);   /* 延时500个tick */		
    }
  }
	
}
  /* USER CODE END CPU_Task */

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */


/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
