/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#ifndef CONFIGURACOES_SDCARDVIEWBASE_HPP
#define CONFIGURACOES_SDCARDVIEWBASE_HPP

#include <gui/common/FrontendApplication.hpp>
#include <mvp/View.hpp>
#include <gui/configuracoes_sdcard_screen/Configuracoes_SDCardPresenter.hpp>
#include <touchgfx/widgets/Box.hpp>
#include <touchgfx/widgets/TextArea.hpp>
#include <touchgfx/widgets/Button.hpp>
#include <touchgfx/containers/scrollers/ScrollList.hpp>
#include <gui/containers/ContainerConexaoWIFI.hpp>
#include <touchgfx/widgets/TextAreaWithWildcard.hpp>
#include <touchgfx/widgets/ButtonWithLabel.hpp>
#include <gui/containers/ContainerLoadingAnimation.hpp>

class Configuracoes_SDCardViewBase : public touchgfx::View<Configuracoes_SDCardPresenter>
{
public:
    Configuracoes_SDCardViewBase();
    virtual ~Configuracoes_SDCardViewBase() {}
    virtual void setupScreen();

    virtual void scrollList2UpdateItem(ContainerConexaoWIFI& item, int16_t itemIndex)
    {
        // Override and implement this function in Configuracoes_SDCard
    }

protected:
    FrontendApplication& application() {
        return *static_cast<FrontendApplication*>(touchgfx::Application::getInstance());
    }

    /*
     * Member Declarations
     */
    touchgfx::Box __background;
    touchgfx::Box box1;
    touchgfx::TextArea textArea;
    touchgfx::Box barra_divisoria;
    touchgfx::Button voltar;
    touchgfx::ScrollList scrollList2;
    touchgfx::DrawableListItems<ContainerConexaoWIFI, 10> scrollList2ListItems;
    touchgfx::TextAreaWithOneWildcard textArea1_1;
    touchgfx::TextArea textArea1;
    touchgfx::ButtonWithLabel buttonWithLabel1;
    ContainerLoadingAnimation containerLoadingAnimation1;

    /*
     * Wildcard Buffers
     */
    static const uint16_t TEXTAREA1_1_SIZE = 30;
    touchgfx::Unicode::UnicodeChar textArea1_1Buffer[TEXTAREA1_1_SIZE];

private:

    /*
     * Callback Declarations
     */
    touchgfx::Callback<Configuracoes_SDCardViewBase, const touchgfx::AbstractButton&> buttonCallback;
    touchgfx::Callback<Configuracoes_SDCardViewBase, touchgfx::DrawableListItemsInterface*, int16_t, int16_t> updateItemCallback;

    /*
     * Callback Handler Declarations
     */
    void buttonCallbackHandler(const touchgfx::AbstractButton& src);
    void updateItemCallbackHandler(touchgfx::DrawableListItemsInterface* items, int16_t containerIndex, int16_t itemIndex);

};

#endif // CONFIGURACOES_SDCARDVIEWBASE_HPP
