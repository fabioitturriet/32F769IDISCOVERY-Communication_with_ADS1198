#ifndef INSTRUCOESPRESENTER_HPP
#define INSTRUCOESPRESENTER_HPP

#include <gui/model/ModelListener.hpp>
#include <mvp/Presenter.hpp>

using namespace touchgfx;

class InstrucoesView;

class InstrucoesPresenter : public touchgfx::Presenter, public ModelListener
{
public:
    InstrucoesPresenter(InstrucoesView& v);

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

    virtual ~InstrucoesPresenter() {};

private:
    InstrucoesPresenter();

    InstrucoesView& view;
};

#endif // INSTRUCOESPRESENTER_HPP
