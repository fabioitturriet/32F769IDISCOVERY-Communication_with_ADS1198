#ifndef CADASTROPRESENTER_HPP
#define CADASTROPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class CadastroView;

class CadastroPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    CadastroPresenter(CadastroView& v);

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

    virtual ~CadastroPresenter() {};

    virtual void P_InitESP();
    virtual void PWiFi_OFF();

    virtual void UpdateScrollList();

    virtual void DefNumRedes(uint8_t NumeroRedes);

    virtual void PAtualizarRedes();

    virtual void GeraPopup();
private:
    CadastroPresenter();

    CadastroView& view;
};

#endif // CADASTROPRESENTER_HPP
