#include <gui/screenkeyboard_screen/ScreenKeyboardView.hpp>

#include <gui/containers/ContainerLoadingAnimation.hpp>
#include <string.h>

extern "C" {
struct SSID
{
	char NomeRede[32];
	uint16_t Senha[18];
};
}
extern struct SSID Redes[10];

extern uint16_t KeyboardSelection;



ScreenKeyboardView::ScreenKeyboardView()
{
	keyboard.setPosition(240, 188, 320, 240); //add
    add(keyboard); //add

	Unicode::strncpy(textArea1Buffer, Redes[KeyboardSelection].NomeRede, strlen(Redes[KeyboardSelection].NomeRede));
	textArea1Buffer[strlen(Redes[KeyboardSelection].NomeRede)] = 0;

}

void ScreenKeyboardView::setupScreen()
{
    ScreenKeyboardViewBase::setupScreen();
}

void ScreenKeyboardView::tearDownScreen()
{
    ScreenKeyboardViewBase::tearDownScreen();
}

void ScreenKeyboardView::getBuffer()
{
	containerLoadingAnimation1.setVisible(true);
	containerLoadingAnimation1.StartLoadingAnim();
	containerLoadingAnimation1.invalidate();
	buttonSalvarPassword.setTouchable(false);
	voltar.setTouchable(false);
	Unicode::UnicodeChar* buff = keyboard.getBuffer();
	Unicode::strncpy(Redes[KeyboardSelection].Senha, buff, Unicode::strlen(buff)+1);
	presenter->PConnectWiFi();
}



void ScreenKeyboardView::FechaPopup()
{
	containerPopup1.setVisible(false);
	buttonFecharPopup.setVisible(false);
	buttonFecharPopup1.setVisible(false);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
	keyboard.setVisible(true);
	keyboard.invalidate();
	application().gotoCadastroScreenNoTransition();
}

void ScreenKeyboardView::FechaPopup1()
{
	containerPopup1.setVisible(false);
	buttonFecharPopup.setVisible(false);
	buttonFecharPopup1.setVisible(false);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
	keyboard.setVisible(true);
	keyboard.invalidate();
	application().gotoCadastroScreenNoTransition();
}

void ScreenKeyboardView::PopUp()
{
	containerLoadingAnimation1.setVisible(false);
	containerLoadingAnimation1.StopLoadingAnim();
	containerLoadingAnimation1.invalidate();
	buttonSalvarPassword.setTouchable(true);
	voltar.setTouchable(true);
	keyboard.setVisible(false);
	keyboard.invalidate();
	containerPopup1.AtualizaMsgErrorPopup();
	containerPopup1.setVisible(true);
	buttonFecharPopup.setVisible(true);
	buttonFecharPopup1.setVisible(true);
	buttonFecharPopup.invalidate();
	buttonFecharPopup1.invalidate();
	containerPopup1.invalidate();
}
