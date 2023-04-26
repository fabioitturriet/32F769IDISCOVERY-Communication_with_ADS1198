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

void Screen1Presenter::PNextDeriv()
{
	model->NextDerivSelect();
}

void Screen1Presenter::PBackDeriv()
{
	model->BackDerivSelect();
}

void Screen1Presenter::PWifiECG()
{
	model->TransmiteWifiECG();
}

void Screen1Presenter::PresenterAtualizarBPM()
{
	model->CalcularBPM();
}

void Screen1Presenter::AtualizarBPM(){
	view.AtualizaBPM();
}

void Screen1Presenter::AtualizarIconHeart(){
	view.AtualizaIconHeart();
}

void Screen1Presenter::GeraPopup()
{
	view.PopUp();
}

void Screen1Presenter::PreStopPanTompkins()
{
	model->StopPanTompkins();
}

void Screen1Presenter::PreStopLoadingAnimation()
{
	view.StopLoadingAnimation();
}

void Screen1Presenter::UpdateMatrixLOFF()
{
	view.MUpdateMatrixLOFF();
}
