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
void Screen1View::UpdateGraph(uint16_t value)
{
	dynamicGraph1.addDataPoint(value); //adiciona o valor em um ponto no gráfico

}
