/*********************************************************************************/
/********** THIS FILE IS GENERATED BY TOUCHGFX DESIGNER, DO NOT MODIFY ***********/
/*********************************************************************************/
#include <gui_generated/containers/ContainerConexaoWIFIBase.hpp>
#include <touchgfx/Color.hpp>
#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

ContainerConexaoWIFIBase::ContainerConexaoWIFIBase()
{
    setWidth(400);
    setHeight(40);
    box1.setPosition(0, 0, 400, 40);
    box1.setColor(touchgfx::Color::getColorFromRGB(255, 255, 255));

    textArea1.setPosition(0, 5, 400, 31);
    textArea1.setColor(touchgfx::Color::getColorFromRGB(0, 0, 0));
    textArea1.setLinespacing(0);
    textArea1Buffer[0] = 0;
    textArea1.setWildcard(textArea1Buffer);
    textArea1.setTypedText(touchgfx::TypedText(T___SINGLEUSE_AAKT));

    image1.setXY(-90, 38);
    image1.setBitmap(touchgfx::Bitmap(BITMAP_BLUE_DIVIDERS_DIVIDER_ROUND_LONG_ID));

    add(box1);
    add(textArea1);
    add(image1);
}

void ContainerConexaoWIFIBase::initialize()
{

}

