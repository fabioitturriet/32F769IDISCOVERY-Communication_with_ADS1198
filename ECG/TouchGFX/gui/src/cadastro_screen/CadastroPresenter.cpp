#include <gui/cadastro_screen/CadastroView.hpp>
#include <gui/cadastro_screen/CadastroPresenter.hpp>

CadastroPresenter::CadastroPresenter(CadastroView& v)
    : view(v)
{

}

void CadastroPresenter::activate()
{

}

void CadastroPresenter::deactivate()
{

}

void CadastroPresenter::P_InitESP()
{
	model->M_InitESP();
}

void CadastroPresenter::PWiFi_OFF()
{
	model->WiFiESP_OFF();
}

void CadastroPresenter::UpdateScrollList()
{
	view.itensAlterados();
}

void CadastroPresenter::PAtualizarRedes()
{
	model->AtualizarListaRedes();
}

void CadastroPresenter::DefNumRedes(uint8_t NumeroRedes){
	view.defnumredes(NumeroRedes);
}

void CadastroPresenter::GeraPopup()
{
	view.PopUp();
}
