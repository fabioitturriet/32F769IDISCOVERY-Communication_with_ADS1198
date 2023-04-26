#ifndef CADASTROVIEW_HPP
#define CADASTROVIEW_HPP

#include <gui_generated/cadastro_screen/CadastroViewBase.hpp>
#include <gui/cadastro_screen/CadastroPresenter.hpp>

class CadastroView : public CadastroViewBase
{
public:
    CadastroView();
    virtual ~CadastroView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void initCadastro();

    virtual void ToggleWiFi();

    virtual void scrollList1UpdateItem(ContainerConexaoWIFI& item, int16_t itemIndex);
    virtual void itensAlterados();

    virtual void defnumredes(uint8_t NumeroRedes);
    virtual void AtualizarRedes();

    virtual void FechaPopup();
    virtual void FechaPopup1();

    virtual void PopUp();
protected:
    // Callback que é executado quando um item na lista de rolagem é selecionado.
    // O paramentro itemSelected é o item selecionado.
    Callback<CadastroView, int16_t> scrollList1_ItemSelectedCallback;
    void scrollList1_ItemSelectedHandler(int16_t itemSelected);
};

#endif // CADASTROVIEW_HPP
