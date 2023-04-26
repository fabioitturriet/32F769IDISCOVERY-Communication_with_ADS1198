#include <gui/configuracoes_ads1198_screen/Configuracoes_ADS1198View.hpp>
#include <gui/configuracoes_ads1198_screen/Configuracoes_ADS1198Presenter.hpp>

Configuracoes_ADS1198Presenter::Configuracoes_ADS1198Presenter(Configuracoes_ADS1198View& v)
    : view(v)
{

}

void Configuracoes_ADS1198Presenter::activate()
{

}

void Configuracoes_ADS1198Presenter::deactivate()
{

}

void Configuracoes_ADS1198Presenter::P_1kSPS_selected()
{
	model->M_1kSPS_selected();
}

void Configuracoes_ADS1198Presenter::P_500SPS_selected()
{
	model->M_500SPS_selected();
}

void Configuracoes_ADS1198Presenter::P_250SPS_selected()
{
	model->M_250SPS_selected();
}

void Configuracoes_ADS1198Presenter::P_125SPS_selected()
{
	model->M_125SPS_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain12_selected()
{
	model->M_Gain12_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain8_selected()
{
	model->M_Gain8_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain6_selected()
{
	model->M_Gain6_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain4_selected()
{
	model->M_Gain4_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain3_selected()
{
	model->M_Gain3_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain2_selected()
{
	model->M_Gain2_selected();
}

void Configuracoes_ADS1198Presenter::P_Gain1_selected()
{
	model->M_Gain1_selected();
}

void Configuracoes_ADS1198Presenter::PresenterStartRDataC()
{
	model->StartRDataC();
}

void Configuracoes_ADS1198Presenter::PresenterSetModoNormal()
{
	model->SetModoNormal();
}

void Configuracoes_ADS1198Presenter::PresenterSetModoTeste()
{
	model->SetModoTeste();
}

void Configuracoes_ADS1198Presenter::PresenterSetModoMensRuido()
{
	model->SetModoMensRuido();
}

void Configuracoes_ADS1198Presenter::P_LOFF_off()
{
	model->Set_LOFF_off();
}

void Configuracoes_ADS1198Presenter::P_LOFF_DCResistor()
{
	model->Set_LOFF_DCResistor();
}

void Configuracoes_ADS1198Presenter::P_LOFF_DCSource()
{
	model->Set_LOFF_DCSource();
}

void Configuracoes_ADS1198Presenter::P_LOFF_AC()
{
	model->Set_LOFF_AC();
}
