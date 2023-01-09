#include <gui/configuracoes_screen/ConfiguracoesView.hpp>
#include <gui/configuracoes_screen/ConfiguracoesPresenter.hpp>

ConfiguracoesPresenter::ConfiguracoesPresenter(ConfiguracoesView& v)
    : view(v)
{

}

void ConfiguracoesPresenter::activate()
{

}

void ConfiguracoesPresenter::deactivate()
{

}

void ConfiguracoesPresenter::P_1kSPS_selected()
{
	model->M_1kSPS_selected();
}

void ConfiguracoesPresenter::P_500SPS_selected()
{
	model->M_500SPS_selected();
}

void ConfiguracoesPresenter::P_250SPS_selected()
{
	model->M_250SPS_selected();
}

void ConfiguracoesPresenter::P_125SPS_selected()
{
	model->M_125SPS_selected();
}

void ConfiguracoesPresenter::P_Gain12_selected()
{
	model->M_Gain12_selected();
}

void ConfiguracoesPresenter::P_Gain8_selected()
{
	model->M_Gain8_selected();
}

void ConfiguracoesPresenter::P_Gain6_selected()
{
	model->M_Gain6_selected();
}

void ConfiguracoesPresenter::P_Gain4_selected()
{
	model->M_Gain4_selected();
}

void ConfiguracoesPresenter::P_Gain3_selected()
{
	model->M_Gain3_selected();
}

void ConfiguracoesPresenter::P_Gain2_selected()
{
	model->M_Gain2_selected();
}

void ConfiguracoesPresenter::P_Gain1_selected()
{
	model->M_Gain1_selected();
}

void ConfiguracoesPresenter::PresenterStartRDataC()
{
	model->StartRDataC();
}
