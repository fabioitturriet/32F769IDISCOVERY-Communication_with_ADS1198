#include <gui/screenkeyboard_screen/ScreenKeyboardView.hpp>
#include <gui/screenkeyboard_screen/ScreenKeyboardPresenter.hpp>

ScreenKeyboardPresenter::ScreenKeyboardPresenter(ScreenKeyboardView& v)
    : view(v)
{

}

void ScreenKeyboardPresenter::activate()
{

}

void ScreenKeyboardPresenter::deactivate()
{

}

void ScreenKeyboardPresenter::PConnectWiFi()
{
	model->MConnectWifi();
}

void ScreenKeyboardPresenter::GeraPopup()
{
	view.PopUp();
}
