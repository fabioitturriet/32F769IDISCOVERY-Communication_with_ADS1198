#ifndef CONFIGURACOES_ADS1198VIEW_HPP
#define CONFIGURACOES_ADS1198VIEW_HPP

#include <gui_generated/configuracoes_ads1198_screen/Configuracoes_ADS1198ViewBase.hpp>
#include <gui/configuracoes_ads1198_screen/Configuracoes_ADS1198Presenter.hpp>

class Configuracoes_ADS1198View : public Configuracoes_ADS1198ViewBase
{
public:
    Configuracoes_ADS1198View();
    virtual ~Configuracoes_ADS1198View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();


    virtual void Set1kSPS();
    virtual void Set500SPS();
    virtual void Set250SPS();
    virtual void Set125SPS();

    virtual void SetGain12();
    virtual void SetGain8();
    virtual void SetGain6();
    virtual void SetGain4();
    virtual void SetGain3();
    virtual void SetGain2();
    virtual void SetGain1();

    virtual void StartRDataC();

    virtual void SetModoNormal();
    virtual void SetModoTeste();
    virtual void SetModoMensRuido();

    virtual void LOFF_off();
    virtual void LOFF_DCResistor();
    virtual void LOFF_DCSource();
    virtual void LOFF_AC();

protected:
};

#endif // CONFIGURACOES_ADS1198VIEW_HPP
