/*
 * ecg_sd.h
 *
 *  Created on: 13 de jun de 2023
 *      Author: Patrick
 */

#ifndef APPLICATION_USER_BIBLIOTECA_SDCARD_ECG_SD_H_
#define APPLICATION_USER_BIBLIOTECA_SDCARD_ECG_SD_H_

#include "stdio.h"

#define ECG_WRITE_DATA_SIZE 6144


void Init_MicroSD(void);
void start_newPacient_recording();
void write2ecg_file(uint8_t *data, uint16_t data_size);
void Read_SD_Data(float *data, char* arquivo, int data_size);
void stop_recording();
void CreatNewFile_write(char *arquivo);
void Write_File(uint8_t *data, int data_size);
static uint16_t strlen16(const uint16_t* str);

#endif /* APPLICATION_USER_BIBLIOTECA_SDCARD_ECG_SD_H_ */
