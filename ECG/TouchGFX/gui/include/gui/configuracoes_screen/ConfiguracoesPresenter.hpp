#ifndef CONFIGURACOESPRESENTER_HPP
#define CONFIGURACOESPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class ConfiguracoesView;

class ConfiguracoesPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    ConfiguracoesPresenter(ConfiguracoesView& v);

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

    virtual ~ConfiguracoesPresenter() {};

private:
    ConfiguracoesPresenter();

    ConfiguracoesView& view;
};

#endif // CONFIGURACOESPRESENTER_HPP
