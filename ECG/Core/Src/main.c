/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "fatfs.h"
#include "app_touchgfx.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../mx25l512/mx25l512.h"
#include "../otm8009a/otm8009a.h"
#include "ADS1298.h"
#include "ESP8266.h"
#include <string.h>
#include "stdio.h"
#include <stdbool.h>
#include <stdlib.h>
#include "queue.h"
#include "panTompkins.h"
#include <math.h>
#include "ecg_sd.h"
#include "Filtros.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


#define REFRESH_COUNT        1834
#define SDRAM_TIMEOUT                            ((uint32_t)0xFFFF)
#define SDRAM_MODEREG_BURST_LENGTH_1             ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_LENGTH_2             ((uint16_t)0x0001)
#define SDRAM_MODEREG_BURST_LENGTH_4             ((uint16_t)0x0002)
#define SDRAM_MODEREG_BURST_LENGTH_8             ((uint16_t)0x0004)
#define SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL      ((uint16_t)0x0000)
#define SDRAM_MODEREG_BURST_TYPE_INTERLEAVED     ((uint16_t)0x0008)
#define SDRAM_MODEREG_CAS_LATENCY_2              ((uint16_t)0x0020)
#define SDRAM_MODEREG_CAS_LATENCY_3              ((uint16_t)0x0030)
#define SDRAM_MODEREG_OPERATING_MODE_STANDARD    ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_PROGRAMMED ((uint16_t)0x0000)
#define SDRAM_MODEREG_WRITEBURST_MODE_SINGLE     ((uint16_t)0x0200)

/* QSPI Error codes */
#define QSPI_OK            ((uint8_t)0x00)
#define QSPI_ERROR         ((uint8_t)0x01)
#define QSPI_BUSY          ((uint8_t)0x02)
#define QSPI_NOT_SUPPORTED ((uint8_t)0x04)
#define QSPI_SUSPENDED     ((uint8_t)0x08)

/* DISPLAY */
#define LCD_ORIENTATION_LANDSCAPE 0x01


#define BufferLenPlotECG 250




/* ADS1198 */
enum InitADS1198 {ECG_12_Lead, Teste_interno, Teste_externo};
enum Derivacoes{dev_I, dev_II, dev_III, dev_aVR, dev_aVL, dev_aVF, dev_V1, dev_V2, dev_V3, dev_V4, dev_V5, dev_V6};

#define initADS1198 ECG_12_Lead


#define LenDataECGtoESP 6000 // multiplo de 300 amostras

#define SIZEECGEXP 30500

#define ADC_BUF_LEN 10
#define POST 0
#define GET 1
#define PATCH 2
#define PUT 3
#define DELETE 4
#define Observation 6
#define PATIENT 7

#define BUFF_LEN_AVGR_BATTERY 5


/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
 ADC_HandleTypeDef hadc3;

CRC_HandleTypeDef hcrc;

DMA2D_HandleTypeDef hdma2d;

DSI_HandleTypeDef hdsi;

I2C_HandleTypeDef hi2c4;

JPEG_HandleTypeDef hjpeg;

LTDC_HandleTypeDef hltdc;

QSPI_HandleTypeDef hqspi;

SD_HandleTypeDef hsd2;
DMA_HandleTypeDef hdma_sdmmc2_rx;
DMA_HandleTypeDef hdma_sdmmc2_tx;

SPI_HandleTypeDef hspi5;
DMA_HandleTypeDef hdma_spi5_rx;
DMA_HandleTypeDef hdma_spi5_tx;

UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;

SDRAM_HandleTypeDef hsdram1;

/* Definitions for DetectordePicos */
osThreadId_t DetectordePicosHandle;
const osThreadAttr_t DetectordePicos_attributes = {
  .name = "DetectordePicos",
  .stack_size = 6000 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for TouchGFXTask */
osThreadId_t TouchGFXTaskHandle;
const osThreadAttr_t TouchGFXTask_attributes = {
  .name = "TouchGFXTask",
  .stack_size = 4096 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for videoTask */
osThreadId_t videoTaskHandle;
const osThreadAttr_t videoTask_attributes = {
  .name = "videoTask",
  .stack_size = 1000 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for InitESPTask */
osThreadId_t InitESPTaskHandle;
const osThreadAttr_t InitESPTask_attributes = {
  .name = "InitESPTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for VerifErrorTask */
osThreadId_t VerifErrorTaskHandle;
const osThreadAttr_t VerifErrorTask_attributes = {
  .name = "VerifErrorTask",
  .stack_size = 5300 * 4,
  .priority = (osPriority_t) osPriorityAboveNormal,
};
/* Definitions for BuscarRedesTask */
osThreadId_t BuscarRedesTaskHandle;
const osThreadAttr_t BuscarRedesTask_attributes = {
  .name = "BuscarRedesTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for ConectWiFiTask */
osThreadId_t ConectWiFiTaskHandle;
const osThreadAttr_t ConectWiFiTask_attributes = {
  .name = "ConectWiFiTask",
  .stack_size = 2048 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for ECGtoESPTask */
osThreadId_t ECGtoESPTaskHandle;
const osThreadAttr_t ECGtoESPTask_attributes = {
  .name = "ECGtoESPTask",
  .stack_size = 8000 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for TrasmECGOnTask */
osThreadId_t TrasmECGOnTaskHandle;
const osThreadAttr_t TrasmECGOnTask_attributes = {
  .name = "TrasmECGOnTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for RealiseDataBPMT */
osThreadId_t RealiseDataBPMTHandle;
const osThreadAttr_t RealiseDataBPMT_attributes = {
  .name = "RealiseDataBPMT",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityBelowNormal,
};
/* Definitions for GetBatteryTask */
osThreadId_t GetBatteryTaskHandle;
const osThreadAttr_t GetBatteryTask_attributes = {
  .name = "GetBatteryTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* Definitions for WrittenSDTask */
osThreadId_t WrittenSDTaskHandle;
const osThreadAttr_t WrittenSDTask_attributes = {
  .name = "WrittenSDTask",
  .stack_size = 512 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for ExperimentoTask */
osThreadId_t ExperimentoTaskHandle;
const osThreadAttr_t ExperimentoTask_attributes = {
  .name = "ExperimentoTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityHigh,
};
/* Definitions for mainTask */
osThreadId_t mainTaskHandle;
const osThreadAttr_t mainTask_attributes = {
  .name = "mainTask",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow,
};
/* USER CODE BEGIN PV */


bool verbose = true;		// torna on/off o envio de msgs para monitor serial
volatile uint32_t timerPowerDown = 0;
extern bool intDRDY;
int16_t ECGchannelData[8];
int16_t ECG_12Dev[ECG_WRITE_DATA_SIZE];
char msg[10];
uint8_t ADSData [19];
uint8_t TxBuf[19]= {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
uint8_t DerivSelecionada=0;


char errorOffset[120];
float valorECG [BufferLenPlotECG];
uint8_t escrita = 0, leitura = 0;
float DerivacoesECG[12];


int16_t SWDviwe;
HAL_StatusTypeDef res;


//***********Variáveis panTompkins**********
int ImputPanTomp[BufferLenImputPanTompkins];
uint16_t panTompPointerW = 0;
uint16_t panTompPointerR = 0;
bool panTompkON = false;
uint8_t BPMResultado;
bool AtualBPM=false;

//***********Variáveis ESP8266**********
char ESPcaracter;
uint8_t CountCompare = 0;
//bool Finalmsg = false;
uint16_t RecievLen = 0;
uint8_t ComandoATselect = 0;

char ESPRxData[2048];

int8_t NumeroRedes = 0;
bool AtualRedesOn = false;
extern bool ECGOn;
bool TransmissaoECGtoESPOn = false;

int TransmitECGData[LenDataECGtoESP];
bool TransmiteECGDataWifiOn = false;
uint16_t TransmitDataPosition = 0;

bool PopupError=false;
char BufferErrorPopup[120];
uint8_t ErrorTentativas = 0;


bool StopLoadAnimat = false;

char pacienteID[25];

extern struct SSID
{
	char NomeRede[32];
	uint16_t Senha[18];
};

extern struct SSID Redes[10];

extern uint16_t KeyboardSelection;

//*************battery level*************
uint8_t Percent_battery = 100;
bool UpdateBatChargeLevel = false;


//**************LEAD OFF*******************
uint8_t Loff_StatP;
uint8_t Loff_StatN;
bool Lead_OFF_Detected = false;
uint16_t Lead_OFF_Detected_Hold = 0;

//****************FATFS*********************
volatile uint16_t counter_circularbuff = 0;
volatile uint8_t half_buffer_ECG = 0, full_buffer_ECG = 0;
volatile uint8_t pressbotao = 0;
volatile uint8_t Start_ECG_to_SD = 0;
volatile uint8_t start_stop_recordingSD = 0;
static uint8_t num_packet = 0;

//****************Filtros*********************
bool Filtro60 = false;
bool FiltroBW = false;

extern uint8_t first_time_filter_HP;
extern uint8_t first_time_filter_notch;

//****************Experimento Filtragem*********************
float ECG_withNoise_EXP[SIZEECGEXP];
uint8_t Init_exp = 0;
int Varredura_SD = 0;
uint8_t Fim_EXP = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void PeriphCommonClock_Config(void);
static void MPU_Config(void);
static void MX_GPIO_Init(void);
static void MX_CRC_Init(void);
static void MX_DSIHOST_DSI_Init(void);
static void MX_LTDC_Init(void);
static void MX_FMC_Init(void);
static void MX_QUADSPI_Init(void);
static void MX_DMA2D_Init(void);
static void MX_DMA_Init(void);
static void MX_JPEG_Init(void);
static void MX_SPI5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_UART5_Init(void);
static void MX_ADC3_Init(void);
static void MX_SDMMC2_SD_Init(void);
void panTompinksCalculation_Task(void *argument);
extern void TouchGFX_Task(void *argument);
extern void videoTaskFunc(void *argument);
void StartInitESPTask(void *argument);
void StartVerifErrorTask(void *argument);
void StartBuscarRedesTask(void *argument);
void StartConectWiFiTask(void *argument);
void StartECGtoESPTask(void *argument);
void StartTrasmECGOnTask(void *argument);
void StartRealiseDataBPMTask(void *argument);
void StartGetBatteryChargeTask(void *argument);
void StartWrittenSDTask(void *argument);
void StartExperimentoTask(void *argument);
void StartmainTask(void *argument);

/* USER CODE BEGIN PFP */
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command);

static uint8_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_EnterMemory_QPI(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_OutDrvStrengthCfg(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi);
static uint8_t QSPI_AutoPollingMemReady  (QSPI_HandleTypeDef *hqspi, uint32_t Timeout);
static uint8_t BSP_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi);

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


uint8_t Red_ADS1198_DMA(void){



	HAL_GPIO_WritePin( ADS_CS_GPIO_Port  , ADS_CS_Pin, GPIO_PIN_RESET);

	res = HAL_SPI_TransmitReceive_DMA(&hspi5, TxBuf, ADSData, 19);
		if (res == HAL_OK){

		return 1;

	}else{

		HAL_GPIO_WritePin( ADS_CS_GPIO_Port  , ADS_CS_Pin, GPIO_PIN_SET);
		return 0;

	}


}

void Red_ADS1198_DMA_Complete(void){

	float ECG_12Dev_calibration[12];


	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET);
//	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET); teste oscilo

	if(Init_exp == 0){

	/*==========================LEAD OFF DETECT BEGIN=============================*/
	Loff_StatP = (ADSData[0] << 4) | (ADSData[1] >> 4);
	Loff_StatN = ((ADSData[1] << 4) | (ADSData[2] >> 4) & 0x02);

	if(Lead_OFF_Detected_Hold != (Loff_StatP + Loff_StatN)){

		Lead_OFF_Detected_Hold = Loff_StatP + Loff_StatN;

		Lead_OFF_Detected = true;

	}

	/*==========================LEAD OFF DETECT END=============================*/

	/*==========================DERIVAÇÕES ECG begin=============================*/
	for(uint8_t i=0; i<8; i++){
	ECGchannelData[i] = (int16_t) ((ADSData[(2*i)+3] << 8) | ADSData[((2*i)+3)+1]);

}

	ECG_12Dev_calibration[0] = (float) ECGchannelData[1]*10.23 - 61.39;//I
	ECG_12Dev_calibration[1] = (float) ECGchannelData[2]*10.22 + 224.89;//II
	ECG_12Dev_calibration[2] = (float) ECG_12Dev_calibration[1]-ECG_12Dev_calibration[0]; //III
	ECG_12Dev_calibration[3] = (float) (-ECG_12Dev_calibration[0]-ECG_12Dev_calibration[1])/2; //aVR
	ECG_12Dev_calibration[4] = (float) ECG_12Dev_calibration[0]-ECG_12Dev_calibration[1]/2; //aVL
	ECG_12Dev_calibration[5] = (float) ECG_12Dev_calibration[1]-ECG_12Dev_calibration[0]/2;//aVF
	ECG_12Dev_calibration[6] = (float) ECGchannelData[7]*10.23 + 143.18;//V1
	ECG_12Dev_calibration[7] = (float) ECGchannelData[3]*10.23 + 225.06;//V2
	ECG_12Dev_calibration[8] = (float) ECGchannelData[4]*10.23 + 61.39;//V3
	ECG_12Dev_calibration[9] = (float) ECGchannelData[5]*10.23 + 583.22;//V4
	ECG_12Dev_calibration[10] = (float) ECGchannelData[6]*10.23 - 112.53;//V5
	ECG_12Dev_calibration[11] = (float) ECGchannelData[0]*10.24 + 163.78;//V6


	ECG_12Dev[counter_circularbuff*12] = (int16_t) round(ECG_12Dev_calibration[0]);
	ECG_12Dev[counter_circularbuff*12+1] = (int16_t) round(ECG_12Dev_calibration[1]);
	ECG_12Dev[counter_circularbuff*12+2] = (int16_t) round(ECG_12Dev_calibration[2]);
	ECG_12Dev[counter_circularbuff*12+3] = (int16_t) round(ECG_12Dev_calibration[3]);
	ECG_12Dev[counter_circularbuff*12+4] = (int16_t) round(ECG_12Dev_calibration[4]);
	ECG_12Dev[counter_circularbuff*12+5] = (int16_t) round(ECG_12Dev_calibration[5]);
	ECG_12Dev[counter_circularbuff*12+6] = (int16_t) round(ECG_12Dev_calibration[6]);
	ECG_12Dev[counter_circularbuff*12+7] = (int16_t) round(ECG_12Dev_calibration[7]);
	ECG_12Dev[counter_circularbuff*12+8] = (int16_t) round(ECG_12Dev_calibration[8]);
	ECG_12Dev[counter_circularbuff*12+9] = (int16_t) round(ECG_12Dev_calibration[9]);
	ECG_12Dev[counter_circularbuff*12+10] = (int16_t) round(ECG_12Dev_calibration[10]);
	ECG_12Dev[counter_circularbuff*12+11] = (int16_t) round(ECG_12Dev_calibration[11]);

//	DerivacoesECG[0] = channelData[1]/97729.20 - 0.00006139; //I
//	DerivacoesECG[1] = channelData[2]/97825.57 + 0.00022489; //II
//	DerivacoesECG[2] = DerivacoesECG[1]-DerivacoesECG[0]; //III
//	DerivacoesECG[3] = (-DerivacoesECG[0]-DerivacoesECG[1])/2; //aVR
//	DerivacoesECG[4] = DerivacoesECG[0]-DerivacoesECG[1]/2; //aVL
//	DerivacoesECG[5] = DerivacoesECG[1]-DerivacoesECG[0]/2; //aVF
//	DerivacoesECG[6] = channelData[7]/97780.42 + 0.00014318; //V1
//	DerivacoesECG[7] = channelData[3]/97751.48 + 0.00022506; //V2
//	DerivacoesECG[8] = channelData[4]/97729.64 + 0.00006139; //V3
//	DerivacoesECG[9] = channelData[5]/97732.93 + 0.00058322; //V4
// 	DerivacoesECG[10] = channelData[6]/97754.28 - 0.00011253; //V5
//	DerivacoesECG[11] = channelData[0]/97693.33 + 0.00016378; //V6
	/*==========================DERIVAÇÕES ECG end=============================*/

	/*==========================EXIBIR ECG begin=============================*/
	escrita++;
	if(escrita == BufferLenPlotECG){
		escrita = 0;
	}

	if(Filtro60){
		ECG_12Dev_calibration[1] = notch_rFixo(ECG_12Dev_calibration[1], 1);
	}

	if(FiltroBW){
		ECG_12Dev_calibration[1] = passa_altas_IIR(ECG_12Dev_calibration[1], 1);
	}

	switch (DerivSelecionada) {
	case dev_I:
		valorECG[escrita] = ECG_12Dev_calibration[0];
		break;
	case dev_II:
		valorECG[escrita] = ECG_12Dev_calibration[1];
		break;
	case dev_III:
		valorECG[escrita] = ECG_12Dev_calibration[2];
		break;
	case dev_aVR:
		valorECG[escrita] = ECG_12Dev_calibration[3];
		break;
	case dev_aVL:
		valorECG[escrita] = ECG_12Dev_calibration[4];
		break;
	case dev_aVF:
		valorECG[escrita] = ECG_12Dev_calibration[5];
		break;
	case dev_V1:
		valorECG[escrita] = ECG_12Dev_calibration[6];
		break;
	case dev_V2:
		valorECG[escrita] = ECG_12Dev_calibration[7];
		break;
	case dev_V3:
		valorECG[escrita] = ECG_12Dev_calibration[8];
		break;
	case dev_V4:
		valorECG[escrita] = ECG_12Dev_calibration[9];
		break;
	case dev_V5:
		valorECG[escrita] = ECG_12Dev_calibration[10];
		break;
	case dev_V6:
		valorECG[escrita] = ECG_12Dev_calibration[11];
		break;
	default:
		break;
	}

	/*==========================EXIBIR ECG end=============================*/

	/*==========================TRANSMITIR begin=============================*/
	if(TransmiteECGDataWifiOn){
		TransmitECGData[TransmitDataPosition] = (int) valorECG[escrita]; //transmite a derivacao selecionada na tela
		TransmitDataPosition++;
	}
	if(TransmitDataPosition == LenDataECGtoESP && TransmiteECGDataWifiOn){
		TransmitDataPosition=0;
		ECGOn = false;
		ComandoATselect =10;
		TransmiteECGDataWifiOn = false;
		TransmissaoECGtoESPOn = true;
	}
	/*==========================EXIBIR ECG end=============================*/


	/*==========================panTompkins begin=============================*/
	if(panTompkON){
		ImputPanTomp[panTompPointerW] = (int) ECG_12Dev[counter_circularbuff*12+2];
		panTompPointerW++;
		if(panTompPointerW == BufferLenImputPanTompkins){
			panTompPointerW = 0;
		}
	}
	/*==========================panTompkins end=============================*/

	/*==========================FATFS begin=============================*/


	if(counter_circularbuff == 255)
	{
		half_buffer_ECG = 1;
	}

	if(counter_circularbuff == 511)
	{
		counter_circularbuff = 0;
		full_buffer_ECG = 1;
	}
	else
	{
		counter_circularbuff++;
	}

	/*==========================FATFS end=============================*/


	}else{
		/*==========================Experimento=============================*/
		escrita++;
		if(escrita == BufferLenPlotECG){
			escrita = 0;
		}

		if(Filtro60){
			ECG_withNoise_EXP[Varredura_SD] = notch_rFixo(ECG_withNoise_EXP[Varredura_SD], 2);
		}

		if(FiltroBW){
			ECG_withNoise_EXP[Varredura_SD] = passa_altas_IIR(ECG_withNoise_EXP[Varredura_SD], 2);
		}

		valorECG[escrita] = ECG_withNoise_EXP[Varredura_SD];

		Varredura_SD++;
		if(Varredura_SD == SIZEECGEXP){
			Fim_EXP = 1;
		}


	}

	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);
}


void bufclear(char *buf){

	uint16_t len = strlen (buf);
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

  /* MPU Configuration--------------------------------------------------------*/
  MPU_Config();

  /* Enable I-Cache---------------------------------------------------------*/
  SCB_EnableICache();

  /* Enable D-Cache---------------------------------------------------------*/
  SCB_EnableDCache();

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

/* Configure the peripherals common clocks */
  PeriphCommonClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_CRC_Init();
  MX_DSIHOST_DSI_Init();
  MX_LTDC_Init();
  MX_FMC_Init();
  MX_QUADSPI_Init();
  MX_DMA2D_Init();
  MX_I2C4_Init();
  MX_DMA_Init();
  MX_JPEG_Init();
  MX_SPI5_Init();
  MX_USART1_UART_Init();
  MX_UART5_Init();
  MX_ADC3_Init();
  MX_SDMMC2_SD_Init();
  MX_FATFS_Init();
  MX_TouchGFX_Init();
  /* USER CODE BEGIN 2 */


  switch (initADS1198){
  case ECG_12_Lead:
	  ADS_Init();
	  break;

  case Teste_interno:
	  ADS_InitTestInt();
	  break;

  case Teste_externo:
	  break;
  }


  ESP_init();


  HAL_UART_Receive_IT(&huart5, &ESPcaracter, 1);
  /* USER CODE END 2 */

  /* Init scheduler */
  osKernelInitialize();

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
  /* creation of DetectordePicos */
  DetectordePicosHandle = osThreadNew(panTompinksCalculation_Task, NULL, &DetectordePicos_attributes);

  /* creation of TouchGFXTask */
  TouchGFXTaskHandle = osThreadNew(TouchGFX_Task, NULL, &TouchGFXTask_attributes);

  /* creation of videoTask */
  videoTaskHandle = osThreadNew(videoTaskFunc, NULL, &videoTask_attributes);

  /* creation of InitESPTask */
  InitESPTaskHandle = osThreadNew(StartInitESPTask, NULL, &InitESPTask_attributes);

  /* creation of VerifErrorTask */
  VerifErrorTaskHandle = osThreadNew(StartVerifErrorTask, NULL, &VerifErrorTask_attributes);

  /* creation of BuscarRedesTask */
  BuscarRedesTaskHandle = osThreadNew(StartBuscarRedesTask, NULL, &BuscarRedesTask_attributes);

  /* creation of ConectWiFiTask */
  ConectWiFiTaskHandle = osThreadNew(StartConectWiFiTask, NULL, &ConectWiFiTask_attributes);

  /* creation of ECGtoESPTask */
  ECGtoESPTaskHandle = osThreadNew(StartECGtoESPTask, NULL, &ECGtoESPTask_attributes);

  /* creation of TrasmECGOnTask */
  TrasmECGOnTaskHandle = osThreadNew(StartTrasmECGOnTask, NULL, &TrasmECGOnTask_attributes);

  /* creation of RealiseDataBPMT */
  RealiseDataBPMTHandle = osThreadNew(StartRealiseDataBPMTask, NULL, &RealiseDataBPMT_attributes);

  /* creation of GetBatteryTask */
  GetBatteryTaskHandle = osThreadNew(StartGetBatteryChargeTask, NULL, &GetBatteryTask_attributes);

  /* creation of WrittenSDTask */
  WrittenSDTaskHandle = osThreadNew(StartWrittenSDTask, NULL, &WrittenSDTask_attributes);

  /* creation of ExperimentoTask */
  ExperimentoTaskHandle = osThreadNew(StartExperimentoTask, NULL, &ExperimentoTask_attributes);

  /* creation of mainTask */
  mainTaskHandle = osThreadNew(StartmainTask, NULL, &mainTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

  /* Start scheduler */
  osKernelStart();

  /* We should never get here as control is now taken by the scheduler */
  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {


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
  RCC_OscInitStruct.PLL.PLLR = 7;
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
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief Peripherals Common Clock Configuration
  * @retval None
  */
void PeriphCommonClock_Config(void)
{
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clock
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC|RCC_PERIPHCLK_SDMMC2
                              |RCC_PERIPHCLK_CLK48;
  PeriphClkInitStruct.PLLSAI.PLLSAIN = 384;
  PeriphClkInitStruct.PLLSAI.PLLSAIR = 7;
  PeriphClkInitStruct.PLLSAI.PLLSAIQ = 2;
  PeriphClkInitStruct.PLLSAI.PLLSAIP = RCC_PLLSAIP_DIV8;
  PeriphClkInitStruct.PLLSAIDivQ = 1;
  PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
  PeriphClkInitStruct.Clk48ClockSelection = RCC_CLK48SOURCE_PLLSAIP;
  PeriphClkInitStruct.Sdmmc2ClockSelection = RCC_SDMMC2CLKSOURCE_CLK48;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC3_Init(void)
{

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Configure the global features of the ADC (Clock, Resolution, Data Alignment and number of conversion)
  */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  if (HAL_ADC_Init(&hadc3) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure for the selected ADC regular channel its corresponding rank in the sequencer and its sample time.
  */
  sConfig.Channel = ADC_CHANNEL_8;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_3CYCLES;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */

}

/**
  * @brief CRC Initialization Function
  * @param None
  * @retval None
  */
static void MX_CRC_Init(void)
{

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_ENABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_ENABLE;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */

}

/**
  * @brief DMA2D Initialization Function
  * @param None
  * @retval None
  */
static void MX_DMA2D_Init(void)
{

  /* USER CODE BEGIN DMA2D_Init 0 */

  /* USER CODE END DMA2D_Init 0 */

  /* USER CODE BEGIN DMA2D_Init 1 */

  /* USER CODE END DMA2D_Init 1 */
  hdma2d.Instance = DMA2D;
  hdma2d.Init.Mode = DMA2D_M2M;
  hdma2d.Init.ColorMode = DMA2D_OUTPUT_ARGB8888;
  hdma2d.Init.OutputOffset = 0;
  hdma2d.LayerCfg[1].InputOffset = 0;
  hdma2d.LayerCfg[1].InputColorMode = DMA2D_INPUT_ARGB8888;
  hdma2d.LayerCfg[1].AlphaMode = DMA2D_NO_MODIF_ALPHA;
  hdma2d.LayerCfg[1].InputAlpha = 0;
  hdma2d.LayerCfg[1].AlphaInverted = DMA2D_REGULAR_ALPHA;
  hdma2d.LayerCfg[1].RedBlueSwap = DMA2D_RB_REGULAR;
  if (HAL_DMA2D_Init(&hdma2d) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DMA2D_ConfigLayer(&hdma2d, 1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DMA2D_Init 2 */

  /* USER CODE END DMA2D_Init 2 */

}

/**
  * @brief DSIHOST Initialization Function
  * @param None
  * @retval None
  */
static void MX_DSIHOST_DSI_Init(void)
{

  /* USER CODE BEGIN DSIHOST_Init 0 */
  /* Activate XRES active low */
  HAL_GPIO_WritePin(DSI_RESET_GPIO_Port, DSI_RESET_Pin, GPIO_PIN_RESET);
  HAL_Delay(20); /* wait 20 ms */
  /* Desactivate XRES */
  HAL_GPIO_WritePin(DSI_RESET_GPIO_Port, DSI_RESET_Pin, GPIO_PIN_SET);
  /* Wait for 10ms after releasing XRES before sending commands */
  HAL_Delay(10);
  /* USER CODE END DSIHOST_Init 0 */

  DSI_PLLInitTypeDef PLLInit = {0};
  DSI_HOST_TimeoutTypeDef HostTimeouts = {0};
  DSI_PHY_TimerTypeDef PhyTimings = {0};
  DSI_LPCmdTypeDef LPCmd = {0};
  DSI_CmdCfgTypeDef CmdCfg = {0};

  /* USER CODE BEGIN DSIHOST_Init 1 */

  /* USER CODE END DSIHOST_Init 1 */
  hdsi.Instance = DSI;
  hdsi.Init.AutomaticClockLaneControl = DSI_AUTO_CLK_LANE_CTRL_DISABLE;
  hdsi.Init.TXEscapeCkdiv = 4;
  hdsi.Init.NumberOfLanes = DSI_TWO_DATA_LANES;
  PLLInit.PLLNDIV = 100;
  PLLInit.PLLIDF = DSI_PLL_IN_DIV5;
  PLLInit.PLLODF = DSI_PLL_OUT_DIV1;
  if (HAL_DSI_Init(&hdsi, &PLLInit) != HAL_OK)
  {
    Error_Handler();
  }
  HostTimeouts.TimeoutCkdiv = 1;
  HostTimeouts.HighSpeedTransmissionTimeout = 0;
  HostTimeouts.LowPowerReceptionTimeout = 0;
  HostTimeouts.HighSpeedReadTimeout = 0;
  HostTimeouts.LowPowerReadTimeout = 0;
  HostTimeouts.HighSpeedWriteTimeout = 0;
  HostTimeouts.HighSpeedWritePrespMode = DSI_HS_PM_DISABLE;
  HostTimeouts.LowPowerWriteTimeout = 0;
  HostTimeouts.BTATimeout = 0;
  if (HAL_DSI_ConfigHostTimeouts(&hdsi, &HostTimeouts) != HAL_OK)
  {
    Error_Handler();
  }
  PhyTimings.ClockLaneHS2LPTime = 28;
  PhyTimings.ClockLaneLP2HSTime = 33;
  PhyTimings.DataLaneHS2LPTime = 15;
  PhyTimings.DataLaneLP2HSTime = 25;
  PhyTimings.DataLaneMaxReadTime = 0;
  PhyTimings.StopWaitTime = 0;
  if (HAL_DSI_ConfigPhyTimer(&hdsi, &PhyTimings) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigFlowControl(&hdsi, DSI_FLOW_CONTROL_BTA) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetLowPowerRXFilter(&hdsi, 10000) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_ConfigErrorMonitor(&hdsi, HAL_DSI_ERROR_OVF) != HAL_OK)
  {
    Error_Handler();
  }
  LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_ENABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_ENABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_ENABLE;
  LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_ENABLE;
  LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_ENABLE;
  LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_ENABLE;
  LPCmd.LPGenLongWrite = DSI_LP_GLW_ENABLE;
  LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_ENABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_ENABLE;
  LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_ENABLE;
  LPCmd.LPDcsLongWrite = DSI_LP_DLW_ENABLE;
  LPCmd.LPMaxReadPacket = DSI_LP_MRDP_ENABLE;
  LPCmd.AcknowledgeRequest = DSI_ACKNOWLEDGE_ENABLE;
  if (HAL_DSI_ConfigCommand(&hdsi, &LPCmd) != HAL_OK)
  {
    Error_Handler();
  }
  CmdCfg.VirtualChannelID = 0;
  CmdCfg.ColorCoding = DSI_RGB565;
  CmdCfg.CommandSize = 200;
  CmdCfg.TearingEffectSource = DSI_TE_EXTERNAL;
  CmdCfg.TearingEffectPolarity = DSI_TE_RISING_EDGE;
  CmdCfg.HSPolarity = DSI_HSYNC_ACTIVE_LOW;
  CmdCfg.VSPolarity = DSI_VSYNC_ACTIVE_LOW;
  CmdCfg.DEPolarity = DSI_DATA_ENABLE_ACTIVE_HIGH;
  CmdCfg.VSyncPol = DSI_VSYNC_FALLING;
  CmdCfg.AutomaticRefresh = DSI_AR_DISABLE;
  CmdCfg.TEAcknowledgeRequest = DSI_TE_ACKNOWLEDGE_ENABLE;
  if (HAL_DSI_ConfigAdaptedCommandMode(&hdsi, &CmdCfg) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_DSI_SetGenericVCID(&hdsi, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN DSIHOST_Init 2 */

  /* USER CODE END DSIHOST_Init 2 */

}

/**
  * @brief I2C4 Initialization Function
  * @param None
  * @retval None
  */
void MX_I2C4_Init(void)
{

  /* USER CODE BEGIN I2C4_Init 0 */

  /* USER CODE END I2C4_Init 0 */

  /* USER CODE BEGIN I2C4_Init 1 */

  /* USER CODE END I2C4_Init 1 */
  hi2c4.Instance = I2C4;
  hi2c4.Init.Timing = 0x00C0EAFF;
  hi2c4.Init.OwnAddress1 = 0;
  hi2c4.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c4.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c4.Init.OwnAddress2 = 0;
  hi2c4.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c4.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c4.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c4) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c4, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c4, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C4_Init 2 */

  /* USER CODE END I2C4_Init 2 */

}

/**
  * @brief JPEG Initialization Function
  * @param None
  * @retval None
  */
static void MX_JPEG_Init(void)
{

  /* USER CODE BEGIN JPEG_Init 0 */

  /* USER CODE END JPEG_Init 0 */

  /* USER CODE BEGIN JPEG_Init 1 */

  /* USER CODE END JPEG_Init 1 */
  hjpeg.Instance = JPEG;
  if (HAL_JPEG_Init(&hjpeg) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN JPEG_Init 2 */

  /* USER CODE END JPEG_Init 2 */

}

/**
  * @brief LTDC Initialization Function
  * @param None
  * @retval None
  */
static void MX_LTDC_Init(void)
{

  /* USER CODE BEGIN LTDC_Init 0 */

  /* USER CODE END LTDC_Init 0 */

  LTDC_LayerCfgTypeDef pLayerCfg = {0};

  /* USER CODE BEGIN LTDC_Init 1 */

  /* USER CODE END LTDC_Init 1 */
  hltdc.Instance = LTDC;
  hltdc.Init.HSPolarity = LTDC_HSPOLARITY_AL;
  hltdc.Init.VSPolarity = LTDC_VSPOLARITY_AL;
  hltdc.Init.DEPolarity = LTDC_DEPOLARITY_AL;
  hltdc.Init.PCPolarity = LTDC_PCPOLARITY_IPC;
  hltdc.Init.HorizontalSync = 1;
  hltdc.Init.VerticalSync = 1;
  hltdc.Init.AccumulatedHBP = 2;
  hltdc.Init.AccumulatedVBP = 2;
  hltdc.Init.AccumulatedActiveW = 202;
  hltdc.Init.AccumulatedActiveH = 482;
  hltdc.Init.TotalWidth = 203;
  hltdc.Init.TotalHeigh = 483;
  hltdc.Init.Backcolor.Blue = 0;
  hltdc.Init.Backcolor.Green = 0;
  hltdc.Init.Backcolor.Red = 0;
  if (HAL_LTDC_Init(&hltdc) != HAL_OK)
  {
    Error_Handler();
  }
  pLayerCfg.WindowX0 = 0;
  pLayerCfg.WindowX1 = 200;
  pLayerCfg.WindowY0 = 0;
  pLayerCfg.WindowY1 = 480;
  pLayerCfg.PixelFormat = LTDC_PIXEL_FORMAT_RGB565;
  pLayerCfg.Alpha = 255;
  pLayerCfg.Alpha0 = 0;
  pLayerCfg.BlendingFactor1 = LTDC_BLENDING_FACTOR1_CA;
  pLayerCfg.BlendingFactor2 = LTDC_BLENDING_FACTOR2_CA;
  pLayerCfg.FBStartAdress = 0xC0000000;
  pLayerCfg.ImageWidth = 200;
  pLayerCfg.ImageHeight = 480;
  pLayerCfg.Backcolor.Blue = 0;
  pLayerCfg.Backcolor.Green = 0;
  pLayerCfg.Backcolor.Red = 0;
  if (HAL_LTDC_ConfigLayer(&hltdc, &pLayerCfg, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN LTDC_Init 2 */
  __HAL_LTDC_DISABLE(&hltdc);
  DSI_LPCmdTypeDef LPCmd;

  HAL_DSI_Start(&hdsi);
  OTM8009A_Init(OTM8009A_FORMAT_RBG565, LCD_ORIENTATION_LANDSCAPE);

  HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF, 0x00);

  LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_DISABLE;
  LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
  LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
  LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_DISABLE;
  LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_DISABLE;
  LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_DISABLE;
  LPCmd.LPGenLongWrite = DSI_LP_GLW_DISABLE;
  LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_DISABLE;
  LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
  LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
  LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
  HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  HAL_LTDC_SetPitch(&hltdc, 800, 0);
  __HAL_LTDC_ENABLE(&hltdc);

  // LPCmd.LPGenShortWriteNoP = DSI_LP_GSW0P_DISABLE;
  // LPCmd.LPGenShortWriteOneP = DSI_LP_GSW1P_DISABLE;
  // LPCmd.LPGenShortWriteTwoP = DSI_LP_GSW2P_DISABLE;
  // LPCmd.LPGenShortReadNoP = DSI_LP_GSR0P_DISABLE;
  // LPCmd.LPGenShortReadOneP = DSI_LP_GSR1P_DISABLE;
  // LPCmd.LPGenShortReadTwoP = DSI_LP_GSR2P_DISABLE;
  // LPCmd.LPGenLongWrite = DSI_LP_GLW_DISABLE;
  // LPCmd.LPDcsShortWriteNoP = DSI_LP_DSW0P_DISABLE;
  // LPCmd.LPDcsShortWriteOneP = DSI_LP_DSW1P_DISABLE;
  // LPCmd.LPDcsShortReadNoP = DSI_LP_DSR0P_DISABLE;
  // LPCmd.LPDcsLongWrite = DSI_LP_DLW_DISABLE;
  // HAL_DSI_ConfigCommand(&hdsi, &LPCmd);

  // HAL_DSI_ShortWrite(&hdsi, 0, DSI_DCS_SHORT_PKT_WRITE_P1, OTM8009A_CMD_DISPOFF, 0x00);

  // HAL_LTDC_SetPitch(&hltdc, 800, 0);
  // __HAL_LTDC_ENABLE(&hltdc);

  /* USER CODE END LTDC_Init 2 */

}

/**
  * @brief QUADSPI Initialization Function
  * @param None
  * @retval None
  */
static void MX_QUADSPI_Init(void)
{

  /* USER CODE BEGIN QUADSPI_Init 0 */

  /* USER CODE END QUADSPI_Init 0 */

  /* USER CODE BEGIN QUADSPI_Init 1 */

  /* USER CODE END QUADSPI_Init 1 */
  /* QUADSPI parameter configuration*/
  hqspi.Instance = QUADSPI;
  hqspi.Init.ClockPrescaler = 1;
  hqspi.Init.FifoThreshold = 16;
  hqspi.Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
  hqspi.Init.FlashSize = 25;
  hqspi.Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_4_CYCLE;
  hqspi.Init.ClockMode = QSPI_CLOCK_MODE_0;
  hqspi.Init.FlashID = QSPI_FLASH_ID_1;
  hqspi.Init.DualFlash = QSPI_DUALFLASH_DISABLE;
  if (HAL_QSPI_Init(&hqspi) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN QUADSPI_Init 2 */
  /* QSPI memory reset */
  if (QSPI_ResetMemory(&hqspi) != QSPI_OK)
  {
    Error_Handler();
  }

  /* Put QSPI memory in QPI mode */
  if( QSPI_EnterMemory_QPI( &hqspi )!=QSPI_OK )
  {
    Error_Handler();
  }

  /* Set the QSPI memory in 4-bytes address mode */
  if (QSPI_EnterFourBytesAddress(&hqspi) != QSPI_OK)
  {
    Error_Handler();
  }

  /* Configuration of the dummy cycles on QSPI memory side */
  if (QSPI_DummyCyclesCfg(&hqspi) != QSPI_OK)
  {
    Error_Handler();
  }

  /* Configuration of the Output driver strength on memory side */
  if( QSPI_OutDrvStrengthCfg( &hqspi ) != QSPI_OK )
  {
    Error_Handler();
  }

  if( BSP_QSPI_EnableMemoryMappedMode(&hqspi) != QSPI_OK )
  {
    Error_Handler();
  }

  /* USER CODE END QUADSPI_Init 2 */

}

/**
  * @brief SDMMC2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SDMMC2_SD_Init(void)
{

  /* USER CODE BEGIN SDMMC2_Init 0 */

  /* USER CODE END SDMMC2_Init 0 */

  /* USER CODE BEGIN SDMMC2_Init 1 */

  /* USER CODE END SDMMC2_Init 1 */
  hsd2.Instance = SDMMC2;
  hsd2.Init.ClockEdge = SDMMC_CLOCK_EDGE_RISING;
  hsd2.Init.ClockBypass = SDMMC_CLOCK_BYPASS_DISABLE;
  hsd2.Init.ClockPowerSave = SDMMC_CLOCK_POWER_SAVE_DISABLE;
  hsd2.Init.BusWide = SDMMC_BUS_WIDE_1B;
  hsd2.Init.HardwareFlowControl = SDMMC_HARDWARE_FLOW_CONTROL_DISABLE;
  hsd2.Init.ClockDiv = 0;
  /* USER CODE BEGIN SDMMC2_Init 2 */

  /* USER CODE END SDMMC2_Init 2 */

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
  hspi5.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
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
  * @brief UART5 Initialization Function
  * @param None
  * @retval None
  */
static void MX_UART5_Init(void)
{

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 9600;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */

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
  /* DMA2_Stream0_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
  /* DMA2_Stream3_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream3_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream3_IRQn);
  /* DMA2_Stream4_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream4_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream4_IRQn);
  /* DMA2_Stream5_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA2_Stream5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(DMA2_Stream5_IRQn);

}

/* FMC initialization function */
static void MX_FMC_Init(void)
{

  /* USER CODE BEGIN FMC_Init 0 */

  /* USER CODE END FMC_Init 0 */

  FMC_SDRAM_TimingTypeDef SdramTiming = {0};

  /* USER CODE BEGIN FMC_Init 1 */

  /* USER CODE END FMC_Init 1 */

  /** Perform the SDRAM1 memory initialization sequence
  */
  hsdram1.Instance = FMC_SDRAM_DEVICE;
  /* hsdram1.Init */
  hsdram1.Init.SDBank = FMC_SDRAM_BANK1;
  hsdram1.Init.ColumnBitsNumber = FMC_SDRAM_COLUMN_BITS_NUM_8;
  hsdram1.Init.RowBitsNumber = FMC_SDRAM_ROW_BITS_NUM_12;
  hsdram1.Init.MemoryDataWidth = FMC_SDRAM_MEM_BUS_WIDTH_32;
  hsdram1.Init.InternalBankNumber = FMC_SDRAM_INTERN_BANKS_NUM_4;
  hsdram1.Init.CASLatency = FMC_SDRAM_CAS_LATENCY_3;
  hsdram1.Init.WriteProtection = FMC_SDRAM_WRITE_PROTECTION_DISABLE;
  hsdram1.Init.SDClockPeriod = FMC_SDRAM_CLOCK_PERIOD_2;
  hsdram1.Init.ReadBurst = FMC_SDRAM_RBURST_ENABLE;
  hsdram1.Init.ReadPipeDelay = FMC_SDRAM_RPIPE_DELAY_0;
  /* SdramTiming */
  SdramTiming.LoadToActiveDelay = 2;
  SdramTiming.ExitSelfRefreshDelay = 7;
  SdramTiming.SelfRefreshTime = 4;
  SdramTiming.RowCycleDelay = 7;
  SdramTiming.WriteRecoveryTime = 2;
  SdramTiming.RPDelay = 2;
  SdramTiming.RCDDelay = 3;

  if (HAL_SDRAM_Init(&hsdram1, &SdramTiming) != HAL_OK)
  {
    Error_Handler( );
  }

  /* USER CODE BEGIN FMC_Init 2 */

  FMC_SDRAM_CommandTypeDef command;

  /* Program the SDRAM external device */
  BSP_SDRAM_Initialization_Sequence(&hsdram1, &command);

  //Deactivate speculative/cache access to first FMC Bank to save FMC bandwidth
  FMC_Bank1->BTCR[0] = 0x000030D2;
  /* USER CODE END FMC_Init 2 */
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
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  __HAL_RCC_GPIOJ_CLK_ENABLE();
  __HAL_RCC_GPIOI_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOJ, VSYNC_FREQ2_Pin|RENDER_TIME2_Pin|FRAMERATE_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOJ, WIFI_RST_Pin|DSI_RESET_Pin|ADS_CS_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_DAISY_GPIO_Port, ADS_DAISY_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_RESET_GPIO_Port, ADS_RESET_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(PW_KEY_GPIO_Port, PW_KEY_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(WIFI_CH_PD_GPIO_Port, WIFI_CH_PD_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ADS_CLKSEL_GPIO_Port, ADS_CLKSEL_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : PB8 ADS_CLKSEL_Pin */
  GPIO_InitStruct.Pin = GPIO_PIN_8|ADS_CLKSEL_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : VSYNC_FREQ2_Pin RENDER_TIME2_Pin */
  GPIO_InitStruct.Pin = VSYNC_FREQ2_Pin|RENDER_TIME2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOJ, &GPIO_InitStruct);

  /*Configure GPIO pin : WIFI_RST_Pin */
  GPIO_InitStruct.Pin = WIFI_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_RST_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : DSI_RESET_Pin */
  GPIO_InitStruct.Pin = DSI_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_MEDIUM;
  HAL_GPIO_Init(DSI_RESET_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PI15 */
  GPIO_InitStruct.Pin = GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);

  /*Configure GPIO pins : ADS_DAISY_Pin ADS_RESET_Pin */
  GPIO_InitStruct.Pin = ADS_DAISY_Pin|ADS_RESET_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS_DRDY_Pin */
  GPIO_InitStruct.Pin = ADS_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(ADS_DRDY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : PW_KEY_Pin */
  GPIO_InitStruct.Pin = PW_KEY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(PW_KEY_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : WIFI_CH_PD_Pin */
  GPIO_InitStruct.Pin = WIFI_CH_PD_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(WIFI_CH_PD_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : ADS_CS_Pin */
  GPIO_InitStruct.Pin = ADS_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ADS_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : FRAMERATE_Pin */
  GPIO_InitStruct.Pin = FRAMERATE_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(FRAMERATE_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */
/**
  * @brief  Perform the SDRAM external memory initialization sequence
  * @param  hsdram: SDRAM handle
  * @param  Command: Pointer to SDRAM command structure
  * @retval None
  */
static void BSP_SDRAM_Initialization_Sequence(SDRAM_HandleTypeDef *hsdram, FMC_SDRAM_CommandTypeDef *Command)
{
 __IO uint32_t tmpmrd = 0;

    /* Step 1: Configure a clock configuration enable command */
    Command->CommandMode            = FMC_SDRAM_CMD_CLK_ENABLE;
    Command->CommandTarget          =  FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber      = 1;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

    /* Step 2: Insert 100 us minimum delay */
    /* Inserted delay is equal to 1 ms due to systick time base unit (ms) */
    HAL_Delay(1);

    /* Step 3: Configure a PALL (precharge all) command */
    Command->CommandMode            = FMC_SDRAM_CMD_PALL;
    Command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber      = 1;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

    /* Step 4: Configure an Auto Refresh command */
    Command->CommandMode            = FMC_SDRAM_CMD_AUTOREFRESH_MODE;
    Command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber      = 8;
    Command->ModeRegisterDefinition = 0;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

    /* Step 5: Program the external memory mode register */
    tmpmrd = (uint32_t)SDRAM_MODEREG_BURST_LENGTH_1          | \
             SDRAM_MODEREG_BURST_TYPE_SEQUENTIAL   | \
             SDRAM_MODEREG_CAS_LATENCY_3           | \
             SDRAM_MODEREG_OPERATING_MODE_STANDARD | \
             SDRAM_MODEREG_WRITEBURST_MODE_SINGLE;

    Command->CommandMode            = FMC_SDRAM_CMD_LOAD_MODE;
    Command->CommandTarget          = FMC_SDRAM_CMD_TARGET_BANK1;
    Command->AutoRefreshNumber      = 1;
    Command->ModeRegisterDefinition = tmpmrd;

    /* Send the command */
    HAL_SDRAM_SendCommand(hsdram, Command, SDRAM_TIMEOUT);

    /* Step 6: Set the refresh rate counter */
    /* Set the device refresh rate */
    HAL_SDRAM_ProgramRefreshRate(hsdram, REFRESH_COUNT);

}

/**
  * @brief  This function reset the QSPI memory.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_ResetMemory(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_AutoPollingTypeDef  s_config;
  uint8_t                  reg;

  /* Send command RESET command in QPI mode (QUAD I/Os) */
  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = RESET_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }
  /* Send the reset memory command */
  s_command.Instruction = RESET_MEMORY_CMD;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Send command RESET command in SPI mode */
  /* Initialize the reset enable command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = RESET_ENABLE_CMD;
  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }
  /* Send the reset memory command */
  s_command.Instruction = RESET_MEMORY_CMD;
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* After reset CMD, 1000ms requested if QSPI memory SWReset occured during full chip erase operation */
  HAL_Delay( 1000 );

  /* Configure automatic polling mode to wait the WIP bit=0 */
  s_config.Match           = 0;
  s_config.Mask            = MX25L512_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.InstructionMode = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction     = READ_STATUS_REG_CMD;
  s_command.DataMode        = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Initialize the reading of status register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Enable write operations, command in 1 bit */
  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = MX25L512_SR_WREN;
  s_config.Mask            = MX25L512_SR_WREN;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_1_LINE;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Update the configuration register with new dummy cycles */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = WRITE_STATUS_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_1_LINE;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable the Quad IO on the QSPI memory (Non-volatile bit) */
  reg |= MX25L512_SR_QUADEN;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, &reg, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* 40ms  Write Status/Configuration Register Cycle Time */
  HAL_Delay( 40 );

  return QSPI_OK;
}

/**
  * @brief  This function put QSPI memory in QPI mode (quad I/O).
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_EnterMemory_QPI( QSPI_HandleTypeDef *hqspi )
{
  QSPI_CommandTypeDef      s_command;
  QSPI_AutoPollingTypeDef  s_config;

  /* Initialize the QPI enable command */
  /* QSPI memory is supported to be in SPI mode, so CMD on 1 LINE */
  s_command.InstructionMode   = QSPI_INSTRUCTION_1_LINE;
  s_command.Instruction       = ENTER_QUAD_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait the QUADEN bit=1 and WIP bit=0 */
  s_config.Match           = MX25L512_SR_QUADEN;
  s_config.Mask            = MX25L512_SR_QUADEN|MX25L512_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.DataMode          = QSPI_DATA_4_LINES;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  This function set the QSPI memory in 4-byte address mode
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_EnterFourBytesAddress(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;

  /* Initialize the command */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = ENTER_4_BYTE_ADDR_MODE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != QSPI_OK)
  {
    return QSPI_ERROR;
  }

  /* Send the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait the memory is ready */
  if (QSPI_AutoPollingMemReady(hqspi, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != QSPI_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  This function configure the dummy cycles on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_DummyCyclesCfg(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef s_command;
  uint8_t reg[2];

  /* Initialize the reading of status register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Initialize the reading of configuration register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &(reg[1]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != QSPI_OK)
  {
    return QSPI_ERROR;
  }

  /* Update the configuration register with new dummy cycles */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = WRITE_STATUS_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* MX25L512_DUMMY_CYCLES_READ_QUAD = 3 for 10 cycles in QPI mode */
  MODIFY_REG( reg[1], MX25L512_CR_NB_DUMMY, (MX25L512_DUMMY_CYCLES_READ_QUAD << POSITION_VAL(MX25L512_CR_NB_DUMMY)));

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* 40ms  Write Status/Configuration Register Cycle Time */
  HAL_Delay( 40 );

  return QSPI_OK;
}

/**
  * @brief  This function configure the Output driver strength on memory side.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_OutDrvStrengthCfg( QSPI_HandleTypeDef *hqspi )
{
  QSPI_CommandTypeDef s_command;
  uint8_t reg[2];

  /* Initialize the reading of status register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Initialize the reading of configuration register */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 1;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Reception of the data */
  if (HAL_QSPI_Receive(hqspi, &(reg[1]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Enable write operations */
  if (QSPI_WriteEnable(hqspi) != QSPI_OK)
  {
    return QSPI_ERROR;
  }

  /* Update the configuration register with new output driver strength */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = WRITE_STATUS_CFG_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.NbData            = 2;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Set Output Strength of the QSPI memory 15 ohms */
  MODIFY_REG( reg[1], MX25L512_CR_ODS, (MX25L512_CR_ODS_15 << POSITION_VAL(MX25L512_CR_ODS)));

  /* Configure the write volatile configuration register command */
  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Transmission of the data */
  if (HAL_QSPI_Transmit(hqspi, &(reg[0]), HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  This function send a Write Enable and wait it is effective.
  * @param  hqspi: QSPI handle
  * @retval None
  */
static uint8_t QSPI_WriteEnable(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Enable write operations */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = WRITE_ENABLE_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_NONE;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  if (HAL_QSPI_Command(hqspi, &s_command, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  /* Configure automatic polling mode to wait for write enabling */
  s_config.Match           = MX25L512_SR_WREN;
  s_config.Mask            = MX25L512_SR_WREN;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  s_command.Instruction    = READ_STATUS_REG_CMD;
  s_command.DataMode       = QSPI_DATA_4_LINES;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, HAL_QPSI_TIMEOUT_DEFAULT_VALUE) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  This function read the SR of the memory and wait the EOP.
  * @param  hqspi: QSPI handle
  * @param  Timeout
  * @retval None
  */
static uint8_t QSPI_AutoPollingMemReady(QSPI_HandleTypeDef *hqspi, uint32_t Timeout)
{
  QSPI_CommandTypeDef     s_command;
  QSPI_AutoPollingTypeDef s_config;

  /* Configure automatic polling mode to wait for memory ready */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = READ_STATUS_REG_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_NONE;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = 0;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  s_config.Match           = 0;
  s_config.Mask            = MX25L512_SR_WIP;
  s_config.MatchMode       = QSPI_MATCH_MODE_AND;
  s_config.StatusBytesSize = 1;
  s_config.Interval        = 0x10;
  s_config.AutomaticStop   = QSPI_AUTOMATIC_STOP_ENABLE;

  if (HAL_QSPI_AutoPolling(hqspi, &s_command, &s_config, Timeout) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

/**
  * @brief  Configure the QSPI in memory-mapped mode
  * @retval QSPI memory status
  */
static uint8_t BSP_QSPI_EnableMemoryMappedMode(QSPI_HandleTypeDef *hqspi)
{
  QSPI_CommandTypeDef      s_command;
  QSPI_MemoryMappedTypeDef s_mem_mapped_cfg;

  /* Configure the command for the read instruction */
  s_command.InstructionMode   = QSPI_INSTRUCTION_4_LINES;
  s_command.Instruction       = QPI_READ_4_BYTE_ADDR_CMD;
  s_command.AddressMode       = QSPI_ADDRESS_4_LINES;
  s_command.AddressSize       = QSPI_ADDRESS_32_BITS;
  s_command.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
  s_command.DataMode          = QSPI_DATA_4_LINES;
  s_command.DummyCycles       = MX25L512_DUMMY_CYCLES_READ_QUAD_IO;
  s_command.DdrMode           = QSPI_DDR_MODE_DISABLE;
  s_command.DdrHoldHalfCycle  = QSPI_DDR_HHC_ANALOG_DELAY;
  s_command.SIOOMode          = QSPI_SIOO_INST_EVERY_CMD;

  /* Configure the memory mapped mode */
  s_mem_mapped_cfg.TimeOutActivation = QSPI_TIMEOUT_COUNTER_DISABLE;
  s_mem_mapped_cfg.TimeOutPeriod     = 0;

  if (HAL_QSPI_MemoryMapped(hqspi, &s_command, &s_mem_mapped_cfg) != HAL_OK)
  {
    return QSPI_ERROR;
  }

  return QSPI_OK;
}

uint8_t transferSPI(uint8_t send){
	uint8_t rx = 0x00;

	HAL_SPI_TransmitReceive(&hspi5, &send, &rx, sizeof(rx),0x1000);

	return rx;
}

//======== USB->USART =======
void USB_Print(char* string){

	uint16_t len = strlen (string);
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

		//intDRDY = true;
		//if(intDRDY){
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_8, GPIO_PIN_SET);
		Red_ADS1198_DMA();


		//}


	}

	if(GPIO_Pin == GPIO_PIN_0){

		pressbotao = true;
	}
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi){

	if(hspi->Instance == SPI5){

	Red_ADS1198_DMA_Complete();

	}
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == UART5)
  {
	  HAL_UART_Receive_IT(&huart5, &ESPcaracter, 1);
	  ESPRxData[RecievLen++] =  ESPcaracter;
  }
}


/* USER CODE END 4 */

/* USER CODE BEGIN Header_panTompinksCalculation_Task */
/**
  * @brief  Function implementing the DetectordePicos thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_panTompinksCalculation_Task */
void panTompinksCalculation_Task(void *argument)
{
  /* USER CODE BEGIN 5 */
	 osThreadSuspend(DetectordePicosHandle);
  for(;;)
  {
		  panTompkins();
		  osThreadSuspend(DetectordePicosHandle);
  }

  /* USER CODE END 5 */
}

/* USER CODE BEGIN Header_StartInitESPTask */
/**
* @brief Function implementing the InitESPTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartInitESPTask */
void StartInitESPTask(void *argument)
{
  /* USER CODE BEGIN StartInitESPTask */
	osThreadSuspend(InitESPTaskHandle);
  /* Infinite loop */
  for(;;)
  {
	  bufclear(ESPRxData);
	  RecievLen = 0;
	  HAL_GPIO_WritePin(WIFI_CH_PD_GPIO_Port, WIFI_CH_PD_Pin, GPIO_PIN_SET); //esp ligado
	  switch (ComandoATselect)
	  {
	  case 0:
		  SendComandAT("AT+RST\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 1:
		  SendComandAT("AT\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 2:
		  SendComandAT("AT+CWMODE=1\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 3:
		  SendComandAT("AT+CIPMODE=0\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 4:
		  SendComandAT("AT+CIPMUX=0\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 5:
		  osThreadResume(BuscarRedesTaskHandle);
		  ComandoATselect++;
		  break;
	  default:
		  break;
	  }

	  osThreadSuspend(InitESPTaskHandle);
  }
  /* USER CODE END StartInitESPTask */
}

/* USER CODE BEGIN Header_StartVerifErrorTask */
/**
* @brief Function implementing the VerifErrorTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartVerifErrorTask */
void StartVerifErrorTask(void *argument)
{
  /* USER CODE BEGIN StartVerifErrorTask */
	uint32_t tick;
	osThreadSuspend(VerifErrorTaskHandle);
	/* Infinite loop */
	for(;;)
	{

		if(ComandoATselect<10){
			tick = osKernelGetTickCount();
			tick += 6000U;                      // delay 3000 ticks periodically
			osDelayUntil(tick);
			if(VerificaErro(ESPRxData, "\r\r\n\r\n")){//se ocorreu error
				ErrorTentativas++;
				if(ErrorTentativas>2){
					ComandoATselect = 99;
					PopupError = true;
					strcpy(BufferErrorPopup, "Erro de inicialização do ESP\nPor favor, verifique a conexão com o Módulo\ne inicialize o dispositivo novamente");
				}
				//ComandoATselect nao sera atualizado
			}else{//nao ocorreu error
				ErrorTentativas=0;
				ComandoATselect++;
				USB_Print(ESPRxData);
			}
			osThreadResume(InitESPTaskHandle);
		}else if(ComandoATselect==10){
			tick = osKernelGetTickCount();
			tick += 8000U;                      // delay 3000 ticks periodically
			osDelayUntil(tick);
			if (VerificaErro(ESPRxData, "\r\n\r\n")) {		//se ocorreu error
				//ComandoATselect nao sera atualizado
				ErrorTentativas++;
				if(ErrorTentativas>2){
					ComandoATselect = 99;
					PopupError = true;
					strcpy(BufferErrorPopup, "Erro de conexão com o servidor\n");
				}
			} else {				//nao ocorreu error
				ErrorTentativas=0;
				ComandoATselect++;
				USB_Print(ESPRxData);
			}
			osThreadResume(ECGtoESPTaskHandle);
		} else if (ComandoATselect == 11) {
			tick = osKernelGetTickCount();
			tick += 2000U;                      // delay 3000 ticks periodically
			osDelayUntil(tick);
			if (VerificaErro(ESPRxData, "\r\r\n\r\n")) {	//se ocorreu error
				//ComandoATselect nao sera atualizado
				ErrorTentativas++;
				if(ErrorTentativas>2){
					ComandoATselect = 99;
					PopupError = true;
					strcpy(BufferErrorPopup, "Erro de conexão com o servidor\n");
				}
			} else {			//nao ocorreu error
				ErrorTentativas=0;
				ComandoATselect++;
				USB_Print(ESPRxData);
			}
			osThreadResume(ECGtoESPTaskHandle);
		} else if (ComandoATselect == 12) {
			tick = osKernelGetTickCount();
			tick += 2000U;                      // delay 3000 ticks periodically
			osDelayUntil(tick);
					if (VerificaErro(ESPRxData, "\r\r\n\r\n")) {	//se ocorreu error
						//ComandoATselect nao sera atualizado
						ErrorTentativas++;
						if(ErrorTentativas>2){
							ComandoATselect = 99;
							PopupError = true;
							strcpy(BufferErrorPopup, "Erro de conexão com o servidor\n");
						}
					} else {			//nao ocorreu error
						ErrorTentativas=0;
						//ComandoATselect++;
						USB_Print(ESPRxData);
					}
					osThreadResume(ECGtoESPTaskHandle);
		}else if (ComandoATselect == 13) {
			tick = osKernelGetTickCount();
			tick += 2000U;                      // delay 3000 ticks periodically
			osDelayUntil(tick);
			if (VerificaErro(ESPRxData, "\r\r\n\r\n")) {	//se ocorreu error
				//ComandoATselect nao sera atualizado
				ErrorTentativas++;
				if(ErrorTentativas>2){
					ComandoATselect = 99;
					PopupError = true;
					strcpy(BufferErrorPopup, "Erro de conexão com o servidor\n");
				}
			} else {			//nao ocorreu error
				ErrorTentativas=0;
				USB_Print(ESPRxData);
				USB_Print("\n\n\rTransmissao PATCHs concluida");
			}
			osThreadResume(ECGtoESPTaskHandle);
		}else if(ComandoATselect == 14) {
			ComandoATselect++;
			osThreadResume(ECGtoESPTaskHandle);
		}else if(ComandoATselect == 15) {
			if (VerificaErro(ESPRxData, "\r\r\n\r\n")) {	//se ocorreu error
							//ComandoATselect nao sera atualizado
							ErrorTentativas++;
							if(ErrorTentativas>2){
								ComandoATselect = 99;
								PopupError = true;
								strcpy(BufferErrorPopup, "Erro de conexão com o servidor\n");
							}
						} else {			//nao ocorreu error
							ErrorTentativas=0;
							ComandoATselect++;
							USB_Print(ESPRxData);
						}
						osThreadResume(ECGtoESPTaskHandle);
		}


		osThreadSuspend(VerifErrorTaskHandle);
	}
  /* USER CODE END StartVerifErrorTask */
}

/* USER CODE BEGIN Header_StartBuscarRedesTask */
/**
* @brief Function implementing the BuscarRedesTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartBuscarRedesTask */
void StartBuscarRedesTask(void *argument)
{
  /* USER CODE BEGIN StartBuscarRedesTask */
	uint32_t tick;
	osThreadSuspend(BuscarRedesTaskHandle);

  /* Infinite loop */
  for(;;)
  {
	  bufclear(ESPRxData);
	  RecievLen = 0;
	  /********* AT+CWLAP **********/
	  SendComandAT("AT+CWLAP\r\n"); //retorna as redes disponíveis para conexão em ESPRxData

	  //aguarda a respota do ESP
	  tick = osKernelGetTickCount();
	  tick += 3000U;                      // delay 3000 ticks periodically
	  osDelayUntil(tick);

	  NumeroRedes = BuscarRedes(ESPRxData); //verifica erro na resposta do esp, separa as redes, retorna o numero de redes disponiveis

	  if(NumeroRedes == -1){
			PopupError = true;
			strcpy(BufferErrorPopup, "Erro ao buscar as redes\nPor favor, tente novamente\n e/ou reinicie o dispositivo");
	  }

	  AtualRedesOn = true;

	  osThreadSuspend(BuscarRedesTaskHandle);
  }
  /* USER CODE END StartBuscarRedesTask */
}

/* USER CODE BEGIN Header_StartConectWiFiTask */
/**
* @brief Function implementing the ConectWiFiTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartConectWiFiTask */
void StartConectWiFiTask(void *argument)
{
  /* USER CODE BEGIN StartConectWiFiTask */
	uint32_t tick;
	char data[50];
	osThreadSuspend(ConectWiFiTaskHandle);
  /* Infinite loop */
  for(;;)
  {
	  bufclear(ESPRxData);
	  RecievLen = 0;
	  ConectarWiFi();

	  //aguarda a respota do ESP
	  tick = osKernelGetTickCount();
	  tick += 15000U;                      // delay 3000 ticks periodically
	  osDelayUntil(tick);

	  if(VerificaErro(ESPRxData, "IP\r\n\r\n")){
		  USB_Print("Erro de conexão com o Wi-Fi!!!");
		  strcpy(BufferErrorPopup, "Erro de conexao com o Wi-Fi!!!\nPor favor, verifique a senha\n e tente novamente\n");
		  PopupError = true;
	  }else{
		  sprintf (data, "Conectado em: \"%s\"", Redes[KeyboardSelection].NomeRede);
		  strcpy(BufferErrorPopup, data);
		  PopupError = true;
		  USB_Print(data);
	  }

	  osThreadSuspend(ConectWiFiTaskHandle);
  }
  /* USER CODE END StartConectWiFiTask */
}

/* USER CODE BEGIN Header_StartECGtoESPTask */
/**
* @brief Function implementing the ECGtoESPTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartECGtoESPTask */
void StartECGtoESPTask(void *argument)
{
  /* USER CODE BEGIN StartECGtoESPTask */
	char jsonobservation[2000];
	char RequestBuffer[2048];
	char RotaID[100];
	char CIPSEND[20];
	uint16_t NumPartesPatch=20;
	uint16_t NumPartesPatchEnviados=0;

	uint32_t tick;
	osThreadSuspend(ECGtoESPTaskHandle);
  /* Infinite loop */
  for(;;)
  {
	  bufclear(ESPRxData);
	  RecievLen = 0;
	  switch (ComandoATselect)
	  {
	  case 10:
		  Read_SD_Data(TransmitECGData, "ECG_001", LenDataECGtoESP);
		  SendComandAT("AT+CIPSTART=\"TCP\",\"cloudecg-env.eba-mau7x2gw.us-east-1.elasticbeanstalk.com\",80\r\n");
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 11:
		  constroijsonO(POST,"preliminary", TransmitECGData, jsonobservation, 0); //1min de ECG tem 100 partes de 300 amostras
		  requesthttp (POST, "/baseR4/Observation", "cloudecg-env.eba-mau7x2gw.us-east-1.elasticbeanstalk.com\r\n", "application/json\r\n","application/json\r\n", strlen(jsonobservation) , jsonobservation, RequestBuffer);
		  sprintf(CIPSEND, "AT+CIPSEND=%d\r\n", strlen(RequestBuffer));
		  SendComandAT(CIPSEND);
		  osThreadResume(VerifErrorTaskHandle);
		  break;
	  case 12:
		  //USB_Print(RequestBuffer);
		  SendComandAT(RequestBuffer);
		  tick = osKernelGetTickCount();
		  tick += 12000U;                      // delay 3000 ticks periodically
		  osDelayUntil(tick);
		  USB_Print(ESPRxData);

		  NumPartesPatchEnviados++;

		  if(NumPartesPatchEnviados==1){
		  ExtrairID(ESPRxData, pacienteID);
		  strcpy(RotaID, "/baseR4/Observation/");
	      strcat(RotaID, pacienteID);
		  USB_Print("\r\n\r\n");
		  //USB_Print(RotaID);
		  }
		  bufclear(ESPRxData);
		  RecievLen = 0;
		  if(NumPartesPatchEnviados < NumPartesPatch){
		  constroijsonO(PATCH,"preliminary", TransmitECGData, jsonobservation, NumPartesPatchEnviados);
		  requesthttp(PATCH, RotaID, "cloudecg-env.eba-mau7x2gw.us-east-1.elasticbeanstalk.com\r\n", "application/json\r\n","application/json\r\n", strlen(jsonobservation) , jsonobservation, RequestBuffer);
		  sprintf(CIPSEND, "AT+CIPSEND=%d\r\n", strlen(RequestBuffer));
		  //USB_Print(RequestBuffer);
		  SendComandAT(CIPSEND);
		  osThreadResume(VerifErrorTaskHandle);
		  }else{
			  ComandoATselect++;
     		  constroijsonO(PUT,"\"Final\"\n\r}", TransmitECGData, jsonobservation, NumPartesPatchEnviados);
     		  requesthttp(PUT, RotaID, "cloudecg-env.eba-mau7x2gw.us-east-1.elasticbeanstalk.com\r\n", "application/json\r\n","application/json\r\n", strlen(jsonobservation) , jsonobservation, RequestBuffer);
			  sprintf(CIPSEND, "AT+CIPSEND=%d\r\n", strlen(RequestBuffer));
			  SendComandAT(CIPSEND);
			  osThreadResume(VerifErrorTaskHandle);
		  }
		  break;
	  case 13:
		  SendComandAT(RequestBuffer);
		  tick = osKernelGetTickCount();
		  tick += 12000U;                      // delay 3000 ticks periodically
		  osDelayUntil(tick);
		  USB_Print(ESPRxData);
	      ADS_START();
		  ADS_RDATAC();
		  ECGOn = true;
		  break;
	  default:
		  break;
	  }
	  osThreadSuspend(ECGtoESPTaskHandle);
  }
  /* USER CODE END StartECGtoESPTask */
}

/* USER CODE BEGIN Header_StartTrasmECGOnTask */
/**
* @brief Function implementing the TrasmECGOnTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartTrasmECGOnTask */
void StartTrasmECGOnTask(void *argument)
{
  /* USER CODE BEGIN StartTrasmECGOnTask */
	osThreadSuspend(TrasmECGOnTaskHandle);
  /* Infinite loop */
  for(;;)
  {

	  if(TransmissaoECGtoESPOn){
		  TransmissaoECGtoESPOn = false;
		  ADS_STOP();
		  ECGOn = false;
		  osThreadResume(ECGtoESPTaskHandle);
		  osThreadSuspend(TrasmECGOnTaskHandle);
	  }
    osDelay(500);
  }
  /* USER CODE END StartTrasmECGOnTask */
}

/* USER CODE BEGIN Header_StartRealiseDataBPMTask */
/**
* @brief Function implementing the RealiseDataBPMT thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartRealiseDataBPMTask */
void StartRealiseDataBPMTask(void *argument)
{
  /* USER CODE BEGIN StartRealiseDataBPMTask */
  /* Infinite loop */
	osThreadSuspend(RealiseDataBPMTHandle);
  for(;;)
  {
	  if(panTompPointerR != panTompPointerW){
		  osThreadFlagsSet(DetectordePicosHandle,1);
		  osThreadSuspend(RealiseDataBPMTHandle);
	  }

	  osDelay(100);
  }
  /* USER CODE END StartRealiseDataBPMTask */
}

/* USER CODE BEGIN Header_StartGetBatteryChargeTask */
/**
* @brief Function implementing the GetBatteryTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartGetBatteryChargeTask */
void StartGetBatteryChargeTask(void *argument)
{
  /* USER CODE BEGIN StartGetBatteryChargeTask */
	uint16_t adc_battery_buff[BUFF_LEN_AVGR_BATTERY];
	uint8_t moving_average_index = 0;
	float avrg_battery_charge = 0.0;

	HAL_ADC_Start(&hadc3);
	HAL_ADC_PollForConversion(&hadc3, 100);
	adc_battery_buff[0] = (uint16_t)HAL_ADC_GetValue(&hadc3);
	HAL_ADC_Stop(&hadc3);


	for(uint8_t i=1; i<BUFF_LEN_AVGR_BATTERY; i++){
		adc_battery_buff[i] = adc_battery_buff[0];
	}
  /* Infinite loop */
  for(;;)
  {
		HAL_ADC_Start(&hadc3);
		HAL_ADC_PollForConversion(&hadc3, 100);
		adc_battery_buff[moving_average_index] = (uint16_t)HAL_ADC_GetValue(&hadc3);
		HAL_ADC_Stop(&hadc3);


		moving_average_index++;

		if(moving_average_index == BUFF_LEN_AVGR_BATTERY)
		{
			moving_average_index = 0;
		}


		for(uint8_t i=0; i<BUFF_LEN_AVGR_BATTERY; i++){
			if(i==0){
				avrg_battery_charge = (float) adc_battery_buff[i];
			}else{
				avrg_battery_charge += (float) adc_battery_buff[i];
			}
		}

		avrg_battery_charge = avrg_battery_charge/BUFF_LEN_AVGR_BATTERY;

		avrg_battery_charge = 0.0044535*avrg_battery_charge + 0.7291520; //calibracao ADC baterry

		if( round(100*(avrg_battery_charge - 10.2)/(12.6 - 10.2)) > 100){
			Percent_battery = 100;
		}else if(round(100*(avrg_battery_charge - 10.2)/(12.6 - 10.2)) < 0){
			Percent_battery = 0;
		}else{
			Percent_battery = (uint8_t) round(100*(avrg_battery_charge - 10.2)/(12.6 - 10.2));
		}

		UpdateBatChargeLevel = true;
		osDelay(30000);
  }
  /* USER CODE END StartGetBatteryChargeTask */
}

/* USER CODE BEGIN Header_StartWrittenSDTask */
/**
* @brief Function implementing the WrittenSDTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartWrittenSDTask */
void StartWrittenSDTask(void *argument)
{
  /* USER CODE BEGIN StartWrittenSDTask */
	uint8_t on_off_ECGtoSD = 1;

	  Init_MicroSD();
	  osThreadSuspend(WrittenSDTaskHandle);
  /* Infinite loop */
  for(;;)
  {
	//osThreadFlagsWait(0x0010, osFlagsWaitAny, osWaitForever);
	  if(on_off_ECGtoSD)
	 	  {
		  on_off_ECGtoSD = 0;
	 		  if(start_stop_recordingSD)
	 		  {
	 			  ADS_STOP();
	 			  ECGOn = false;
//	 			  if(counter_circularbuff<255)
//	 			  {
//	 				  write2ecg_file((uint8_t *) ECGchannelData, counter_circularbuff*16);
//	 			  }
//	 			  else if(counter_circularbuff>255 && counter_circularbuff<511)
//	 			  {
//	 				  write2ecg_file(((uint8_t *) ECGchannelData) + ECG_WRITE_DATA_SIZE, (counter_circularbuff - 255)*16);
//	 			  }
	 			  stop_recording();
	 			  start_stop_recordingSD = 0;
	 			  ADS_START();
	 			  ADS_RDATAC();
	 			  ECGOn = true;
	 			  TransmissaoECGtoESPOn = true;
	 			  osThreadSuspend(WrittenSDTaskHandle);
	 		  }
	 		  else
	 		  {
	 			  ADS_STOP();
	 			  ECGOn = false;
	 			  start_newPacient_recording(); //Com o Header incluso
	 			  start_stop_recordingSD = 1;
	 			  counter_circularbuff = 0;
	 			  half_buffer_ECG = 0;
				  full_buffer_ECG = 0;
	 			  ADS_START();
	 			  ADS_RDATAC();
	 			  ECGOn = true;
	 		  }
	 	  }
	  if(start_stop_recordingSD == 1 && half_buffer_ECG == 1)
	  {
		  write2ecg_file((uint8_t *) ECG_12Dev, ECG_WRITE_DATA_SIZE);
		  half_buffer_ECG = 0;
	  }
	  if(start_stop_recordingSD == 1 && full_buffer_ECG == 1)
	  {
		  write2ecg_file(((uint8_t *) ECG_12Dev) + ECG_WRITE_DATA_SIZE, ECG_WRITE_DATA_SIZE);
		  full_buffer_ECG = 0;
		  num_packet++;
		  if(num_packet == 60)
		  {
			  on_off_ECGtoSD = 1;
		  }
	  }
    osDelay(1);
  }
  /* USER CODE END StartWrittenSDTask */
}

/* USER CODE BEGIN Header_StartExperimentoTask */
/**
* @brief Function implementing the ExperimentoTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartExperimentoTask */
void StartExperimentoTask(void *argument)
{
  /* USER CODE BEGIN StartExperimentoTask */
	osThreadSuspend(ExperimentoTaskHandle);

	  ADS_STOP();
	  ECGOn = false;

	  first_time_filter_HP = 0;
	  first_time_filter_notch = 0;

	  Init_exp = 1;
	  //DataToSD_Clean_ECG
	  //DataToSD_BWSignal0dB
	  //DataToSD_BWeECG_0dB
	  Read_SD_Data(ECG_withNoise_EXP, "DataToSD_Clean_ECG", 4*SIZEECGEXP);
	  ADS_START();
	  ADS_RDATAC();
	  ECGOn = true;

  /* Infinite loop */
  for(;;)
  {
	  if(Fim_EXP){
		  Fim_EXP = 0;
		  ADS_STOP();
		  ECGOn = false;
		  //CleanECG_Filtrado_MCU
		  CreatNewFile_write("CleanECG_Filtrado_MCU_60Hz"); //sem o Header
		  Write_File((uint8_t *) ECG_withNoise_EXP, 4*SIZEECGEXP);
		  Init_exp = 0;
		  osThreadSuspend(ExperimentoTaskHandle);
	  }
    osDelay(1);
  }
  /* USER CODE END StartExperimentoTask */
}

/* USER CODE BEGIN Header_StartmainTask */
/**
* @brief Function implementing the mainTask thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_StartmainTask */
void StartmainTask(void *argument)
{
  /* USER CODE BEGIN StartmainTask */
  /* Infinite loop */
  for(;;)
  {
	  if(Start_ECG_to_SD == 1){
		  Start_ECG_to_SD = 0;
		  osThreadResume(WrittenSDTaskHandle);
	  }

	  if(pressbotao == 1){
		  pressbotao = 0;
		  osThreadResume(ExperimentoTaskHandle);
	  }


    osDelay(1);
  }
  /* USER CODE END StartmainTask */
}

/* MPU Configuration */

void MPU_Config(void)
{
 MPU_Region_InitTypeDef MPU_InitStruct = {0};

  /* Disables the MPU */
  HAL_MPU_Disable();

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.BaseAddress = 0x0;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512KB;
  MPU_InitStruct.SubRegionDisable = 0x0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER1;
  MPU_InitStruct.BaseAddress = 0x90000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER2;
  MPU_InitStruct.Size = MPU_REGION_SIZE_64MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER3;
  MPU_InitStruct.BaseAddress = 0xC0000000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_512MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /** Initializes and configures the Region and the memory to be protected
  */
  MPU_InitStruct.Number = MPU_REGION_NUMBER4;
  MPU_InitStruct.Size = MPU_REGION_SIZE_16MB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_BUFFERABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);
  /* Enables the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}

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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
