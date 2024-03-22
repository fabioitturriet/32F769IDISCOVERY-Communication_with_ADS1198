/*
 * ESP8266.c
 *
 *  Created on: Jan 10, 2023
 *      Author: Patrick
 */

/*------------------------------- INCLUDES -----------------------------*/
#include "ESP8266.h"
#include "UartRingbuffer_multi.h"
#include "stdio.h"
#include "string.h"



extern void USB_Print(char* string);

extern void bufclear(char *buf);

extern UART_HandleTypeDef huart5;

/*------------------------------- DEFINES -----------------------------*/
#define wifi_uart &huart5
#define BUFFER_READ_LEN 2048

struct SSID
{
	char NomeRede[32];
	uint16_t Senha[18];
};

struct SSID Redes[10];

struct Cadastro
{
	uint16_t nome[32];
	uint16_t sobrenome[32];
	uint16_t contato[32];
	uint16_t genero;
	uint16_t NascDia[3];
	uint16_t NascMes[3];
	uint16_t NascAno[5];
};

struct Cadastro NewPacient;


extern uint16_t KeyboardSelection;

HAL_StatusTypeDef HAL_respost;

enum request {POST, GET, PATCH, PUT, DELETE, OBSERVATION, PATIENT};

char buffer[20];

bool connectedWiFi;
/*------------------------------- FUNÇÕES -----------------------------*/
void ESP_init()
{
	for(uint8_t i=0; i<32;i++){
		NewPacient.nome[i] = 0;
		NewPacient.sobrenome[i] = 0;
		NewPacient.contato[i] = 0;
	}
	for(uint8_t i=0; i<3;i++){
		NewPacient.NascDia[i] = 0;
		NewPacient.NascMes[i] = 0;
	}
	for(uint8_t i=0; i<5;i++){
		NewPacient.NascAno[i] = 0;
	}
	NewPacient.genero = 0;


}

void WiFi_OFF()
{
	HAL_GPIO_WritePin(WIFI_CH_PD_GPIO_Port, WIFI_CH_PD_Pin, GPIO_PIN_RESET);
}

int8_t BuscarRedes(char *CWLAPResponse)
{
	char Marcador[7] = {'+', 'C', 'W', 'L', 'A', 'P', ':'};
	uint8_t count=0;
	uint8_t numCaracter=0;

	if(VerificaErro(CWLAPResponse, "\r\n\r\n")){
		return -1;
	}
	/********* SEPARAÇÃO DAS REDES **********/
	uint8_t NumeroRedes=-1;

	for(int i=0; i<strlen(CWLAPResponse); i++)
	{
		if(CWLAPResponse[i]==Marcador[count]){
			if(count == 6){
				count = -1;
				NumeroRedes++;
				memset(Redes[NumeroRedes].NomeRede, '\0', 32);
				while(CWLAPResponse[i]!='"') i++;
				for(int j=i+1; j<i+26; j++)
				{
					Redes[NumeroRedes].NomeRede[numCaracter] = CWLAPResponse[j];
					numCaracter++;
					if(CWLAPResponse[j+1]=='"'){
						break;
					}
				}
				numCaracter = 0;
			}
			count ++;

		}else{

			count = 0;
		}
	}

	bufclear(CWLAPResponse);
	USB_Print("pesquisa de wifi completa\n\r\n");

	return NumeroRedes+1;

}

void ConectarWiFi()
{
	char data[80];
	char senha[18];
	uint8_t len = 0;
	uint16_t *ptr = &Redes[KeyboardSelection].Senha[0];

	while(*ptr++)
		len++;

	for(uint8_t i=0; i<len; i++){
		senha[i]=Redes[KeyboardSelection].Senha[i];
	}
	senha[len] = '\0';

	/********* AT+CWJAP="SSID","PASSWD" **********/
	USB_Print("connecting... to the provided AP\n\r");
	sprintf (data, "AT+CWJAP=\"%s\",\"%s\"\r\n", Redes[KeyboardSelection].NomeRede, senha);
	USB_Print(data);
	SendComandAT(data);
}

AT_StatusTypeDef transmitATcommand(const char *string)
{
	char ErrorCheck[15];

	Uart_sendstring(string, wifi_uart);
	while (!(Wait_for("\r\r\n\r\n", wifi_uart)));
	while (!(Copy_upto ("\r\n", ErrorCheck, wifi_uart)));

	for(uint8_t i=0; i<15; i++){
		if(ErrorCheck[i]== '\n'){
			ErrorCheck[i+1] = '\0';
			break;
		}
	}
	if(strcmp(ErrorCheck, "OK\r\n") == 0){
		return AT_OK;
	}else if(strcmp(ErrorCheck, "ERROR\r\n") == 0){
		return AT_ERROR;
	}else if(strcmp(ErrorCheck, "busy p...\r\n") == 0){
		return AT_BUSY;
	}else{
		return AT_OK;
	}
}

AT_StatusTypeDef rodaComandoAT(char* comandoAT, uint32_t delay){

	char Rx_data[BUFFER_READ_LEN];
	bufclear(Rx_data);
 	uint16_t msgtoESP_size= strlen(comandoAT);


 	HAL_respost = HAL_UART_Transmit(wifi_uart, (uint8_t *) comandoAT, msgtoESP_size, HAL_MAX_DELAY);
 	HAL_respost = HAL_UART_Receive(wifi_uart, (uint8_t *)Rx_data, BUFFER_READ_LEN, delay);
 	  return VerificaErro(Rx_data, "\r\r\n\r\n");
 }

AT_StatusTypeDef VerificaErro(char *StringReceived, char *Marcacao){
	//uint16_t msgReceivedLen = strlen(StringReceived);
	//uint16_t EsperarAte_Counter = 0;
	//uint16_t controle = strlen(EsperarAte);
	uint16_t i=0;

	uint8_t msgErrorLen=0;
	char ErrorCheck[15];


//	while(i<msgReceivedLen){
//		if(EsperarAte[EsperarAte_Counter] == StringReceived[i]){
//			EsperarAte_Counter++;
//		}else{
//			EsperarAte_Counter = 0;
//		}
//		i++;
//		if(EsperarAte_Counter == controle){
//			break;
//		}
//
//	}
	if(EsperarAte(StringReceived, Marcacao, &i)){
		while(StringReceived[i] != '\n'){
			ErrorCheck[msgErrorLen]=StringReceived[i];
			msgErrorLen++;
			i++;
		}
		ErrorCheck[msgErrorLen]= '\n';
		ErrorCheck[msgErrorLen+1]= '\0';

		if(strcmp(ErrorCheck, "OK\r\n") == 0){
			return AT_OK;
		}else if(strcmp(ErrorCheck, "ERROR\r\n") == 0){
			return AT_ERROR;
		}else if(strcmp(ErrorCheck, "busy p...\r\n") == 0){
			return AT_BUSY;
		}else{
			return AT_OK;
		}
	}else{
		USB_Print("erro de comunicação com o ESP\r\n");
		return AT_ERROR;
	}



}

void VerificaConexaoWiFi()
{
	char resposta[150];
	uint16_t i=0;
	char ssidRede[31];
	uint8_t Auxlen=0;

	bufclear(resposta);

	HAL_respost = HAL_UART_Transmit(wifi_uart, (uint8_t *)"AT+CWJAP?\r\n", strlen("AT+CWJAP?\r\n"), HAL_MAX_DELAY);
	HAL_respost = HAL_UART_Receive(wifi_uart, (uint8_t *)resposta, 150, 2000);

	if(EsperarAte(resposta, "\r\r\n", &i))
	{
		if(resposta[i] == 'N'){
			connectedWiFi = false; //nenhum WiFi conectado
		}else if(resposta[i] == '+'){
			connectedWiFi = true; //conectado a uma rede
			EsperarAte(resposta, "\"", &i);
			while(resposta[i] != '\"'){
				ssidRede[Auxlen]=resposta[i];
					Auxlen++;
					i++;
				}
			ssidRede[Auxlen] = '\0';
		}

	}else{
		USB_Print("erro de comunicação com o ESP\r\n");
	}
	USB_Print(resposta);
}

bool EsperarAte(char *StringReceived, char *Marcacao, uint16_t *ptr)
{
	uint16_t msgReceivedLen = strlen(StringReceived);
	uint16_t EsperarAte_Counter = 0;
	uint16_t controle = strlen(Marcacao);

	while(*ptr<msgReceivedLen){
			if(Marcacao[EsperarAte_Counter] == StringReceived[*ptr]){
				EsperarAte_Counter++;
			}else{
				EsperarAte_Counter = 0;
			}
			*ptr+=1;
			if(EsperarAte_Counter == controle){
				return 1; //marcaçao encontrada
			}

		}
		return 0; //marcação não encontrada
}

void SendComandAT(char* comandoAT)
{
	HAL_UART_Transmit(wifi_uart, (uint8_t *)comandoAT, strlen(comandoAT), HAL_MAX_DELAY);
}

void ServerInit()
{
	char jsonobservation[2048];
   	int ECGData[300]={11955, 122, 13, 14, 15, 16, 1722, 18, 19, 20, 2, 21, 21, 21, 22, 22, 22, 22, 23, 22, 22, 22, 21, 20, 20, 19, 18, 18, 17, 16, 15, 15, 14, 13, 13, 13, 12, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 12, 11, 11, 12, 11, 11, 12, 11, 9, 9, 9, 10, 12, 16, 22, 28, 37, 46, 54, 63, 70, 77, 80, 81, 79, 74, 67, 59, 49, 40, 31, 22, 15, 11, 7, 6, 6, 6, 6, 7, 8, 8, 9, 10, 11, 11, 12, 12, 11, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 14, 16, 17, 18, 19, 19, 20, 21, 22, 23, 24, 24, 24, 25, 25, 26, 27, 27, 27, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 28, 27, 27, 26, 25, 24, 23, 23, 22, 21, 20, 19, 18, 16, 16, 15, 14, 13, 13, 12, 12, 12, 12, 11, 12, 12, 11, 12, 12, 12, 12, 12, 12, 11, 12, 12, 11, 12, 12, 11, 11, 12, 12, 12, 12, 12, 11, 12, 12, 12, 12, 12, 12, 11, 12, 12, 11, 12, 12, 12, 12, 12, 12, 11, 12, 12, 12, 12, 12, 11, 11, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 12, 12, 11, 12, 12, 11, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 12, 13, 14, 16, 17, 17, 19, 19, 20, 20, 21, 21, 21, 22, 22, 22, 23, 23, 22, 22, 22, 21, 20, 20, 19, 18, 18, 17, 16, 15, 15, 14, 13, 13, 13, 12, 12, 12, 11, 11, 12, 12, 11, 12, 12, 11, 12, 12};
	char RequestBuffer[2048];
	char CIPSEND[20];

	constroijsonO(POST,"preliminary", ECGData, jsonobservation, 0);

	requesthttp (POST, "/baseR4/Observation", "cloudecg-env.eba-mau7x2gw.us-east-1.elasticbeanstalk.com\r\n", "application/json\r\n","application/json\r\n", strlen(jsonobservation) , jsonobservation, RequestBuffer);

	sprintf(CIPSEND, "AT+CIPSEND=%d\r\n", strlen(RequestBuffer));

	//envia para o CIPSEND para o esp
	//envia para o esp o RequestBuffer
}

void constroijsonO(uint8_t type, char* status, int* ECGData, char* JSONBuffer,  uint8_t PartePatch){

	 uint16_t ECGData_len = (uint16_t) size(ECGData);
	 char DataAux[5];
	 int i=0;

	bufclear(JSONBuffer);
 	if(type==POST){
 		//HAL_UART_Transmit(&huart2, (uint8_t*)"\n\nconstroijsonO\n", strlen("\n\nconstroijsonO\n"), HAL_MAX_DELAY);
 		strcpy(JSONBuffer, "{\r\n\"resourceType\": \"Observation");
 		strcat(JSONBuffer, "\",\r\n\"status\": \"");
 		strcat(JSONBuffer, status);
 		strcat(JSONBuffer, "\",\r\n\r\n\"component\": [\r\n{\r\n\"valueSampledData\": {\r\n\r\n\"data\": \"");

 		for(i=PartePatch*300; i<(PartePatch+1)*300; i++){
 			sprintf(DataAux, "%d ", ECGData[i]);
 			strcat(JSONBuffer, DataAux);
 		}

 		strcat(JSONBuffer, "\"\r\n}\r\n}\r\n]\r\n}");
 	}
 	if(type==PATCH){
 		strcpy(JSONBuffer, "[\r\n {\r\n \"op\": \"add");
 		strcat(JSONBuffer, "\",\r\n \"path\": \"/component/");
 		sprintf(DataAux, "%d", PartePatch);
 		strcat(JSONBuffer, DataAux);
 		strcat(JSONBuffer, "/valueSampledData/data");
 		strcat(JSONBuffer, "\",\r\n \"value\": \"");

 		for(i=PartePatch*300; i<(PartePatch+1)*300; i++){
 		 			sprintf(DataAux, "%d ", ECGData[i]);
 		 			strcat(JSONBuffer, DataAux);
 		 		}
 		strcat(JSONBuffer, "\"\r\n}\r\n]");

 	}
 	if(type==PUT){
 		strcpy(JSONBuffer, "{\r\n \"Status\": ");
 		strcat(JSONBuffer, status);
 	}

}

void requesthttp(uint8_t type, char* rota, char* host, char* accept, char* content_type, int length, char* json, char* RequestBuffer){

 	char DataAux[5];

	bufclear(RequestBuffer);

	if (type == POST) {
		strcpy(RequestBuffer, "POST ");
	} else if (type == PATCH) {
		strcpy(RequestBuffer, "PATCH ");
	} else if (type == GET) {
		strcpy(RequestBuffer, "GET ");
	} else if (type == PUT) {
		strcpy(RequestBuffer, "PUT ");
	}

	strcat(RequestBuffer, rota);
 	strcat(RequestBuffer, " HTTP/1.1\r\n");
 	strcat(RequestBuffer, "Host: ");
 	strcat(RequestBuffer, host);
 	strcat(RequestBuffer, "Accept: ");
 	strcat(RequestBuffer, accept);
 	strcat(RequestBuffer, "Content-Type: ");
 	strcat(RequestBuffer, content_type);
 	strcat(RequestBuffer, "Content-Length: ");
 	sprintf(DataAux, "%d", length);
 	strcat(RequestBuffer, DataAux);
 	strcat(RequestBuffer, "\r\n\r\n");
 	strcat(RequestBuffer, json);
 }


int size(int *valores){
	int len=0;

	while((*valores++) != 0) len++;

	return len;
}

void ExtrairID(char* ESPRxData, char* IDobservation)
{
	uint16_t msglen = strlen(ESPRxData);
	uint16_t j=0;
	uint16_t i=0;
	for(i=0; i<msglen;i++){
		if(ESPRxData[i] == 'i' && ESPRxData[i+1] == 'd'){
			break;
		}
	}

	while(ESPRxData[i+5]!='"'){
		IDobservation[j]=ESPRxData[i+5];
		i++;
		j++;
	}
	IDobservation[j]='\0';
}
