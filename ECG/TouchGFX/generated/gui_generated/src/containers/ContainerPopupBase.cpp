/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ContainerPopupBase.hpp>
#include <BitmapDatabase.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <touchgfx/Color.hpp>

ContainerPopupBase::ContainerPopupBase()
{
    setWidth(800);
    setHeight(480);
    image2.setXY(160, 104);
    image2.setBitmap(touchgfx::Bitmap(BITMAP_DARK_BACKGROUNDS_MAIN_BG_TEXTURE_480X272PX_ID));

    image1.setXY(588, 324);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_DARK_ICONS_WARNING_48_ID));

    textArea1.setPosition(173, 162, 454, 156);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));
    textArea1.setLinespacing(0);
    textArea1Buffer[0] = 0;
    textArea1.setWildcard(textArea1Buffer);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_OVUX));

    add(image2);
    add(image1);
    add(textArea1);
}

void ContainerPopupBase::initialize()
{

}
