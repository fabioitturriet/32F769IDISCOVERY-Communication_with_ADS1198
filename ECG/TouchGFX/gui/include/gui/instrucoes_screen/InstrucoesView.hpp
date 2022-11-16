#ifndef INSTRUCOESVIEW_HPP
#define INSTRUCOESVIEW_HPP

#include <gui_generated/instrucoes_screen/InstrucoesViewBase.hpp>
#include <gui/instrucoes_screen/InstrucoesPresenter.hpp>

class InstrucoesView : public InstrucoesViewBase
{
public:
    InstrucoesView();
    virtual ~InstrucoesView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void scrollList1UpdateItem(ContainerListaDeInstrucoes& item, int16_t itemIndex);
protected:
    // Callback que é executado quando um item na lista de rolagem é selecionado.
    // O paramentro itemSelected é o item selecionado.
    Callback<InstrucoesView, int16_t> scrollList1_ItemSelectedCallback;
    void scrollList1_ItemSelectedHandler(int16_t itemSelected);
};

#endif // INSTRUCOESVIEW_HPP
