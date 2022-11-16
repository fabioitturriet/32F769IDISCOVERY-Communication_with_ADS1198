#ifndef MENUPRESENTER_HPP
#define MENUPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class MENUView;

class MENUPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    MENUPresenter(MENUView& v);

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

    virtual ~MENUPresenter() {};

private:
    MENUPresenter();

    MENUView& view;
};

#endif // MENUPRESENTER_HPP
