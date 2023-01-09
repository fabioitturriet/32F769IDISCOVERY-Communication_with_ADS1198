#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"



int16_t dado =0;
 int espaco, count;
 uint8_t PD, MUX;
 int MediaEscala;
 int16_t SetMaxEscala;
 int16_t SetMinEscala;



#define BufferLenPlotECG 250

 extern __IO float valorECG[BufferLenPlotECG];
 extern __IO uint8_t escrita;
 extern __IO uint8_t leitura;

 extern __IO uint8_t CanaisSet;

 int conta;

 extern "C" {
 	 extern void ADS_WREG(uint8_t _address, uint8_t _value);
 	 extern void SetCanaisIguais(uint8_t setcanais);

 	 extern void ADS_START();
 	 extern void ADS_RDATAC();
 	 extern void ADS_SDATAC();
 }



Model::Model() : modelListener(0)
{

}

void Model::tick()
{
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
			modelListener->UpdateGraph(valorECG[leitura]);
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
			modelListener->UpdateGraph(valorECG[leitura]);
		}
	}
}

void Model::M_1kSPS_selected()
{
	ADS_WREG(0x01,0x03);
}

void Model::M_500SPS_selected()
{
	ADS_WREG(0x01,0x04);
}

void Model::M_250SPS_selected()
{
	ADS_WREG(0x01,0x05);
}

void Model::M_125SPS_selected()
{
	ADS_WREG(0x01,0x06);
}

void Model::M_Gain12_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x60 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain8_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x50 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain6_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x00 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain4_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x40 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain3_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x30 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain2_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x20 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::M_Gain1_selected()
{
	PD = CanaisSet>>7;
	PD = PD<<7;
	MUX = CanaisSet<<4;
	MUX = MUX>>4;

	CanaisSet = PD | 0x10 | MUX;
	SetCanaisIguais(CanaisSet);
}

void Model::StartRDataC()
{
	ADS_START();
	ADS_RDATAC();
}

void Model::StopRDataC()
{
	ADS_SDATAC();
}

void Model::AjusteEscala()
{
	for(uint8_t i=0; i < BufferLenPlotECG; i++){
		if(i==0){
			MediaEscala= (int) valorECG[i];
		}else{
			MediaEscala+= (int) valorECG[i];
		}
	}
		MediaEscala=MediaEscala/BufferLenPlotECG;
		SetMaxEscala = MediaEscala + 2000;
		SetMinEscala = MediaEscala - 500;

		modelListener -> SetMinMaxEscalaGraph(SetMinEscala, SetMaxEscala);
}



