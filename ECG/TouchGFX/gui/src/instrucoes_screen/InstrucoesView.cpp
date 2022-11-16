#include <gui/instrucoes_screen/InstrucoesView.hpp>

#include <texts/TextKeysAndLanguages.hpp>
#include <BitmapDatabase.hpp>

InstrucoesView::InstrucoesView() :
scrollList1_ItemSelectedCallback(this, &InstrucoesView::scrollList1_ItemSelectedHandler)
{

}

void InstrucoesView::setupScreen()
{
	scrollList1.setItemSelectedCallback(scrollList1_ItemSelectedCallback);
    InstrucoesViewBase::setupScreen();
}

void InstrucoesView::tearDownScreen()
{
    InstrucoesViewBase::tearDownScreen();
}

void InstrucoesView::scrollList1UpdateItem(ContainerListaDeInstrucoes& item, int16_t itemIndex)
{
	item.setListElements(itemIndex);
}

// O Callback depois de selecionar o item na lista de rolagem
void InstrucoesView::scrollList1_ItemSelectedHandler(int16_t itemSelected)
{
	image1.invalidate();
	//textArea1.invalidate();

    switch (itemSelected)
    {
    case 0:
    	image1.setBitmap(Bitmap(BITMAP_LOGO_TECH4HEALTH_ID));
    	//textArea2.setTypedText(TypedText(T_RESOURCEID4));
    	break;
    case 1:
    	image1.setBitmap(Bitmap(BITMAP_VERSAOVERTICAL_RESIZED_ID));
    	//textArea2.setTypedText(TypedText(T_RESOURCEID3));
    	break;
    case 2:
    	image1.setBitmap(Bitmap(BITMAP_ICONS8_CLOUD_96_ID));
    	//textArea2.setTypedText(TypedText(T_RESOURCEID2));
    	break;
    case 3:
    	image1.setBitmap(Bitmap(BITMAP_ICONS8_PATIENT_64_ID));
    	//textArea2.setTypedText(TypedText(T_RESOURCEID1));
    	break;
    default:
    	break;
    }

	image1.invalidate();
}
