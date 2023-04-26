#ifndef CONFIGURACOES_EXTRASVIEW_HPP
#define CONFIGURACOES_EXTRASVIEW_HPP

#include <gui_generated/configuracoes_extras_screen/Configuracoes_ExtrasViewBase.hpp>
#include <gui/configuracoes_extras_screen/Configuracoes_ExtrasPresenter.hpp>

class Configuracoes_ExtrasView : public Configuracoes_ExtrasViewBase
{
public:
    Configuracoes_ExtrasView();
    virtual ~Configuracoes_ExtrasView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void TogglePanTompkinsSet();
    virtual void BatteryLevelSetVisible();
protected:
};

#endif // CONFIGURACOES_EXTRASVIEW_HPP
