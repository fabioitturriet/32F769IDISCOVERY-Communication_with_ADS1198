#include <gui/screen1_screen/Screen1View.hpp>

Screen1View::Screen1View()
{

}

void Screen1View::setupScreen()
{
    Screen1ViewBase::setupScreen();
}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::UpdateGraph(float value)
{
	dynamicGraph1.addDataPoint(value); //adiciona o valor em um ponto no gráfico

}

void Screen1View::StopData()
{
	presenter->PresenterStopData();
}

void Screen1View::AjusteEscala()
{
	presenter->PresenterAjusteEscala();
}

void Screen1View::SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala)
{
	dynamicGraph1.setGraphRangeY(SetMinEscala, SetMaxEscala);
}
