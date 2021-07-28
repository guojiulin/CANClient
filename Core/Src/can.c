/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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

/* Includes ------------------------------------------------------------------*/
#include "can.h"

/* USER CODE BEGIN 0 */
CAN_HandleTypeDef hcan;
CAN_FilterTypeDef  CAN_FilterInitStructure;


CAN_TxHeaderTypeDef   TxHeader;
CAN_RxHeaderTypeDef   RxHeader;
uint32_t               id=0x18013F01;
uint8_t               TxData[8] = {0};
uint8_t               RxData[8] = {0};
uint32_t              Data[64] = {0};
uint32_t              TxMailbox;
/* USER CODE END 0 */


/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 9;
  hcan.Init.Mode = CAN_MODE_NORMAL;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_8TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_7TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = ENABLE;
  hcan.Init.AutoWakeUp = ENABLE;
  hcan.Init.AutoRetransmission = DISABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
	 CAN_FilterInitStructure.FilterBank=0;//指定过滤器为1
  CAN_FilterInitStructure.FilterMode=CAN_FILTERMODE_IDMASK;//指定过滤器为标识符屏蔽位模式
  CAN_FilterInitStructure.FilterScale=CAN_FILTERSCALE_32BIT;//过滤器位宽为32位
  CAN_FilterInitStructure.FilterIdHigh=0x0000;// 过滤器标识符的高16位值
  CAN_FilterInitStructure.FilterIdLow=0x0000;//	 过滤器标识符的低16位值
  CAN_FilterInitStructure.FilterMaskIdHigh=0x0000;//过滤器屏蔽标识符的高16位值
  CAN_FilterInitStructure.FilterMaskIdLow=0x0000;//	过滤器屏蔽标识符的低16位值
  CAN_FilterInitStructure.FilterFIFOAssignment=CAN_RX_FIFO0;// 设定了指向过滤器的FIFO为0
  CAN_FilterInitStructure.FilterActivation=ENABLE;// 使能过滤器
	CAN_FilterInitStructure.SlaveStartFilterBank = 14;
	

	if (HAL_CAN_ConfigFilter(&hcan, &CAN_FilterInitStructure) != HAL_OK)
  {
    /* Filter configuration Error */
    Error_Handler();
  }
 
  /* Start the CAN peripheral */
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {
    /* Start Error */
    Error_Handler();
  }
 
  /* Activate CAN RX notification */
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_RX_FIFO0_MSG_PENDING) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }
	
  /* Activate CAN TX notification */
  if (HAL_CAN_ActivateNotification(&hcan, CAN_IT_TX_MAILBOX_EMPTY) != HAL_OK)
  {
    /* Notification Error */
    Error_Handler();
  }
	
//  TxHeader.ExtId = 0x1401013F;
//  TxHeader.RTR = CAN_RTR_DATA;
//  TxHeader.IDE = CAN_ID_EXT;
//  TxHeader.DLC = 8;
//  TxHeader.TransmitGlobalTime = DISABLE;


  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
void CAN_Send_Message()
{   

  TxHeader.ExtId = 0x18013F01;
  TxHeader.RTR = CAN_RTR_DATA;
  TxHeader.IDE = CAN_ID_EXT;
  TxHeader.DLC = 8;
  TxHeader.TransmitGlobalTime = DISABLE;


		TxData[0]=RxData[0];
	  TxData[1]=RxData[1];
		TxData[2]=RxData[2];
		TxData[3]=RxData[3];
		TxData[4]=RxData[4];
		TxData[5]=RxData[5];
		TxData[6]=RxData[6];
		TxData[7]=RxData[7];
		HAL_CAN_AddTxMessage(&hcan,&TxHeader,TxData,&TxMailbox);

	
}


void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *CanHandle)
{
	RxHeader.ExtId = 0x00;
  RxHeader.RTR = CAN_RTR_DATA;
  RxHeader.IDE = CAN_ID_EXT;
  RxHeader.DLC = 8;
	HAL_CAN_GetRxMessage(CanHandle, CAN_RX_FIFO0, &RxHeader, RxData);
	if(RxHeader.ExtId==0x1401013F)
	{
		dataprocess();
	}
 
}

void dataprocess()
{
	unsigned char loop,count;
	for(loop=0;loop<8;loop++)
	{
		Data[0+8*loop]=RxData[loop]>>0&1;
		Data[1+8*loop]=RxData[loop]>>1&1;
		Data[2+8*loop]=RxData[loop]>>2&1;
		Data[3+8*loop]=RxData[loop]>>3&1;
		Data[4+8*loop]=RxData[loop]>>4&1;
		Data[5+8*loop]=RxData[loop]>>5&1;
		Data[6+8*loop]=RxData[loop]>>6&1;
		Data[7+8*loop]=RxData[loop]>>7&1;
	}
			if(Data[0]==1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_RESET);
		}
		 if(Data[0]==0)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET);
		}
		 if(Data[63]==1)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_RESET);
		}
		 if(Data[63]==0)
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_15, GPIO_PIN_SET);
		}
//	for(count=0;count<64;count++)
//	{

//		if(mask & Data[count])
//		{
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);
//		}
//		else
//		{
//			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_SET);
//		}
//	}
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
