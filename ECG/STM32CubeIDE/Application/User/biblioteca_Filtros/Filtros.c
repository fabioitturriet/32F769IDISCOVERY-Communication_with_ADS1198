/*
 * Filtros.c
 *
 *  Created on: 8 de ago de 2023
 *      Author: Patrick
 */
/*------------------------------- INCLUDES -----------------------------*/
#include "Filtros.h"
#include "math.h"

#define BUFFER_SIZE_HP 170
#define BUFFER_SIZE_NOTCH 6
#define BUFFER_SIZE_HP_TWO_PASS 150
#define HP_NSEC 2

const static float NUM[HP_NSEC][3] = {
		{
				0.996858417987824,	1.99371683597565,	0.996858417987824
		},
		{
				0.996868252754211,	0.996868252754211, 0
		}
};

const static float DEN[HP_NSEC][3] = {
		{
				1,	1.99369716644287,	0.993736505508423
		},
		{
				1,	0.993736445903778, 0
		}

};

//const static float NUM[HP_NSEC][3] = {
//		{
//				0.325368076562881,	0.650731925353963,	0.325368076562881
//		},
//		{
//				0.997062444686890,	1.99411930299581,	0.997062444686890
//		},
//		{
//				3.04266309738159,	3.04266309738159, 0
//		}
//};
//
//const static float DEN[HP_NSEC][3] = {
//		{
//				1,	1.99905347824097,	0.999093174934387
//		},
//		{
//				1,	1.99408233165741,	0.994161784648895
//		},
//		{
//				1,	0.980882346630096, 0
//		}
//};


// Variáveis Filtros HighPass
static float signal_in_HP [12][BUFFER_SIZE_HP];
static float signal_out_HP[HP_NSEC][12][BUFFER_SIZE_HP];
//static float signal_out_HP_TwoPass [3][12][BUFFER_SIZE_HP];
static volatile uint16_t poiter_HP_w[12], pointer_HP_r[12][BUFFER_SIZE_HP];
//const static float num_HP[6] = {0.987077302635839, 4.935368156848893, 9.870717957439346, 9.870717957439346, 4.935368156848893, 0.987077302635839};
//const static float den_HP[6] = {1, 4.97401815652847, 9.89632860420768, 9.84487417839213, 4.89683517935783, 0.974271448584605};
static float filtro_two_pass_in[BUFFER_SIZE_HP_TWO_PASS];
static float filtro_two_pass1[BUFFER_SIZE_HP_TWO_PASS];
static float filtro_two_pass2[BUFFER_SIZE_HP_TWO_PASS];
static float filtro_two_pass3[BUFFER_SIZE_HP_TWO_PASS];

//const static float num_HP[6] = {0.98707, 4.93536, 9.87071, 9.87071, 4.93536, 0.98707};
//const static float den_HP[6] = {1, 4.97401, 9.89632, 9.84487, 4.89683, 0.97427};

// Variáveis Filtros Notch
static float signal_in_Notch [12][BUFFER_SIZE_NOTCH];
static float signal_out_Notch [12][BUFFER_SIZE_NOTCH];
static volatile uint16_t poiter_Notch_w[12], pointer_Notch_r[12];
uint8_t first_time_filter_HP = 0;
uint8_t first_time_filter_notch = 0;


float notch_rFixo(float signal, uint8_t lead)
{
	poiter_Notch_w[lead]++;

	if(poiter_Notch_w[lead]>=BUFFER_SIZE_NOTCH)
	{
		poiter_Notch_w[lead] = 0;
	}

	if(first_time_filter_notch==0){
		first_time_filter_notch=1;
		for(uint8_t i=0; i<BUFFER_SIZE_NOTCH; i++){
			signal_out_Notch[lead][i]=signal;
		}

	}
	//60Hz + 120hz
	signal_in_Notch[lead][poiter_Notch_w[lead]] = signal;

	switch (first_time_filter_notch)
	{
	case 0:
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_in_Notch[lead][poiter_Notch_w[lead]];
		first_time_filter_notch++;
		break;
	case 1:
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_in_Notch[lead][poiter_Notch_w[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 1, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.575600702431943*signal_out_Notch[lead][pointer_Notch_r[lead]];
		first_time_filter_notch++;
		break;
	case 2:
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_in_Notch[lead][poiter_Notch_w[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 1, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.575600702431943*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 2, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 2.183089275345444*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 2.161312959823873*signal_out_Notch[lead][pointer_Notch_r[lead]];
		first_time_filter_notch++;
		break;
	case 3:
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_in_Notch[lead][poiter_Notch_w[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 1, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.575600702431943*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 2, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 2.183089275345444*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 2.161312959823873*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 3, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.559884085425184*signal_out_Notch[lead][pointer_Notch_r[lead]];
		first_time_filter_notch++;
		break;
	default:
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_in_Notch[lead][poiter_Notch_w[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 1, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.575600702431943*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 2, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 2.183089275345444*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 2.161312959823873*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 3, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 1.583518293901450*signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + 1.559884085425184*signal_out_Notch[lead][pointer_Notch_r[lead]];
		pointer_Notch_r[lead] = calculate_pointer_R(poiter_Notch_w[lead], 4, BUFFER_SIZE_NOTCH);
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] + signal_in_Notch[lead][pointer_Notch_r[lead]];
		signal_out_Notch[lead][poiter_Notch_w[lead]] = signal_out_Notch[lead][poiter_Notch_w[lead]] - 0.980149500625000*signal_out_Notch[lead][pointer_Notch_r[lead]];
		break;
	}


	return signal_out_Notch[lead][poiter_Notch_w[lead]];
}

float passa_altas_IIR(float signal, uint8_t lead)
{

	poiter_HP_w[lead]++;

	if(poiter_HP_w[lead]>=BUFFER_SIZE_HP)
	{
		poiter_HP_w[lead] = 0;
	}

	signal_in_HP[lead][poiter_HP_w[lead]] = signal;

	//Tempo Direto
	switch(first_time_filter_HP)
	{
	case 0:
		for(uint8_t i = 0; i < HP_NSEC; i++)
		{
			if(i==0){
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_in_HP[lead][poiter_HP_w[lead]];
			}else{
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_out_HP[i-1][lead][poiter_HP_w[lead]];
			}
		}

		break;

	case 1:
		for(uint8_t i = 0; i < HP_NSEC; i++)
		{
			switch (i)
			{
			case 0:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_in_HP[lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_in_HP[lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			case 1:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_out_HP[i-1][lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_out_HP[i-1][lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			case 2:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_out_HP[i-1][lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_out_HP[i-1][lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			}
		}

		break;

	default:
		for(uint8_t i = 0; i < HP_NSEC; i++)
		{
			switch (i)
			{
			case 0:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_in_HP[lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_in_HP[lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 2, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + NUM[i][2]*signal_in_HP[lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - DEN[i][2]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			case 1:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_out_HP[i-1][lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_out_HP[i-1][lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 2, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + NUM[i][2]*signal_out_HP[i-1][lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - DEN[i][2]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			case 2:
				signal_out_HP[i][lead][poiter_HP_w[lead]] = NUM[i][0]*signal_out_HP[i-1][lead][poiter_HP_w[lead]];
				pointer_HP_r[lead][0] = calculate_pointer_R(poiter_HP_w[lead], 1, BUFFER_SIZE_HP);
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] - NUM[i][1]*signal_out_HP[i-1][lead][pointer_HP_r[lead][0]];
				signal_out_HP[i][lead][poiter_HP_w[lead]] = signal_out_HP[i][lead][poiter_HP_w[lead]] + DEN[i][1]*signal_out_HP[i][lead][pointer_HP_r[lead][0]];
				break;
			}
		}


			break;

	}

	//tempo reverso
	if(first_time_filter_HP == BUFFER_SIZE_HP_TWO_PASS)
	{
	for(uint8_t i = 0; i < BUFFER_SIZE_HP_TWO_PASS; i++)
	{
		filtro_two_pass_in[i] = signal_out_HP[HP_NSEC-1][lead][calculate_pointer_R(poiter_HP_w[lead], BUFFER_SIZE_HP_TWO_PASS-1-i, BUFFER_SIZE_HP)];
	}

	//primeiro seção do filtro
	for(int16_t z = BUFFER_SIZE_HP_TWO_PASS-1; z >=0; z--)
	{
		if(z==BUFFER_SIZE_HP_TWO_PASS-1)
		{
			filtro_two_pass1[z] = NUM[0][0]*filtro_two_pass_in[z];
		}else if(z==BUFFER_SIZE_HP_TWO_PASS-2){
			filtro_two_pass1[z] = NUM[0][0]*filtro_two_pass_in[z]- NUM[0][1]*filtro_two_pass_in[z+1]+DEN[0][1]*filtro_two_pass1[z+1];
		}else{
			filtro_two_pass1[z] = NUM[0][0]*filtro_two_pass_in[z] - NUM[0][1]*filtro_two_pass_in[z+1] +  NUM[0][2]*filtro_two_pass_in[z+2] + DEN[0][1]*filtro_two_pass1[z+1] - DEN[0][2]*filtro_two_pass1[z+2];
		}
	}

    if(HP_NSEC==1){
    	return filtro_two_pass1[0];
    }

	//segunda seção do filtro
	for(int16_t z = BUFFER_SIZE_HP_TWO_PASS-1; z >=0; z--)
	{
		if(z==BUFFER_SIZE_HP_TWO_PASS-1)
		{
			filtro_two_pass2[z] = NUM[1][0]*filtro_two_pass1[z];
		}else if(z==BUFFER_SIZE_HP_TWO_PASS-2){
			filtro_two_pass2[z] = NUM[1][0]*filtro_two_pass1[z] - NUM[1][1]*filtro_two_pass1[z+1]+DEN[1][1]*filtro_two_pass2[z+1];
		}else{
			filtro_two_pass2[z] = NUM[1][0]*filtro_two_pass1[z] - NUM[1][1]*filtro_two_pass1[z+1] +  NUM[1][2]*filtro_two_pass1[z+2] + DEN[1][1]*filtro_two_pass2[z+1] - DEN[1][2]*filtro_two_pass2[z+2];
		}
	}

	if(HP_NSEC==2){
	    	return filtro_two_pass2[0];
	    }

	//terceira seção do filtro
	for(int16_t z = BUFFER_SIZE_HP_TWO_PASS-1; z >=0; z--)
	{
		if(z==BUFFER_SIZE_HP_TWO_PASS-1)
		{
			filtro_two_pass3[z] = NUM[2][0]*filtro_two_pass2[z];
		}else{
			filtro_two_pass3[z] = NUM[2][0]*filtro_two_pass2[z]-NUM[2][1]*filtro_two_pass2[z+1]+DEN[2][1]*filtro_two_pass3[z+1];
		}
	}

	return filtro_two_pass3[0];

	}



	if(first_time_filter_HP != BUFFER_SIZE_HP_TWO_PASS){
				first_time_filter_HP++;
	}

	return signal_out_HP[1][lead][poiter_HP_w[lead]];
}


static int16_t calculate_pointer_R(int16_t poiter_w, int16_t distance, int16_t Bufflen)
{
	if(poiter_w - distance >=0)
	{
		return poiter_w-distance;
	}
	else
	{
		return Bufflen + (poiter_w-distance);
	}

}
