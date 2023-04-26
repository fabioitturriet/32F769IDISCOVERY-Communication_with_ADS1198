#ifndef CONFIGURACOES_ADS1198PRESENTER_HPP
#define CONFIGURACOES_ADS1198PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Configuracoes_ADS1198View;

class Configuracoes_ADS1198Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Configuracoes_ADS1198Presenter(Configuracoes_ADS1198View& v);

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

    virtual ~Configuracoes_ADS1198Presenter() {};

    virtual void P_1kSPS_selected();
    virtual void P_500SPS_selected();
    virtual void P_250SPS_selected();
    virtual void P_125SPS_selected();

    virtual void P_Gain12_selected();
    virtual void P_Gain8_selected();
    virtual void P_Gain6_selected();
    virtual void P_Gain4_selected();
    virtual void P_Gain3_selected();
    virtual void P_Gain2_selected();
    virtual void P_Gain1_selected();

    virtual void PresenterStartRDataC();

    virtual void PresenterSetModoNormal();
    virtual void PresenterSetModoTeste();
    virtual void PresenterSetModoMensRuido();

	virtual void P_LOFF_off();
	virtual void P_LOFF_DCResistor();
	virtual void P_LOFF_DCSource();
	virtual void P_LOFF_AC();

private:
    Configuracoes_ADS1198Presenter();

    Configuracoes_ADS1198View& view;
};

#endif // CONFIGURACOES_ADS1198PRESENTER_HPP
