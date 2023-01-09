#include <gui/configuracoes_screen/ConfiguracoesView.hpp>

ConfiguracoesView::ConfiguracoesView()
{

}

void ConfiguracoesView::setupScreen()
{
    ConfiguracoesViewBase::setupScreen();
}

void ConfiguracoesView::tearDownScreen()
{
    ConfiguracoesViewBase::tearDownScreen();
}


//1kSPS selecionado
void ConfiguracoesView::Set1kSPS()
{
		presenter->P_1kSPS_selected();

}

void ConfiguracoesView::Set500SPS()
{

		presenter->P_500SPS_selected();

}

void ConfiguracoesView::Set250SPS()
{

		presenter->P_250SPS_selected();

}

void ConfiguracoesView::Set125SPS()
{
		presenter->P_125SPS_selected();

}

void ConfiguracoesView::SetGain12()
{
	presenter->P_Gain12_selected();
}

void ConfiguracoesView::SetGain8()
{
	presenter->P_Gain8_selected();
}

void ConfiguracoesView::SetGain6()
{
	presenter->P_Gain6_selected();
}

void ConfiguracoesView::SetGain4()
{
	presenter->P_Gain4_selected();
}

void ConfiguracoesView::SetGain3()
{
	presenter->P_Gain3_selected();
}

void ConfiguracoesView::SetGain2()
{
	presenter->P_Gain2_selected();
}

void ConfiguracoesView::SetGain1()
{
	presenter->P_Gain1_selected();
}

void ConfiguracoesView::StartRDataC()
{
	presenter->PresenterStartRDataC();
}
