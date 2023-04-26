#include <gui/screen2_screen/Screen2View.hpp>

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

void Screen2View::StartRead()
{
	presenter->PresenterStartRead();
}

void Screen2View::Tipo12DevECG()
{
	presenter->PresenterConfig12Dev();
}

void Screen2View::Tipo3DevECG()
{
	presenter->PresenterConfig3Dev();
}
