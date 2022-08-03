/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <string.h>
#include "stdio.h"
#include <stdbool.h>
#include "ADS1298.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
#define ITM_Port32(n)   (*((volatile unsigned long *)(0xE0000000+4*n)))

#define size_buffer 10
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

SPI_HandleTypeDef hspi5;
DMA_HandleTypeDef hdma_spi5_rx;
DMA_HandleTypeDef hdma_spi5_tx;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
bool verbose = true;		// torna on/off o envio de msgs para monitor serial
bool pressbotao = false;
volatile uint32_t timerPowerDown = 0;
extern bool intDRDY;
extern int16_t channelData [16];
uint8_t bit = 0b10110110;
uint8_t bits = 0;
char msg[10];


uint8_t TxBuf[19]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t ADSData [19];
int16_t SWDviwe;
int CanalData[8];
int DerivacoesECG[12];

//vairiaveis para medição do erro de offset
char errorOffset[150];


HAL_StatusTypeDef res;
//Variaveis para sensor de temperatura, e fitro de média móvel
int mediaf=0; //variavél apenas para mostrar a media no SWV
int16_t buffer_circular[size_buffer];
char inicio = 0, fim = 0;
int media=0;
float temp;
char contador=0;


/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_DMA_Init(void);
static void MX_SPI5_Init(void);
/* USER CODE BEGIN PFP */

void USB_Print(char* string);
void USB_SendBits(uint8_t b);
//void USB_SendNumber(int32_t num);

void USART_Send(uint8_t* cmd);
//void USART_Send2Byte(uint8_t* fb);
void USART_Send4Byte(uint8_t* fb);

//void USB_SendByte( uint8_t* fb);
void USB_Send2Byte(uint8_t* fb);
void USB_Sendfloat(float fb);
void USB_Send16bit(int16_t fb);
//void USB_Send3Byte(uint8_t* fb);
void USB_Send4Byte(uint8_t* fb);

//uint8_t transferSPI(uint8_t send);
uint8_t transferSPI(uint8_t send);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
uint8_t Read_ADS1198_DMA(void){

	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);
	HAL_GPIO_WritePin( ADS_CS_GPIO_Port  , ADS_CS_Pin, GPIO_PIN_RESET);


	res = HAL_SPI_TransmitReceive_DMA(&hspi5, TxBuf, ADSData, 19);
		if (res == HAL_OK){

		return 1;
	}else{

		HAL_GPIO_WritePin( ADS_CS_GPIO_Port  , ADS_CS_Pin, GPIO_PIN_SET);
		return 0;
	}
}

void Read_ADS1198_DMA_Complete(void){

	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET);

	for(uint8_t i=0; i<8; i++){
		channelData[i] = (int16_t) ((ADSData[(2*i)+3] << 8) | ADSData[((2*i)+3)+1]); //separação dos canais do ADS com dados crus
		CanalData[i] = (int) ((channelData[i]*4000000)/32767); // transformar em  nano-volts
	}

	/*==========================DERIVAÇÕES ECG begin=============================*/
	DerivacoesECG[0] = channelData[1]; //I
	DerivacoesECG[1] = channelData[2]; //II
	DerivacoesECG[2] = channelData[2]-channelData[1]; //III
	DerivacoesECG[3] = (-channelData[1]-channelData[2])/2; //aVR
	DerivacoesECG[4] = channelData[1]-channelData[2]/2; //aVL
	DerivacoesECG[5] = channelData[2]-channelData[1]/2; //aVF
	DerivacoesECG[6] = channelData[7]; //V1
	DerivacoesECG[7] = channelData[3]; //V2
	DerivacoesECG[8] = channelData[4]; //V3
	DerivacoesECG[9] = channelData[5]; //V4
 	DerivacoesECG[10] = channelData[6]; //V5
	DerivacoesECG[11] = channelData[0]; //V6
	/*==========================DERIVAÇÕES ECG begin=============================*/


//	/*==========================ANALISE RUIDO OFFSET begin=============================*/
//	sprintf(errorOffset, "%d %d %d %d %d %d %d %d\r\n", CanalData[0], CanalData[1], CanalData[2], CanalData[3], CanalData[4], CanalData[5], CanalData[6], CanalData[7]);
//	USB_Print(errorOffset);
//	bufclear(errorOffset);
//	/*==========================ANALISE RUIDO OFFSET end=============================*/


	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
	//xQueueSend(messageQ, &channelData[1], 0);

}
void bufclear(char *buf){

	uint8_t len = strlen (buf);
	for(int i=0; i<len; i++){
		buf[i] = '\0';
	}
}

int _write(int file, char *ptr, int len)
{
	int DataIdx;

	for(DataIdx=0; DataIdx<len; DataIdx++){
		ITM_SendChar(*ptr++);
	}
	return len;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  ITM_Port32(31) = 1;

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_DMA_Init();
  MX_SPI5_Init();
  /* USER CODE BEGIN 2 */
  printf("GPIO inicializado \r\n");
  ITM_Port32(31) = 2;
  //USB_SendBits(bit);
  //ADS_SDATAC();
  ADS_Init();

  // modo continuo
  ADS_RDATAC();

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if(intDRDY){

		  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_7);
	  			//startAcq = true;
	  			intDRDY = false;

	  	//modo de disparo único
	  			//ADS_RDATA();
	  			//ADS_SendData();
	  			//ADS_START();

		//modo contínuo
	  		//	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET);

			ADS_updateChannelData();
			//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET);
			ADS_SendData();


		/* Temperatura e media movel
			if(inicio >= size_buffer) inicio = 0;

			if(inicio != fim){

				buffer_circular[(int) inicio] = channelData[1];
				media = 0;
				for(char z=0; z<size_buffer; z++){
					media += buffer_circular[(int) z];
				}
				media = media/size_buffer;
				mediaf = media;
				temp = (((media*36.62221137)-145300)/490) +25;

				if(contador == 50){
					USB_Sendfloat(temp);

					contador = 0;
				}else{
					contador++;
				}
			    fim++;
				if(fim>=size_buffer) fim=0;
			}
*/

	  		}

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 400;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV16;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI5_Init(void)
{

  /* USER CODE BEGIN SPI5_Init 0 */

  /* USER CODE END SPI5_Init 0 */

  /* USER CODE BEGIN SPI5_Init 1 */

  /* USER CODE END SPI5_Init 1 */
  /* SPI5 parameter configuration*/
  hspi5.Instance = SPI5;
  hspi5.Init.Mode = SPI_MODE_MASTER;
  hspi5.Init.Direction = SPI_DIRECTION_2LINES;
  hspi5.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi5.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi5.Init.CLKPhase = SPI_PHASE_2EDGE;
  hspi5.Init.NSS = SPI_NSS_SOFT;
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi5.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi5.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi5.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi5.Init.CRCPolynomial = 7;
  hspi5.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi5.Init.NSSPMode = SPI_NSS_PULSE_DISABLE;
  if (HAL_SPI_Init(&hspi5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI5_Init 2 */

  /* USER CODE END SPI5_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * Enable DMA controller clock
  */
static void MX_DMA_Init(void)
{

  /* DMA controller clock enable */
  __HAL_RCC_DMA2_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_12|PW_KEY_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, ADS_DAISY_Pin|GPIO_PIN_7, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_RESET_GPIO_Port, ADS_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_CS_GPIO_Port, ADS_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_CLKSEL_GPIO_Port, ADS_CLKSEL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PA12 PW_KEY_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_12|PW_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PA11 */
  GPIO_InitStruct.Pin = GPIO_PIN_11;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : ADS_DAISY_Pin PC7 ADS_RESET_Pin */
  GPIO_InitStruct.Pin = ADS_DAISY_Pin|GPIO_PIN_7|ADS_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS_DRDY_Pin */
  GPIO_InitStruct.Pin = ADS_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ADS_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PJ4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : Botao_Pin */
  GPIO_InitStruct.Pin = Botao_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(Botao_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PH6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOH, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS_CS_Pin */
  GPIO_InitStruct.Pin = ADS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS_CLKSEL_Pin */
  GPIO_InitStruct.Pin = ADS_CLKSEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS_CLKSEL_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PB15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */



uint8_t transferSPI(uint8_t send){
	uint8_t rx = 0x00;

	HAL_SPI_TransmitReceive(&hspi5, &send, &rx, sizeof(rx),0x1000);

	return rx;
}

//======== USB->USART =======
void USB_Print(char* string){

	uint8_t len = strlen (string);
	HAL_UART_Transmit(&huart1, (uint8_t *)string, len, 1000); //transmite em modo de bloqueio

}

void USART_Send(uint8_t* cmd){
	uint8_t* ptr = cmd;
	int size = 0;

	while(*cmd++)
		size++;

	//while( HAL_UART_Transmit(&huart3, ptr, size, 0x1000) != HAL_OK);
	HAL_UART_Transmit(&huart1, ptr, size, 0x1000);
}

void USART_Send4Byte(uint8_t* fb){
	//while( HAL_UART_Transmit(&huart3, fb, sizeof(int32_t), 0x1000) != HAL_OK);
	HAL_UART_Transmit(&huart1, fb, sizeof(int32_t), 0x1000);
	USB_Print("\r\n");
}

void USB_Send4Byte(uint8_t* fb){
	HAL_UART_Transmit(&huart1, fb, sizeof(int32_t), 0x1000);
	//while( HAL_UART_Transmit(&huart1, fb, sizeof(int32_t), 0x1000) != HAL_OK);
		USB_Print("\r\n");
}
void USB_Send2Byte(uint8_t* fb){
	HAL_UART_Transmit(&huart1, fb, sizeof(int16_t), 0x1000);
	//while( HAL_UART_Transmit(&huart1, fb, sizeof(int32_t), 0x1000) != HAL_OK);
		USB_Print("\r\n");
}
void USB_Send16bit(int16_t fb){

	sprintf(msg, "%d\r\n", fb);
	uint8_t len = strlen (msg);
	HAL_UART_Transmit(&huart1,(uint8_t *)msg, len, 0x1000);
	bufclear(msg);
}
void USB_Sendfloat(float fb){

		sprintf(msg, "%.2f\r\n", fb);
			uint8_t len = strlen (msg);
			HAL_UART_Transmit(&huart1,(uint8_t *)msg, len, 0x1000);
			bufclear(msg);



}


void USB_SendBits(uint8_t b){//recebe um conjunto de bits
	int _i = 0;
	uint8_t buf[8];

	//if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) == GPIO_PIN_SET )
		USB_Print("0b");

	for(_i = 7; _i >= 0; _i--){//aloca cada bit em uma posição do vertor buf
		if( (b >> _i) & 0x01)
			buf[7-_i] = 49;
		else
			buf[7-_i] = 48;
	}

		HAL_UART_Transmit(&huart1, buf, sizeof(buf), 1000);
}

/* --------------- Inetrrupts ------------------------------------------*/
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin){


	if(GPIO_Pin == GPIO_PIN_6){

		//inicio ++;  //media movel
		//intDRDY = true;   SE COMENTADO -> DMA SENDO USADO
		if(pressbotao){
		Read_ADS1198_DMA();
		}
	}

	if(GPIO_Pin == GPIO_PIN_0){
		pressbotao = !pressbotao;
	}
	/*
	if(GPIO_Pin == GPIO_PIN_0){

		if(pressbotao){
			pressbotao = !pressbotao;
			//HAL_GPIO_WritePin(PW_KEY_GPIO_Port, PW_KEY_Pin, GPIO_PIN_SET); //liga os circuitos internos do ADS
			HAL_Delay(10);



			//timerPowerDown = HAL_GetTick();
		}else{
			pressbotao = !pressbotao;
			//if( HAL_GetTick() - timerPowerDown > 1000)
			 //   HAL_GPIO_WritePin(PW_KEY_GPIO_Port, PW_KEY_Pin, GPIO_PIN_RESET); //desliga os circuitos internos do ADS
		}
	}
	*/
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){

	if(hspi->Instance == SPI5){

	Read_ADS1198_DMA_Complete();

	}


}



/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
