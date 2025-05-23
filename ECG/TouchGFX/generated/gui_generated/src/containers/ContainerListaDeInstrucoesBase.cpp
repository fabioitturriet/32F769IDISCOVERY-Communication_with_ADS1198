/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ContainerListaDeInstrucoesBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

ContainerListaDeInstrucoesBase::ContainerListaDeInstrucoesBase()
{
    setWidth(500);
    setHeight(80);
    box1.setPosition(0, 0, 500, 80);
    box1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));

    textArea1.setPosition(73, 24, 422, 33);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_GIV2));

    image2.setXY(-134, 78);
    image2.setBitmap(touchgfx::Bitmap(BITMAP_BLUE_DIVIDERS_DIVIDER_ROUND_LONG_ID));

    toggleButton1.setXY(13, 17);
    toggleButton1.setBitmaps(touchgfx::Bitmap(BITMAP_BLUE_ICONS_DONE_48_ID), touchgfx::Bitmap(BITMAP_BLUE_ICON_DONE_CLIKED_48_ID));

    add(box1);
    add(textArea1);
    add(image2);
    add(toggleButton1);
}

void ContainerListaDeInstrucoesBase::initialize()
{

}

