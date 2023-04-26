#include <gui/containers/ContainerPopup.hpp>

#include <string.h>

extern char BufferErrorPopup[120];

ContainerPopup::ContainerPopup()
{

}

void ContainerPopup::initialize()
{
    ContainerPopupBase::initialize();
}

void ContainerPopup::AtualizaMsgErrorPopup()
{
	Unicode::strncpy(textArea1Buffer, BufferErrorPopup, strlen(BufferErrorPopup));
	textArea1Buffer[strlen(BufferErrorPopup)] = 0;
	textArea1.invalidate();
}


