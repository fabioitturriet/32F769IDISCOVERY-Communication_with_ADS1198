#ifndef CONFIGURACOESVIEW_HPP
#define CONFIGURACOESVIEW_HPP

#include <gui_generated/configuracoes_screen/ConfiguracoesViewBase.hpp>
#include <gui/configuracoes_screen/ConfiguracoesPresenter.hpp>

class ConfiguracoesView : public ConfiguracoesViewBase
{
public:
    ConfiguracoesView();
    virtual ~ConfiguracoesView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

protected:
};

#endif // CONFIGURACOESVIEW_HPP
