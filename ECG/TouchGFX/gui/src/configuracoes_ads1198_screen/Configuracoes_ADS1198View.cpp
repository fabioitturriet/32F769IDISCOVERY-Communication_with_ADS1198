#include <gui/configuracoes_ads1198_screen/Configuracoes_ADS1198View.hpp>

Configuracoes_ADS1198View::Configuracoes_ADS1198View()
{

}

void Configuracoes_ADS1198View::setupScreen()
{
    Configuracoes_ADS1198ViewBase::setupScreen();
}

void Configuracoes_ADS1198View::tearDownScreen()
{
    Configuracoes_ADS1198ViewBase::tearDownScreen();
}


//1kSPS selecionado
void Configuracoes_ADS1198View::Set1kSPS()
{
		presenter->P_1kSPS_selected();

}

void Configuracoes_ADS1198View::Set500SPS()
{

		presenter->P_500SPS_selected();

}

void Configuracoes_ADS1198View::Set250SPS()
{

		presenter->P_250SPS_selected();

}

void Configuracoes_ADS1198View::Set125SPS()
{
		presenter->P_125SPS_selected();

}

void Configuracoes_ADS1198View::SetGain12()
{
	presenter->P_Gain12_selected();
}

void Configuracoes_ADS1198View::SetGain8()
{
	presenter->P_Gain8_selected();
}

void Configuracoes_ADS1198View::SetGain6()
{
	presenter->P_Gain6_selected();
}

void Configuracoes_ADS1198View::SetGain4()
{
	presenter->P_Gain4_selected();
}

void Configuracoes_ADS1198View::SetGain3()
{
	presenter->P_Gain3_selected();
}

void Configuracoes_ADS1198View::SetGain2()
{
	presenter->P_Gain2_selected();
}

void Configuracoes_ADS1198View::SetGain1()
{
	presenter->P_Gain1_selected();
}

void Configuracoes_ADS1198View::StartRDataC()
{
	presenter->PresenterStartRDataC();
}

void Configuracoes_ADS1198View::SetModoNormal()
{
	presenter->PresenterSetModoNormal();
}

void Configuracoes_ADS1198View::SetModoTeste()
{
	presenter->PresenterSetModoTeste();
}

void Configuracoes_ADS1198View::SetModoMensRuido()
{
	presenter->PresenterSetModoMensRuido();
}

void Configuracoes_ADS1198View::LOFF_off()
{
	presenter->P_LOFF_off();
}

void Configuracoes_ADS1198View::LOFF_DCResistor()
{
	presenter->P_LOFF_DCResistor();
}

void Configuracoes_ADS1198View::LOFF_DCSource()
{
	presenter->P_LOFF_DCSource();
}

void Configuracoes_ADS1198View::LOFF_AC()
{
	presenter->P_LOFF_AC();
}
