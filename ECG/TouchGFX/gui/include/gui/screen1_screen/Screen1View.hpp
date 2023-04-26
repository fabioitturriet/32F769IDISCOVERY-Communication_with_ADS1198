#ifndef SCREEN1VIEW_HPP
#define SCREEN1VIEW_HPP

#include <gui_generated/screen1_screen/Screen1ViewBase.hpp>
#include <gui/screen1_screen/Screen1Presenter.hpp>

class Screen1View : public Screen1ViewBase
{
public:
    Screen1View();
    virtual ~Screen1View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    void UpdateGraph(float value);

    void MUpdateMatrixLOFF();

    virtual void StopData();
    virtual void AjusteEscala();
    virtual void SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala);

    virtual void NextDeriv();
    virtual void BackDeriv();

    virtual void AtualizaTextDerv();

	virtual void WifiECG();

    virtual void FechaPopup();
    virtual void FechaPopup1();

    virtual void PopUp();

    virtual void FechaStatLead();

    virtual void AbreStatLead();

	void AtualizaBPM();
	void AtualizaIconHeart();

	virtual void StopLoadingAnimation();
protected:
};

#endif // SCREEN1VIEW_HPP
