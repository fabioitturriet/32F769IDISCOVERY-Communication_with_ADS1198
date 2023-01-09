#include <gui/screen1_screen/Screen1View.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

Screen1Presenter::Screen1Presenter(Screen1View& v)
    : view(v)
{

}

void Screen1Presenter::activate()
{

}

void Screen1Presenter::deactivate()
{

}

void Screen1Presenter::UpdateGraph(float value)
{
	view.UpdateGraph(value); //atualiza o gráfico com o valor
}

void Screen1Presenter::PresenterStopData()
{
	model->StopRDataC();
}

void Screen1Presenter::PresenterAjusteEscala()
{
	model->AjusteEscala();
}

void Screen1Presenter::SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala)
{
	view.SetMinMaxEscalaGraph(SetMinEscala, SetMaxEscala);

}
