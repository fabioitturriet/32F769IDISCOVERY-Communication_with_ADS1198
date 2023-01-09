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

protected:
};

#endif // CONFIGURACOESVIEW_HPP
