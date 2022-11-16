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
protected:
};

#endif // CADASTROVIEW_HPP
