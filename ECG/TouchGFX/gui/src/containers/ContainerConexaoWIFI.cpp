#include <gui/containers/ContainerConexaoWIFI.hpp>

#include <texts/TextKeysAndLanguages.hpp>

#include <string.h>


extern "C" {
struct SSID
{
	char NomeRede[32];
	uint16_t Senha[18];
};
}


extern struct SSID Redes[10];

ContainerConexaoWIFI::ContainerConexaoWIFI()
{

}

void ContainerConexaoWIFI::initialize()
{
    ContainerConexaoWIFIBase::initialize();
}


void ContainerConexaoWIFI::setListElements(int item)
{
    switch (item)
    {
    case 0:
    	Unicode::strncpy(textArea1Buffer, Redes[0].NomeRede, strlen(Redes[0].NomeRede));
    	textArea1Buffer[strlen(Redes[0].NomeRede)] = 0;
    	break;
    case 1:
    	Unicode::strncpy(textArea1Buffer, Redes[1].NomeRede, strlen(Redes[1].NomeRede));
    	textArea1Buffer[strlen(Redes[1].NomeRede)] = 0;
    	break;
    case 2:
    	Unicode::strncpy(textArea1Buffer, Redes[2].NomeRede, strlen(Redes[2].NomeRede));
    	textArea1Buffer[strlen(Redes[2].NomeRede)] = 0;
    	break;
    case 3:
    	Unicode::strncpy(textArea1Buffer, Redes[3].NomeRede, strlen(Redes[3].NomeRede));
    	textArea1Buffer[strlen(Redes[3].NomeRede)] = 0;
    	break;
    case 4:
    	Unicode::strncpy(textArea1Buffer, Redes[4].NomeRede, strlen(Redes[4].NomeRede));
    	textArea1Buffer[strlen(Redes[4].NomeRede)] = 0;
    	break;
    case 5:
    	Unicode::strncpy(textArea1Buffer, Redes[5].NomeRede, strlen(Redes[5].NomeRede));
    	textArea1Buffer[strlen(Redes[5].NomeRede)] = 0;
    	break;
    case 6:
    	Unicode::strncpy(textArea1Buffer, Redes[6].NomeRede, strlen(Redes[6].NomeRede));
    	textArea1Buffer[strlen(Redes[6].NomeRede)] = 0;
    	break;
    case 7:
    	Unicode::strncpy(textArea1Buffer, Redes[7].NomeRede, strlen(Redes[7].NomeRede));
    	textArea1Buffer[strlen(Redes[7].NomeRede)] = 0;
    	break;
    case 8:
    	Unicode::strncpy(textArea1Buffer, Redes[8].NomeRede, strlen(Redes[8].NomeRede));
    	textArea1Buffer[strlen(Redes[8].NomeRede)] = 0;
    	break;
    case 9:
    	Unicode::strncpy(textArea1Buffer, Redes[9].NomeRede, strlen(Redes[9].NomeRede));
    	textArea1Buffer[strlen(Redes[9].NomeRede)] = 0;
    	break;
    default:
    	break;
    	textArea1.invalidate();
    }
}
