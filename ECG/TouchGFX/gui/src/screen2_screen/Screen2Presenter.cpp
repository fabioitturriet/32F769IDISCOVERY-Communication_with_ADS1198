#include <gui/screen2_screen/Screen2View.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

Screen2Presenter::Screen2Presenter(Screen2View& v)
    : view(v)
{

}

void Screen2Presenter::activate()
{

}

void Screen2Presenter::deactivate()
{

}

void Screen2Presenter::PresenterStartRead()
{
	model->StartRDataC();
}

void Screen2Presenter::PresenterConfig12Dev()
{
	model->Config12Dev();
}

void Screen2Presenter::PresenterConfig3Dev()
{
	model->Config3Dev();
}
