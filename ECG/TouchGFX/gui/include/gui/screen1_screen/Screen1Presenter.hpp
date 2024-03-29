#ifndef SCREEN1PRESENTER_HPP
#define SCREEN1PRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class Screen1View;

class Screen1Presenter : public touchgfx::Presenter, public ModelListener
{
public:
    Screen1Presenter(Screen1View& v);

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

    virtual ~Screen1Presenter() {};

    void UpdateGraph(float value);

    void UpdateMatrixLOFF();

    virtual void PresenterStopData();

    virtual void PresenterAjusteEscala();

    virtual void SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala);

    virtual void PBackDeriv();
    virtual void PNextDeriv();

    virtual void PSalvarECG();

    virtual void PresenterAtualizarBPM();
    virtual void PreStopPanTompkins();

    virtual void GeraPopup();

    void AtualizarBPM();
    void AtualizarIconHeart();

    virtual void PreStopLoadingAnimation();

    virtual void PresenterToggleF60();
    virtual void PresenterToggleFBW();

    virtual void UpdateBatChargeLevel();

private:
    Screen1Presenter();

    Screen1View& view;
};

#endif // SCREEN1PRESENTER_HPP
