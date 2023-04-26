#include <gui/screen1_screen/Screen1View.hpp>

#include <texts/TextKeysAndLanguages.hpp>
#include <gui/containers/ContainerLoadingAnimation.hpp>
#include <gui/containers/ContainerPopup.hpp>
#include <gui/containers/ContainerLOFFMatrix.hpp>
#include <cstring>


 extern bool PanTompkinsAlgorithmSet;
 extern __IO uint8_t DerivSelecionada;
 extern __IO uint16_t BPMResultado;
 extern "C" {
 enum Derivacoes{dev_I, dev_II, dev_III, dev_aVR, dev_aVL, dev_aVF, dev_V1, dev_V2, dev_V3, dev_V4, dev_V5, dev_V6};

 struct Cadastro
 {
 	uint16_t nome[32];
 	uint16_t contato[32];
 	uint16_t genero;
 	uint16_t NascDia[3];
 	uint16_t NascMes[3];
 	uint16_t NascAno[5];
 };
 }

 extern struct Cadastro NewPacient;


Screen1View::Screen1View()
{
	if(NewPacient.nome[0] != 0){
		uint8_t i=0;
		while(NewPacient.nome[i] != 0x20){
			textArea1Buffer[i] = NewPacient.nome[i];
			i++;
			if(i == 12){
				break;
			}
		}
		textArea1Buffer[i] = 0;
	}

}

void Screen1View::setupScreen()
{


    Screen1ViewBase::setupScreen();

    AtualizaTextDerv();

    if(PanTompkinsAlgorithmSet){
    	presenter->PresenterAtualizarBPM();
    }else{
    	Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "off");
    	textArea2.invalidate();
    }

}

void Screen1View::tearDownScreen()
{
    Screen1ViewBase::tearDownScreen();
}

void Screen1View::UpdateGraph(float value)
{
	dynamicGraph1.addDataPoint(value); //adiciona o valor em um ponto no gráfico

}

void Screen1View::StopData()
{
	presenter->PresenterStopData();
	presenter->PreStopPanTompkins();
}

void Screen1View::AjusteEscala()
{
	presenter->PresenterAjusteEscala();
}

void Screen1View::SetMinMaxEscalaGraph(int16_t SetMinEscala, int16_t SetMaxEscala)
{
	dynamicGraph1.setGraphRangeY(SetMinEscala, SetMaxEscala);
}

void Screen1View::BackDeriv()
{
	presenter->PBackDeriv();
	AtualizaTextDerv();

}

void Screen1View::NextDeriv()
{
	presenter->PNextDeriv();
	AtualizaTextDerv();
}

void Screen1View::WifiECG()
{
	presenter->PreStopPanTompkins();
	scalableImage4_1.setVisible(false);
	containerLoadingAnimation1.setVisible(true);
	containerLoadingAnimation1.StartLoadingAnim();
	containerLoadingAnimation1.invalidate();
	presenter->PWifiECG();
}

void Screen1View::AtualizaTextDerv()
{
	textArea3.invalidate();
	switch (DerivSelecionada) {
	case dev_I:
		textArea3.setTypedText(TypedText(T_IDDEV1));
		break;
	case dev_II:
		textArea3.setTypedText(TypedText(T_IDDEV2));
		break;
	case dev_III:
		textArea3.setTypedText(TypedText(T_IDDEV3));
		break;
	case dev_aVR:
		textArea3.setTypedText(TypedText(T_IDDEV4));
		break;
	case dev_aVL:
		textArea3.setTypedText(TypedText(T_IDDEV5));
		break;
	case dev_aVF:
		textArea3.setTypedText(TypedText(T_IDDEV6));
		break;
	case dev_V1:
		textArea3.setTypedText(TypedText(T_IDDEV7));
		break;
	case dev_V2:
		textArea3.setTypedText(TypedText(T_IDDEV8));
		break;
	case dev_V3:
		textArea3.setTypedText(TypedText(T_IDDEV9));
		break;
	case dev_V4:
		textArea3.setTypedText(TypedText(T_IDDEV10));
		break;
	case dev_V5:
		textArea3.setTypedText(TypedText(T_IDDEV11));
		break;
	case dev_V6:
		textArea3.setTypedText(TypedText(T_IDDEV12));
		break;
	default:
		break;
	}
	textArea3.resizeToCurrentText();
	textArea3.invalidate();

}

void Screen1View::AtualizaBPM()
{
	Unicode::snprintf(textArea2Buffer, TEXTAREA2_SIZE, "%u", BPMResultado);
	textArea2.invalidate();
	scalableImage8.setVisible(true);
	scalableImage8.invalidate();
}

void Screen1View::AtualizaIconHeart()
{
	scalableImage8.setVisible(false);
	scalableImage8.invalidate();
}

void Screen1View::FechaPopup()
{
	containerPopup1.setVisible(false);
	buttonFechaPopup.setVisible(false);
	buttonFechaPopup1.setVisible(false);
	buttonFechaPopup.invalidate();
	buttonFechaPopup1.invalidate();
	containerPopup1.invalidate();
}

void Screen1View::FechaPopup1()
{
	containerPopup1.setVisible(false);
	buttonFechaPopup.setVisible(false);
	buttonFechaPopup1.setVisible(false);
	buttonFechaPopup.invalidate();
	buttonFechaPopup1.invalidate();
	containerPopup1.invalidate();
}

void Screen1View::PopUp()
{
	containerPopup1.AtualizaMsgErrorPopup();
	containerPopup1.setVisible(true);
	buttonFechaPopup.setVisible(true);
	buttonFechaPopup1.setVisible(true);
	buttonFechaPopup.invalidate();
	buttonFechaPopup1.invalidate();
	containerPopup1.invalidate();
}

void Screen1View::StopLoadingAnimation()
{
	scalableImage4_1.setVisible(true);
	scalableImage4_1.invalidate();
	containerLoadingAnimation1.StopLoadingAnim();
	containerLoadingAnimation1.setVisible(false);
	containerLoadingAnimation1.invalidate();
}

void Screen1View::MUpdateMatrixLOFF()
{
	containerLOFFMatrix1.UpdateMatrixStatusLOFF();
	containerLOFFMatrix1.invalidate();
}

void Screen1View::FechaStatLead()
{
	containerLOFFMatrix1.setVisible(false);
	buttonFechaStatLead.setVisible(false);
	containerLOFFMatrix1.invalidate();

}

void Screen1View::AbreStatLead()
{
	containerLOFFMatrix1.setVisible(true);
	containerLOFFMatrix1.invalidate();
	buttonFechaStatLead.setVisible(true);
	buttonFechaStatLead.invalidate();
}
