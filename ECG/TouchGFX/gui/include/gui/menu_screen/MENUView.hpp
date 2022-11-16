#ifndef MENUVIEW_HPP
#define MENUVIEW_HPP

#include <gui_generated/menu_screen/MENUViewBase.hpp>
#include <gui/menu_screen/MENUPresenter.hpp>

class MENUView : public MENUViewBase
{
public:
    MENUView();
    virtual ~MENUView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // MENUVIEW_HPP
