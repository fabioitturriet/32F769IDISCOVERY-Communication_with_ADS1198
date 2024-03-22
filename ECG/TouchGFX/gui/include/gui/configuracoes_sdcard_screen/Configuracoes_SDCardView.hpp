#ifndef CONFIGURACOES_SDCARDVIEW_HPP
#define CONFIGURACOES_SDCARDVIEW_HPP

#include <gui_generated/configuracoes_sdcard_screen/Configuracoes_SDCardViewBase.hpp>
#include <gui/configuracoes_sdcard_screen/Configuracoes_SDCardPresenter.hpp>

class Configuracoes_SDCardView : public Configuracoes_SDCardViewBase
{
public:
    Configuracoes_SDCardView();
    virtual ~Configuracoes_SDCardView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // CONFIGURACOES_SDCARDVIEW_HPP
