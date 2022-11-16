/*------------------------------- DEFINES-----------------------------*/

#include "ADS1298.h"
//

/*------------------------------- VARIABLES -----------------------------*/
//bool intDRDY = false; // Flag to ready data from ADS1298

bool isDaisy = false;		// does this have a daisy chain board?
bool intDRDY = false; // Flag to ready data from ADS1298

int stat_1 = 0, stat_2;    	// used to hold the status register for boards 1 and 2

uint8_t regData[24];	// array with data from all registers

// variável de leitura dos canais para ADS1298
//int32_t channelData [16];	// array used when reading channel data board 1+2

//variável de leitura dos canais para ADS1198
int16_t channelData [16];






uint32_t pila = 0;
/*------------------------------- FUNCTIIONS -----------------------------*/

void ADS_Init(){
	if(verbose){
		HAL_Delay(6000);
		USB_Print("\n\r**************************************\n\r");
		USB_Print("Start ADS1298\n\r");
	}
	
	ADS_RESET();
	HAL_Delay(500);
	
	ADS_SDATAC();
	HAL_Delay(500);
	
	ADS_getDeviceID();
	HAL_Delay(1000);

	//Work settings
	ADS_WREG(CONFIG1,0b00000100); //00000110 fMOD/1024 125SPS  100 500SPS  011 1000SPS
	HAL_Delay(100);
	ADS_WREG(CONFIG2,0x30);   //00110000 fonte de sinal de teste interno
	HAL_Delay(100);
	ADS_WREG(CONFIG3,0b11101100);  //testar E8 caso de erro // RLD conectado, VREFP = 2.4V, sinal RLD_IN (sem conexao no nosso projeto deixei esse bit desativado inicialmente) é roteado para o canal que possui o MUX_Setting 010 (VREF)
	HAL_Delay(100);
	
	//ADS_WREG(LOFF,0x03);  //00000011
	HAL_Delay(10);
	
	ADS_WREG(CH1SET,0b01100000);  //00010000
	HAL_Delay(10);
	ADS_WREG(CH2SET,0b01100000);  //00010100 medir DC
	HAL_Delay(10);
	ADS_WREG(CH3SET,0b01100000);  //10010000 canal desligado, entrada normal
	HAL_Delay(10);
	ADS_WREG(CH4SET,0b01100000);
	HAL_Delay(10);
	ADS_WREG(CH5SET,0b01100000);  //01010101
	HAL_Delay(10);
	ADS_WREG(CH6SET,0b01100000);
	HAL_Delay(10);
	ADS_WREG(CH7SET,0b01100000);  //01100101
	HAL_Delay(10);
	ADS_WREG(CH8SET,0b01100000);  //00010000
	HAL_Delay(10);

	ADS_WREG(BIAS_SENSP,0b00000110);
	HAL_Delay(10);
	ADS_WREG(BIAS_SENSN,0b00000010);
	HAL_Delay(10);
	/*
	ADS_WREG(LOFF_SENSP,0xFF);
	HAL_Delay(10);
	ADS_WREG(LOFF_SENSN,0xF2);
	HAL_Delay(10);
	
	ADS_WREG(LOFF_FLIP,0x00);  
	HAL_Delay(10);
	
	//Registradores de somente leitura, nao sei pq ele usa esse comando
//	ADS_WREG(LOFF_STATP,0xF1);
//	HAL_Delay(10);
	//ADS_WREG(LOFF_STATN,0x00);
//	HAL_Delay(10);
	
	ADS_WREG(GPIO,0x00);  
	HAL_Delay(10);
	
	ADS_WREG(MISC1,0x00);  //pace detection off
	HAL_Delay(10);
	ADS_WREG(MISC2,0x00); //reservado
	HAL_Delay(10);
	
	ADS_WREG(CONFIG4,0b00000010);  //00000010 modo continuo, WCT para RLD off, Lead-off comparador on
	//ADS_WREG(CONFIG4,0b00001010);//Single-shot mode
	HAL_Delay(10);
	
	ADS_WREG(0x18,0x0A);  //WTC1, 00001010 Channel 2 positive input connected to WCTA amplifier
	HAL_Delay(10);
	
	ADS_WREG(0x19,0xE3); //WCT2, 11100011 100 = Channel 3 positive input connected to WCTB amplifier e
	//011 = Channel 2 negative input connected to WCTC amplifier
	HAL_Delay(10);
	*/

	ADS_RREGS(0,17);
	HAL_Delay(1000);
	
//	ADS_START();
	HAL_Delay(100);
/*
	ADS_RDATAC();            // enter Read Data Continuous mode
	HAL_Delay(100);
*/
	
	
	if(verbose){
		USB_Print("\n\rADS1298 configure ..... DONE!\n\r");
		USB_Print("\n\r**************************************\n\r");
		HAL_Delay(3000);
	}
	
}

void ADS_SDATAC(){
	
	HAL_GPIO_WritePin( ADS_CS_GPIO_Port  , ADS_CS_Pin, GPIO_PIN_RESET); //cs
	transferSPI(_SDATAC);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET); //cs
}

//start data conversion 
void ADS_START() {			
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_RESET);
	transferSPI(_START);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET);
}

//stop data conversion 
void ADS_STOP() {			
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET);
	transferSPI(_STOP);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET);
}

void ADS_RESET(){
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_RESET);
	transferSPI(_RESET);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET);
}

void ADS_RDATAC() {
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET);
	transferSPI(_RDATAC);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET);
}

void ADS_WAKEUP() {
  HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET);
	transferSPI( _WAKEUP);
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET);
}

// Register Read/Write Commands
uint8_t ADS_getDeviceID() {			// simple hello world com check
	
	uint8_t data = ADS_RREG(0);

	if(verbose){						// verbose otuput
		USB_Print("\n\rDevice ID: ");
		USB_SendBits(data);
	}
	
	return data;
}

uint8_t ADS_RREG(uint8_t _address){		//  reads ONE register at _address
	uint8_t opcode1 = _address + 0x20; 		//  RREG expects 001rrrrr where rrrrr = _address
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_RESET);			//  open SPI
	
	transferSPI( opcode1); 								//  opcode1
  transferSPI( 0); 											//  opcode2
  
	regData[_address] = transferSPI( 0);		//  update mirror location with returned byte
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET);				//  close SPI
	
	//HAL_UART_Transmit(&huart1, &regData[0], sizeof(uint8_t),0x1000);
	
	if (verbose){													//  verbose output
		USB_Print("\r\nRegister: ");
		ADS_printRegisterName(_address);
		USB_Print(" has value: ");
		USB_SendBits(regData[_address]);
		USB_Print("\n\r");
	}
	
	return regData[_address];			// return requested register value
}

// Read more than one register starting at _address
void ADS_RREGS(uint8_t _address, uint8_t _numRegistersMinusOne) {
//	for(byte i = 0; i < 0x17; i++){
//		regData[i] = 0;									//  reset the regData array
//	}
	int i;
	
	uint8_t opcode1 = _address + 0x20; 				//  RREG expects 001rrrrr where rrrrr = _address
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET); 					//  open SPI
	
	transferSPI( opcode1); 										//  opcode1
	transferSPI( _numRegistersMinusOne);				//  opcode2

	for(i = 0; i <= _numRegistersMinusOne; i++){
		regData[_address + i] = transferSPI( 0x00); 	//  add register byte to mirror array
	}
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET); 			//  close SPI
	
	if(verbose){												//  verbose output
		USB_Print("\n\r======================\n\rRegister map\n\r");
		for(i = 0; i<= _numRegistersMinusOne; i++){
			USB_Print("\n\rRegister: ");
			ADS_printRegisterName(_address + i);
			USB_Print(" has value: ");
			USB_SendBits(regData[_address + i]);		
		}	
		USB_Print("\n\r======================\n\r");
	}   
}


void ADS_WREG(uint8_t _address, uint8_t _value) {	//  Write ONE register at _address
	uint8_t opcode1 = _address + 0x40; 				//  WREG expects 010rrrrr where rrrrr = _address
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET); 					//  open SPI
	
	transferSPI( opcode1);											//  Send WREG command & address
	transferSPI( 0x00);												//	Send number of registers to read -1
	transferSPI( _value);											//  Write the value to the register
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET); 						//  close SPI
	
	regData[_address] = _value;			//  update the mirror array
	
	if(verbose){						//  verbose output
		USB_Print("\n\rRegister: ");
		ADS_printRegisterName(_address);
		USB_Print(" - modified\n\r");
	}
}

void ADS_WREGS(uint8_t _address, uint8_t _numRegistersMinusOne){
	int i;
	
	uint8_t opcode1 = _address + 0x40;				//  WREG expects 010rrrrr where rrrrr = _address
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_RESET); 					//  open SPI
	
	transferSPI( opcode1);											//  Send WREG command & address
	transferSPI( _numRegistersMinusOne);				//	Send number of registers to read -1	
	
	for (i=_address; i <=(_address + _numRegistersMinusOne); i++){
		transferSPI( regData[i]);								//  Write to the registers
	}	
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin, GPIO_PIN_SET); 						//  close SPI
	
	if(verbose){
		USB_Print("\n\rRegisters: ");
		ADS_printRegisterName(_address); USB_Print("  TO: ");
		USB_SendBits((_address + _numRegistersMinusOne));
		USB_Print(" - modified!\n\r");
	}
}

void ADS_updateChannelData(){
	uint8_t inByte, inByte1, inByte2;
	int i,j;				// iterator in loop
	int nchan=8;  //assume 8 channel.  If needed, it automatically changes to 16 automatically in a later block.
	
	
	for(i = 0; i < nchan; i++){
		channelData[i] = 0;
	}
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_RESET); 					//  open SPI
	
	// READ CHANNEL DATA FROM FIRST ADS IN DAISY LINE
	for(i = 0; i < 3; i++){										//  read 3 byte status register from ADS 1 (1100+LOFF_STATP+LOFF_STATN+GPIO[7:4])
		inByte = transferSPI( 0x00);
		stat_1 = (stat_1<<8) | inByte;				
	}
	
	/*for(i = 0; i < 8; i++){
		for( j=0; j<3; j++){		//  read 24 bits of channel data from 1st ADS in 8 3 byte chunks
			inByte = transferSPI( 0x00);
			channelData[i] = (channelData[i]<<8) | inByte;
		}
	}*/
	for(i = 0; i < 8; i++){
					inByte1 = transferSPI( 0x00);
					inByte2 = transferSPI( 0x00);

				channelData[i] = (inByte1 << 8) | inByte2;

			}
	
	if (isDaisy) {
		nchan = 16;
		// READ CHANNEL DATA FROM SECOND ADS IN DAISY LINE
		for(i = 0; i < 3; i++){			//  read 3 byte status register from ADS 2 (1100+LOFF_STATP+LOFF_STATN+GPIO[7:4])
			inByte = transferSPI( 0x00);
			stat_2 = (stat_1<<8) | inByte;				
		}
		
		for( i = 8; i < 16; i++){
			for( j = 0; j < 3; j++){		//  read 24 bits of channel data from 2nd ADS in 8 3 byte chunks
				inByte = transferSPI( 0x00);
				channelData[i] = (channelData[i]<<8) | inByte;
			}
		}
	}
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port , ADS_CS_Pin, GPIO_PIN_SET); 			// CLOSE SPI
	
	//reformat the numbers
	/*
	for( i=0; i<nchan; i++){			// convert 3 byte 2's compliment to 4 byte 2's compliment		
		if( (channelData[i] & 0x00800000) == 0x00800000 )	{
			channelData[i] = ~channelData[i];
			channelData[i] += 0x00000001;
		}else{
			//channelData[i] &= 0x00FFFFFF;
		}
		channelData[i] = channelData[i]  << 8;
	}
	*/
}

//read data
void ADS_RDATA() {				//  use in Stop Read Continuous mode when DRDY goes low
	uint8_t inByte,inByte1,inByte2,inByte3;
	int i,j;
	int nchan = 8;	//assume 8 channel.  If needed, it automatically changes to 16 automatically in a later block.
	
	stat_1 = 0;							//  clear the status registers
	stat_2 = 0;	
	
	for(i = 0; i < nchan; i++){
		channelData[i] = 0;
	}
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin,GPIO_PIN_RESET); 					//  open SPI
	transferSPI( _RDATA);
	
	// READ CHANNEL DATA FROM FIRST ADS IN DAISY LINE
	for(i = 0; i < 3; i++){			//  read 3 byte status register (1100+LOFF_STATP+LOFF_STATN+GPIO[7:4])
		inByte = transferSPI( 0x00);
		stat_1 = (stat_1<<8) | inByte;				
	}
	
	/* Leitura 24bits de resolução por canal ADS1198
	for(i = 0; i < 8; i++){
			inByte1 = transferSPI( 0x00);
			inByte2 = transferSPI( 0x00);
			inByte3 = transferSPI( 0x00);
		
		channelData[i] = (inByte1 << 16) | (inByte2 << 8) | inByte3;
		
	}
	*/
	for(i = 0; i < 8; i++){
				inByte1 = transferSPI( 0x00);
				inByte2 = transferSPI( 0x00);

			channelData[i] = (inByte1 << 8) | inByte2;

		}
	
	if (isDaisy) {
		nchan = 16;
		
		// READ CHANNEL DATA FROM SECOND ADS IN DAISY LINE
		for( i=0; i<3; i++){			//  read 3 byte status register (1100+LOFF_STATP+LOFF_STATN+GPIO[7:4])
			inByte = transferSPI( 0x00);
			stat_2 = (stat_1<<8) | inByte;				
		}
		
		for( i = 8; i<16; i++){
			for( j=0; j<3; j++){		//  read 24 bits of channel data from 2nd ADS in 8 3 byte chunks
				inByte = transferSPI( 0x00);
				channelData[i] = (channelData[i]<<8) | inByte;
			}
		}
	}
	
	HAL_GPIO_WritePin(ADS_CS_GPIO_Port ,ADS_CS_Pin,GPIO_PIN_SET); 					//  open SPI
	
	

/*	for( i = 0; i<nchan; i++){			// convert 3 byte 2's compliment to 4 byte 2's compliment
		//if(bitRead(channelData[i],15) == 1){
		if( (channelData[i] & 0x8000) == 0x8000  ){
			channelData[i] = ~channelData[i] + 1;
			
		}else{
			channelData[i] &= 0xFFFF;
		}
		//channelData[i] = channelData[i] << 8;
	} */

}

// String-Byte converters for RREG and WREG
void ADS_printRegisterName(uint8_t _address) {
    if(_address == ID){
        USB_Print("ID "); //the "F" macro loads the string directly from Flash memory, thereby saving RAM
    }
    else if(_address == CONFIG1){
        USB_Print("CONFIG1 ");
    }
    else if(_address == CONFIG2){
        USB_Print("CONFIG2 ");
    }
    else if(_address == CONFIG3){
        USB_Print("CONFIG3 ");
    }
    else if(_address == LOFF){
        USB_Print("LOFF ");
    }
    else if(_address == CH1SET){
        USB_Print("CH1SET ");
    }
    else if(_address == CH2SET){
        USB_Print("CH2SET ");
    }
    else if(_address == CH3SET){
        USB_Print("CH3SET ");
    }
    else if(_address == CH4SET){
       USB_Print("CH4SET ");
    }
    else if(_address == CH5SET){
        USB_Print("CH5SET ");
    }
    else if(_address == CH6SET){
        USB_Print("CH6SET ");
    }
    else if(_address == CH7SET){
        USB_Print("CH7SET ");
    }
    else if(_address == CH8SET){
       USB_Print("CH8SET ");
    }
    else if(_address == BIAS_SENSP){
        USB_Print("BIAS_SENSP ");
    }
    else if(_address == BIAS_SENSN){
        USB_Print("BIAS_SENSN ");
    }
    else if(_address == LOFF_SENSP){
        USB_Print("LOFF_SENSP ");
    }
    else if(_address == LOFF_SENSN){
        USB_Print("LOFF_SENSN ");
    }
    else if(_address == LOFF_FLIP){
        USB_Print("LOFF_FLIP ");
    }
    else if(_address == LOFF_STATP){
        USB_Print("LOFF_STATP ");
    }
    else if(_address == LOFF_STATN){
        USB_Print("LOFF_STATN ");
    }
    else if(_address == GPIO){
        USB_Print("GPIO ");
    }
    else if(_address == MISC1){
        USB_Print("MISC1 ");
    }
    else if(_address == MISC2){
        USB_Print("MISC2 ");
    }
    else if(_address == CONFIG4){
        USB_Print("CONFIG4 ");
    }
		//USB_Print("\t");
}

void ADS_sendUSBData(void){




	//temp = (((channelData[1]*73.24442274)-145300)/490) +25;
	//USB_Send16bit(channel2);
	//USB_Sendfloat(temp);



	//envia 2 byte ADS1198
			/*
		USB_Print("DT0");
		USB_Send2Byte((uint8_t*)&channelData[0]);
		USB_Print("DT1");
		USB_Send2Byte((uint8_t*)&channelData[1]);
		USB_Print("DT2");
		USB_Send2Byte((uint8_t*)&channelData[2]);
		USB_Print("DT3");
		USB_Send2Byte((uint8_t*)&channelData[3]);
		USB_Print("DT4");
		USB_Send2Byte((uint8_t*)&channelData[4]);
		USB_Print("DT5");
		USB_Send2Byte((uint8_t*)&channelData[5]);
		USB_Print("DT6");
		USB_Send2Byte((uint8_t*)&channelData[6]);
		USB_Print("DT7");
		USB_Send2Byte((uint8_t*)&channelData[7]);
		*/
	/*envia 4 byte ADS1298
	//if(!verbose){
		USB_Print("DT0");
		USB_Send4Byte((uint8_t*)&channelData[0]);
		USB_Print("DT1");
		USB_Send4Byte((uint8_t*)&channelData[1]);
		USB_Print("DT2");
		USB_Send4Byte((uint8_t*)&channelData[2]);
		USB_Print("DT3");
		USB_Send4Byte((uint8_t*)&channelData[3]);
		USB_Print("DT4");
		USB_Send4Byte((uint8_t*)&channelData[4]);
		USB_Print("DT5");
		USB_Send4Byte((uint8_t*)&channelData[5]);
		USB_Print("DT6");
		USB_Send4Byte((uint8_t*)&channelData[6]);
		USB_Print("DT7");
		USB_Send4Byte((uint8_t*)&channelData[7]);
		*/
	//}
	//else
	//	HAL_Delay(500);
	//USB_SendNumber(channelData[0]);
	//USB_Print("DT1");
	//USB_SendNumber(0x66223344);
}
	
void ADS_sendUARTData(void){
	

	//envia 2 byte ADS1198

	/*envia 4 byte ADS1298
	USART_Send("DT0");
	USART_Send4Byte((uint8_t*)&channelData[0]);
	USART_Send("DT1");
	USART_Send4Byte((uint8_t*)&channelData[1]);
	USART_Send("DT2");
	USART_Send4Byte((uint8_t*)&channelData[2]);
	USART_Send("DT3");
	USART_Send4Byte((uint8_t*)&channelData[3]);
	USART_Send("DT4");
	USART_Send4Byte((uint8_t*)&channelData[4]);
	USART_Send("DT5");
	USART_Send4Byte((uint8_t*)&channelData[5]);
	USART_Send("DT6");
	USART_Send4Byte((uint8_t*)&channelData[6]);
	USART_Send("DT7");
	USART_Send4Byte((uint8_t*)&channelData[7]);
	*/
}
	
void ADS_SendData(){
	//if(HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_9) == GPIO_PIN_SET)
		ADS_sendUSBData();
	//else
		//ADS_sendUARTData();
}

int16_t* getChannelData(){
	return channelData;
}



