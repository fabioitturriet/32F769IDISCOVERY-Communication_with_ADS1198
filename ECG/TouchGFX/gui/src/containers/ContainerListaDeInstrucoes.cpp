#include <gui/containers/ContainerListaDeInstrucoes.hpp>

#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

ContainerListaDeInstrucoes::ContainerListaDeInstrucoes()
{

}

void ContainerListaDeInstrucoes::initialize()
{
    ContainerListaDeInstrucoesBase::initialize();
}

void ContainerListaDeInstrucoes::setListElements(int item)
{
    switch (item)
    {
    case 0:
    	//image1.setBitmap(Bitmap(BITMAP_BLUE_ICONS_DONE_48_ID));
    	textArea1.setTypedText(TypedText(T_RESOURCEID4));
    	break;
    case 1:
    	//image1.setBitmap(Bitmap(BITMAP_BLUE_ICONS_DONE_48_ID));
    	textArea1.setTypedText(TypedText(T_RESOURCEID3));
    	break;
    case 2:
    	//image1.setBitmap(Bitmap(BITMAP_BLUE_ICONS_DONE_48_ID));
    	textArea1.setTypedText(TypedText(T_RESOURCEID2));
    	break;
    case 3:
    	//image1.setBitmap(Bitmap(BITMAP_BLUE_ICONS_DONE_48_ID));
    	textArea1.setTypedText(TypedText(T_RESOURCEID1));
    	break;
    default:
    	break;
    }
}
