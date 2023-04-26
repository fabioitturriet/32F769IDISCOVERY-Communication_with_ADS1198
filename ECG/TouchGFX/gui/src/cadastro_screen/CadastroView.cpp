#include <gui/cadastro_screen/CadastroView.hpp>
#include <gui/containers/ContainerConexaoWIFI.hpp>

#include <gui/containers/ContainerLoadingAnimation.hpp>
#include <gui/containers/ContainerPopup.hpp>
#include "stm32f7xx_hal.h"

uint16_t KeyboardSelection;

CadastroView::CadastroView() :
scrollList1_ItemSelectedCallback(this, &CadastroView::scrollList1_ItemSelectedHandler)
{

}

void CadastroView::setupScreen()
{
	scrollList1.setItemSelectedCallback(scrollList1_ItemSelectedCallback);
    CadastroViewBase::setupScreen();
}

void CadastroView::tearDownScreen()
{
    CadastroViewBase::tearDownScreen();
}

void CadastroView::initCadastro()
{
	if(HAL_GPIO_ReadPin(GPIOH, GPIO_PIN_7)){
		 voltar.setTouchable(false);
		 toggleInitWIFI.forceState(true);
		 presenter->PAtualizarRedes();
		 containerLoadingAnimation1.setVisible(true);
		 containerLoadingAnimation1.StartLoadingAnim();
		 containerLoadingAnimation1.invalidate();
		 buttonAtualizarRedes.setTouchable(false);
		 buttonAtualizarRedes.setAlpha(100);
		 buttonAtualizarRedes.invalidate();
		 toggleInitWIFI.invalidate();
	}else{
		 toggleInitWIFI.forceState(false);
		 buttonAtualizarRedes.setTouchable(false);
		 buttonAtualizarRedes.setAlpha(100);
		 buttonAtualizarRedes.invalidate();
		 toggleInitWIFI.invalidate();
	}
}

void CadastroView::ToggleWiFi()
{
	if (toggleInitWIFI.getState()){
		containerLoadingAnimation1.setVisible(true);
		containerLoadingAnimation1.StartLoadingAnim();
		containerLoadingAnimation1.invalidate();
		toggleInitWIFI.setTouchable(false);
		voltar.setTouchable(false);
		presenter->P_InitESP();
	}else{
		presenter->PWiFi_OFF();
		buttonAtualizarRedes.setTouchable(false);
		buttonAtualizarRedes.setAlpha(100);
		buttonAtualizarRedes.invalidate();
	}

}

void CadastroView::scrollList1UpdateItem(ContainerConexaoWIFI& item, int16_t itemIndex)
{
	item.setListElements(itemIndex);
}


void CadastroView::itensAlterados()
{
	buttonAtualizarRedes.setTouchable(true);
	toggleInitWIFI.setTouchable(true);
	voltar.setTouchable(true);
	buttonAtualizarRedes.setAlpha(255);
	buttonAtualizarRedes.invalidate();
	scrollList1.invalidate();
	containerLoadingAnimation1.StopLoadingAnim();
	containerLoadingAnimation1.setVisible(false);
	containerLoadingAnimation1.invalidate();
}

void CadastroView::AtualizarRedes()
{
	buttonAtualizarRedes.setTouchable(false);
	voltar.setTouchable(false);
	toggleInitWIFI.setTouchable(false);
	containerLoadingAnimation1.setVisible(true);
	containerLoadingAnimation1.StartLoadingAnim();
	containerLoadingAnimation1.invalidate();
	presenter->PAtualizarRedes();
}

void CadastroView::defnumredes(uint8_t NumeroRedes)
{
	scrollList1.setNumberOfItems(NumeroRedes);
	for(uint8_t i=0;i<NumeroRedes;i++)
	scrollList1.itemChanged(i);
}

void CadastroView::scrollList1_ItemSelectedHandler(int16_t itemSelected)
{
	KeyboardSelection = itemSelected;
	application().gotoScreenKeyboardScreenNoTransition();
}

void CadastroView::FechaPopup()
{
	containerPopup1.setVisible(false);
	buttonFecharPopup.setVisible(false);
	buttonFecharPopup1.setVisible(false);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
}

void CadastroView::FechaPopup1()
{
	containerPopup1.setVisible(false);
	buttonFecharPopup.setVisible(false);
	buttonFecharPopup1.setVisible(false);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
}

void CadastroView::PopUp()
{
	containerPopup1.AtualizaMsgErrorPopup();
	containerPopup1.setVisible(true);
	buttonFecharPopup.setVisible(true);
	buttonFecharPopup1.setVisible(true);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
}
