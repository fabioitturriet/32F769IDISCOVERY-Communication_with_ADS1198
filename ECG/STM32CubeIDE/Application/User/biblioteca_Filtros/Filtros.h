/*
 * Filtros.h
 *
 *  Created on: 8 de ago de 2023
 *      Author: Patrick
 */

#ifndef APPLICATION_USER_BIBLIOTECA_FILTROS_FILTROS_H_
#define APPLICATION_USER_BIBLIOTECA_FILTROS_FILTROS_H_


#include "stdio.h"

float passa_altas_IIR(float signal, uint8_t lead);
float notch_rFixo(float signal, uint8_t lead);

static int16_t calculate_pointer_R(int16_t poiter_w, int16_t distance, int16_t Bufflen);



#endif /* APPLICATION_USER_BIBLIOTECA_FILTROS_FILTROS_H_ */
