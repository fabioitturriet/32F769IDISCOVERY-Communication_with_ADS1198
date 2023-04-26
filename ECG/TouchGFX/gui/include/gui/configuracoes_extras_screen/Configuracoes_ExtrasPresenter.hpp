#ifndef CONFIGURACOES_EXTRASPRESENTER_HPP
#define CONFIGURACOES_EXTRASPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Configuracoes_ExtrasView;

class Configuracoes_ExtrasPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    Configuracoes_ExtrasPresenter(Configuracoes_ExtrasView& v);

    /**
     * The activate function is called automatically when this screen is "switched in"
     * (ie. made active). Initialization logic can be placed here.
     */
    virtual void activate();

    /**
     * The deactivate function is called automatically when this screen is "switched out"
     * (ie. made inactive). Teardown functionality can be placed here.
     */
    virtual void deactivate();

    virtual ~Configuracoes_ExtrasPresenter() {};

private:
    Configuracoes_ExtrasPresenter();

    Configuracoes_ExtrasView& view;
};

#endif // CONFIGURACOES_EXTRASPRESENTER_HPP
