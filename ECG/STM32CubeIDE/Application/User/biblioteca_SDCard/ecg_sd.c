/*
 * ecg_sd.c
 *
 *  Created on: 13 de jun de 2023
 *      Author: Patrick
 */


#include "ecg_sd.h"
#include "fatfs.h"
#include "main.h"
#include <string.h>

extern void USB_Print(char* string);

extern struct Cadastro
{
	uint16_t nome[32];
	uint16_t sobrenome[32];
	uint16_t contato[32];
	uint16_t genero;
	uint16_t NascDia[3];
	uint16_t NascMes[3];
	uint16_t NascAno[5];
};
extern struct Cadastro NewPacient;

extern char pacienteID[25];
extern uint16_t FS;

//	Obs: Para leitura correta do arquivo um numero multiplo de 24 (2 bytes x 12 canais) maior que o tamanho
//de ecg_file_header de amostras devem ser ignoradas (Para ecg_file_header = 145,
//descartar 168 amostras iniciais.
// 0 - 24    -> ID do Paciente										{0x00, ..., 0x00}
// 25 - 46   -> Nome do paciente            						{0x00, ..., 0x00}
// 47 - 78   -> Sobrenome do paciente      				    		{0x00, ..., 0x00}
// 79 - 110  -> Contato do paciente  						 		{0x00, ..., 0x00}
// 111 - 112 -> Gênero do paciente		    	     				{0x00, 0x00}
// 113 - 114 -> Dia de nascimento                            		{0x00, 0x00}
// 115-  116 -> Mês de nascimento                             		{0x00, 0x00}
// 117 - 120 -> Ano de nascimento                              		{0x00, 0x00, 0x00, 0x00}
// 121 - 122 -> Número de derivações                        12  	{0x0c, 0x00}
// 123 - 124 -> Taxa de amostragem                          500     {0xf4, 0x01}
// 125 - 126 -> bits por amostra                            16		{0x10, 0x00}
// 127 - 130 -> Nome do dispositivo "BIOT"			        		{0x42, 0x49, 0x4f, 0x54}
// 131 - 134 -> Data do exame    									{0x00, 0x00, 0x00, 0x00}
// 135 - 136 -> Hora do exame                						{0x00, 0x00}
// 137 - 140 -> "data" 												{0x64, 0x61, 0x74, 0x61}
// 141 - 144 -> size of the data section				    		{data section size}
//	data
static uint8_t ecg_file_header[145]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		0x0c, 0x00, 0xf4, 0x01, 0x10, 0x00, 0x42, 0x49, 0x4f, 0x54, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x64, 0x61,
		0x74, 0x61, 0x00, 0x00, 0x00, 0x00};

static FRESULT result;
static FATFS sdCard;
static FIL sdFile;
static uint32_t byteswritten, bytesread;
static uint32_t ECG_file_size;
static uint8_t first_time = 0;


void Init_MicroSD(void)
{
	USB_Print("\r\n\r\nPreparando o cartão SD\r\n\r\n");

	result = f_mount(&sdCard, (TCHAR const*)SDPath, 1);
	if( result != FR_OK)
	{

		USB_Print("errorSD: área de trabalho não criada\r\n");
	}
	else
	{
		USB_Print("Montagem do SD com sucesso\r\n");

	}
}

void start_newPacient_recording()
{
	static char file_name[] = "ECG_000";
	static uint8_t file_counter = 1;
	int file_number_digits = file_counter;
	uint8_t i=0, j=0;

	//defining a file header
	for(i=0; i<24; i++){
		ecg_file_header[i] = pacienteID[i];
	}
	j=0;
	for(i=25; i<strlen16(NewPacient.nome)+25; i++){
		ecg_file_header[i] = (uint8_t) NewPacient.nome[j];
		j++;
	}
	j=0;
	for(i=47; i<strlen16(NewPacient.sobrenome)+47; i++){
		ecg_file_header[i] = (uint8_t) NewPacient.sobrenome[j];
		j++;
	}
	j=0;
	for(i=79; i<strlen16(NewPacient.contato)+79; i++){
		ecg_file_header[i] = (uint8_t) NewPacient.contato[j];
		j++;
	}
	ecg_file_header[111] = (uint8_t) NewPacient.genero;
	ecg_file_header[113] = (uint8_t) NewPacient.NascDia[0];
	ecg_file_header[114] = (uint8_t) NewPacient.NascDia[1];
	ecg_file_header[115] = (uint8_t) NewPacient.NascMes[0];
	ecg_file_header[116] = (uint8_t) NewPacient.NascMes[1];
	ecg_file_header[117] = (uint8_t) NewPacient.NascAno[0];
	ecg_file_header[118] = (uint8_t) NewPacient.NascAno[1];
	ecg_file_header[119] = (uint8_t) NewPacient.NascAno[2];
	ecg_file_header[120] = (uint8_t) NewPacient.NascAno[3];
	ecg_file_header[123] = (uint8_t) FS;
	ecg_file_header[124] = (uint8_t) (FS >> 8);

	// defining a ECG file name
	file_name[6] = file_number_digits%10+48;
	file_number_digits /= 10;
	file_name[5] = file_number_digits%10+48;
	file_number_digits /= 10;
	file_name[4] = file_number_digits%10+48;
	file_counter++;

	// creating a file write
	CreatNewFile_write(file_name);

	ECG_file_size = 0;
}

void write2ecg_file(uint8_t *data, uint16_t data_size)
{

	if(first_time == 0)
	{
		for(int i = 0; i < 145; i++)
		{
			*(data + i) = ecg_file_header[i];
		}
		first_time = 1;
	}

	Write_File(data, data_size);

	ECG_file_size += data_size;
}

void stop_recording()
{
	static char file_size[30];

	ECG_file_size -= 145;

	ecg_file_header[141] = (uint8_t) ECG_file_size;
	ecg_file_header[142] = (uint8_t) (ECG_file_size >> 8);
	ecg_file_header[143] = (uint8_t) (ECG_file_size >> 16);
	ecg_file_header[144] = (uint8_t) (ECG_file_size >> 24);

	sprintf(file_size, "size: %d Bytes\r\n", ECG_file_size);
	USB_Print(file_size);

	f_lseek(&sdFile, 0);
	result = f_write(&sdFile,(void *)ecg_file_header, sizeof(ecg_file_header),(UINT*)&byteswritten);
	if(result != FR_OK)
	{
		USB_Print("error in updating the first sector");
		while(1);
	}
	f_close(&sdFile);
	first_time = 0;
	USB_Print("Gravação finalizada\r\n");
}

void Read_SD_Data(float *data, char* arquivo, int data_size)
{
	// open file read
	result = f_open(&sdFile, arquivo,  FA_READ);
	if(result != FR_OK)
	{
		USB_Print("errorSD: Arquivo txt não encontrado\r\n");
	}
	else
	{
		USB_Print("Arquivo aberto com sucesso\r\n");
	}
	f_lseek(&sdFile, 168);
	result = f_read(&sdFile, (void *)data, data_size, (UINT *)&byteswritten);
	if(result != FR_OK)
	{
		USB_Print("errorSD: erro na leitura\r\n");
		while(1);
	}
	f_close(&sdFile);
}

void CreatNewFile_write(char *arquivo)
{
	result = f_open(&sdFile, arquivo, FA_CREATE_ALWAYS | FA_WRITE);
	if(result != FR_OK)
	{
		USB_Print("errorSD: Arquivo txt não criado\r\n");
	}
	else
	{
		USB_Print("Arquivo criado com sucesso\r\n");

	}
}

void Write_File(uint8_t *data, int data_size)
{
	result = f_write(&sdFile, (void *)data, data_size, (UINT *)&byteswritten);
	if(result != FR_OK)
	{
		USB_Print("errorSD: erro na escrita\r\n");
		while(1);
	}else{
		USB_Print("Data gravado no SD com sucesso!!!\r\n");
	}
	f_close(&sdFile);
}


static uint16_t strlen16(const uint16_t* str)
{
	uint16_t i=0;

	while(*str++) i++;

	return i;
}






