#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "stm32f7xx_hal.h"
#include "FreeRTOS.h"
#include "queue.h"


int16_t dado =0;
 int espaco, count;
#define BufferLenPlotECG 60
 extern __IO int16_t SWDviwe;

 extern __IO int16_t valorECG[200];
 extern __IO uint8_t escrita;
 extern __IO uint8_t leitura;

 int conta;

 extern "C"
 {
 	 xQueueHandle messageQ;//define a fila criada no main.c
 }

Model::Model() : modelListener(0)
{
	messageQ = xQueueCreate(50, sizeof (int16_t)); //cria uma fila com 120 valores de espaço
}

void Model::tick()
{

//	espaco = uxQueueSpacesAvailable(messageQ);//cada atualização da tela pega o valor de espaço livre na fila
//		espaco = 50 - espaco; //tespaço usado na fila

//		for (count=0; count<espaco; count++){//repete até desocupar a fila

//		xQueueReceive(messageQ, &dado, 0);//pega um dado da fila
if( leitura < escrita){
	conta = escrita - leitura;


	for(int i=0; i<conta; i++){
		leitura ++;
		if(leitura==BufferLenPlotECG){
			leitura = 0;
		}
		modelListener->UpdateGraph(valorECG[leitura]);
}}else{
	conta = (BufferLenPlotECG-leitura) + escrita;
	for(int i=0; i<conta; i++){
		leitura ++;
		if(leitura==BufferLenPlotECG){
			leitura = 0;
		}
		modelListener->UpdateGraph(valorECG[leitura]);
	}
}



//		modelListener->UpdateGraph(SWDviwe);//atualiza o grafico com o dado


//}
//
}
