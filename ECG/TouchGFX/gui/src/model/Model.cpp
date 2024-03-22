#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "cmsis_os.h"


//int16_t dado =0;
//int espaco, count;
 uint8_t PD, MUX, GAIN;
 int OffsetExibe = 0;
 int MediaOffset = 0;
 //int16_t SetMaxEscala;
 //int16_t SetMinEscala;
 bool ECGOn = false;



#define BufferLenPlotECG 250

 extern __IO float valorECG[BufferLenPlotECG];
 extern __IO uint8_t escrita;
 extern __IO uint8_t leitura;

 extern __IO uint8_t CanaisSet;

 extern __IO uint8_t DerivSelecionada;

 extern uint8_t NumeroRedes;

 extern bool AtualRedesOn;

 extern bool TransmiteECGDataWifiOn;

 extern bool PopupError;

 extern uint8_t ComandoATselect;

 extern bool panTompkON;

 extern bool AtualBPM;
 extern uint16_t FS;
// extern uint8_t WINDOWSIZE;
// extern uint16_t BUFFSIZE;
 bool AtualImgBPM;
 uint8_t tickCounter = 0;
 bool iconBPMon = false;

 extern bool StopLoadAnimat;

 extern bool UpdateBatChargeLevel;

 extern bool Lead_OFF_Detected;

 extern TIM_HandleTypeDef htim2;

 extern osThreadId_t InitESPTaskHandle;
 extern osThreadId_t BuscarRedesTaskHandle;
 extern osThreadId_t ConectWiFiTaskHandle;
 extern osThreadId_t TrasmECGOnTaskHandle;
 extern osThreadId_t DetectordePicosHandle;
 extern osThreadId_t CalculaBPMHandle;
 extern osThreadId_t GetBatteryTaskHandle;

 extern bool Filtro60;
 extern bool FiltroBW;

 extern uint8_t first_time_filter_HP;
 extern uint8_t first_time_filter_notch;

 extern volatile uint8_t Start_ECG_to_SD;

 int conta;

 extern "C" {
 	 extern void ADS_WREG(uint8_t _address, uint8_t _value);
 	 extern void SetCanaisIguais(uint8_t setcanais);

 	 extern void ADS_Config12Dev();
 	 extern void ADS_Config3Dev();

 	 extern void ADS_LOFF_DC_Resistor();
 	 extern void ADS_LOFF_DC_CurrentSource();
 	 extern void ADS_LOFF_AC();
 	 extern void ADS_LOFF_OFF();

 	 extern void ADS_START();
 	 extern void ADS_RDATAC();
 	 extern void ADS_SDATAC();
     extern void ADS_STOP();
 	 extern void ESP_init();
 	 extern void WiFi_OFF();

 	 extern int8_t BuscarRedes();

 	 extern void ConectarWiFi();
 }



Model::Model() : modelListener(0)
{

}

void Model::tick()
{
	if(ECGOn){
		if( leitura < escrita)
		{
			conta = escrita - leitura;


			for(int i=0; i<conta; i++)
			{
				leitura ++;
				if(leitura==BufferLenPlotECG)
				{
					leitura = 0;
				}
				modelListener->UpdateGraph(valorECG[leitura] - OffsetExibe);
			}
		}else{
			conta = (BufferLenPlotECG-leitura) + escrita;
			for(int i=0; i<conta; i++)
			{
				leitura ++;
				if(leitura==BufferLenPlotECG)
				{
					leitura = 0;
				}
				modelListener->UpdateGraph(valorECG[leitura]- OffsetExibe );
			}
		}
	}

	if(AtualRedesOn){
		modelListener->DefNumRedes(NumeroRedes);
		modelListener->UpdateScrollList();
		AtualRedesOn = false;
	}

	if(AtualBPM){
		AtualBPM = false;
		iconBPMon = true;
		tickCounter = 0;
		modelListener->AtualizarBPM();
	}

	if(PopupError){
		PopupError = false;
		modelListener->GeraPopup();
	}

	if(iconBPMon){
		tickCounter++;
	}

	if(tickCounter == 12){
		tickCounter++;
		iconBPMon = false;
		modelListener->AtualizarIconHeart();
	}

	if(StopLoadAnimat){
		StopLoadAnimat = false;
		modelListener->PreStopLoadingAnimation();
	}

	if(UpdateBatChargeLevel){
		UpdateBatChargeLevel = false;
		modelListener->UpdateBatChargeLevel();
	}

	if(Lead_OFF_Detected){
		Lead_OFF_Detected = false;
		modelListener->UpdateMatrixLOFF();
	}

}

void Model::M_1kSPS_selected()
{
	ADS_WREG(0x01,0x03);
	FS = 1000;
//	WINDOWSIZE = (uint8_t) ((float)FS * 0.15 + 10);
//	BUFFSIZE = FS + FS/2;
}

void Model::M_500SPS_selected()
{
	ADS_WREG(0x01,0x04);
	FS = 500;
//	WINDOWSIZE = (uint8_t) ((float)FS * 0.15 + 10);
//	BUFFSIZE = FS + FS/2;
}

void Model::M_250SPS_selected()
{
	ADS_WREG(0x01,0x05);
	FS = 250;
//	WINDOWSIZE = (uint8_t) ((float)FS * 0.15 + 10);
//	BUFFSIZE = FS + FS/2;
}

void Model::M_125SPS_selected()
{
	ADS_WREG(0x01,0x06);
	FS = 125;
//	WINDOWSIZE = (uint8_t) ((float)FS * 0.15 + 10);
//	BUFFSIZE = FS + FS/2;
}

void Model::M_Gain12_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x60;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain8_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x50;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain6_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x00;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain4_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x40;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain3_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x30;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain2_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x20;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain1_selected()
{
	PD = CanaisSet & 0x80;
	GAIN = 0x10;
	MUX = CanaisSet & 0x0F;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::StartRDataC()
{
	ADS_START();
	ADS_RDATAC();
	ECGOn = true;
}

void Model::StopRDataC()
{
	ADS_STOP();
	ECGOn = false;
}

void Model::AjusteEscala()
{
	for(uint8_t i=0; i < BufferLenPlotECG; i++){
		if(i==0){
			MediaOffset= (int) valorECG[i];
		}else{
			MediaOffset+= (int) valorECG[i];
		}
	}
	OffsetExibe=MediaOffset/BufferLenPlotECG + 420;
	//	SetMaxEscala = MediaEscala + 2000;
	//	SetMinEscala = MediaEscala - 500;

	//	modelListener -> SetMinMaxEscalaGraph(SetMinEscala, SetMaxEscala);
}

void Model::SetModoNormal()
{
	PD = CanaisSet & 0x80;
	GAIN = CanaisSet & 0x70;
	MUX = 0x00;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::SetModoTeste()
{
	PD = CanaisSet & 0x80;
	GAIN = CanaisSet & 0x70;
	MUX = 0x05;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::SetModoMensRuido()
{
	PD = CanaisSet & 0x80;
	GAIN = CanaisSet & 0x70;
	MUX = 0x01;
	CanaisSet = PD | GAIN | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_InitESP()
{
	ComandoATselect = 0;
	osThreadResume(InitESPTaskHandle);
}

void Model::WiFiESP_OFF()
{
	WiFi_OFF();
}

void Model::AtualizarListaRedes()
{
	osThreadResume(BuscarRedesTaskHandle);
}

void Model::NextDerivSelect()
{
	if(DerivSelecionada < 11){
		DerivSelecionada++;
	}
}

void Model::BackDerivSelect()
{
	if(DerivSelecionada > 0){
		DerivSelecionada--;
	}
}

void Model::MConnectWifi()
{
	osThreadResume(ConectWiFiTaskHandle);
}

void Model::SalvarECGnoSD()
{
// transmite WiFi
//	TransmiteECGDataWifiOn = true;
//	osThreadResume(TrasmECGOnTaskHandle);
	Start_ECG_to_SD = 1;
}

void Model::CalcularBPM()
{
	panTompkON = true;
	osThreadResume(DetectordePicosHandle);
}

void Model::StopPanTompkins()
{
	panTompkON = false;
	//osThreadSuspend(DetectordePicosHandle);
}

void Model::Config12Dev()
{
	ADS_Config12Dev();
}

void Model::Config3Dev()
{
	ADS_Config3Dev();
}

void Model::Set_LOFF_off()
{
	 ADS_LOFF_OFF();
}

void Model::Set_LOFF_DCResistor()
{
	ADS_LOFF_DC_Resistor();
}

void Model::Set_LOFF_DCSource()
{
	ADS_LOFF_DC_CurrentSource();
}

void Model::Set_LOFF_AC()
{
	ADS_LOFF_AC();
}

void Model::ToggleFIltro60()
{
	first_time_filter_notch = 0;
	Filtro60 = !Filtro60;
}

void Model::ToggleFIltroBW()
{
	first_time_filter_HP = 0;
	FiltroBW = !FiltroBW;

}
