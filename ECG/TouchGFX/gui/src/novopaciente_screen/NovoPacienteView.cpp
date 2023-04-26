#include <gui/novopaciente_screen/NovoPacienteView.hpp>

#include <string.h>

enum Dado {NOME, CONTATO, NASC_DIA, NASC_MES, NASC_ANO};

extern "C" {
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




uint8_t DadoSelecionado;

NovoPacienteView::NovoPacienteView()
{
	keyboard.setPosition(240, 188, 320, 240); //add
	keyboard.setVisible(false);

	Unicode::strncpy(textNomeBuffer, NewPacient.nome, Unicode::strlen(NewPacient.nome)+1);
	textNomeBuffer[Unicode::strlen(NewPacient.nome)+2] = 0;
	Unicode::strncpy(textContatoBuffer, NewPacient.contato, Unicode::strlen(NewPacient.contato)+1);
	textContatoBuffer[Unicode::strlen(NewPacient.contato)+2] = 0;
	Unicode::strncpy(textDataNascDiaBuffer, NewPacient.NascDia, Unicode::strlen(NewPacient.NascDia)+1);
	textDataNascDiaBuffer[2] = 0;
	Unicode::strncpy(textDataNascMesBuffer, NewPacient.NascMes, Unicode::strlen(NewPacient.NascMes)+1);
	textDataNascMesBuffer[2] = 0;
	Unicode::strncpy(textDataNascAnoBuffer, NewPacient.NascAno, Unicode::strlen(NewPacient.NascAno)+1);
	textDataNascAnoBuffer[4] = 0;

	if(NewPacient.genero == 0){

	}else if (NewPacient.genero == 'F'){
		radioButtonSexoF.setSelected(true);
	}else{
		radioButtonSexoM.setSelected(true);
	}


    add(keyboard); //add
}

void NovoPacienteView::setupScreen()
{
    NovoPacienteViewBase::setupScreen();
}

void NovoPacienteView::tearDownScreen()
{
    NovoPacienteViewBase::tearDownScreen();
}

void NovoPacienteView::SalvarCadastro()
{

}

void NovoPacienteView::SalvarDadosTeclado()
{
	Unicode::UnicodeChar* buff = keyboard.getBuffer();
	switch (DadoSelecionado)
	{
	case NOME:
		Unicode::strncpy(NewPacient.nome, buff,Unicode::strlen(buff)+1);
		Unicode::strncpy(textNomeBuffer, NewPacient.nome, Unicode::strlen(NewPacient.nome)+1);
		textNomeBuffer[Unicode::strlen(NewPacient.nome)+2] = 0;
		textNome.invalidate();
		break;
	case CONTATO:
		Unicode::strncpy(NewPacient.contato, buff,Unicode::strlen(buff)+1);
		Unicode::strncpy(textContatoBuffer, NewPacient.contato, Unicode::strlen(NewPacient.contato)+1);
		textContatoBuffer[Unicode::strlen(NewPacient.contato)+2] = 0;
		textContato.invalidate();
		break;
	case NASC_DIA:
		Unicode::strncpy(NewPacient.NascDia, buff,2);
		Unicode::strncpy(textDataNascDiaBuffer, NewPacient.NascDia, Unicode::strlen(NewPacient.NascDia)+1);
		textDataNascDiaBuffer[2] = 0;
		textDataNascDia.invalidate();
		break;
	case NASC_MES:
		Unicode::strncpy(NewPacient.NascMes, buff,2);
		Unicode::strncpy(textDataNascMesBuffer, NewPacient.NascMes, Unicode::strlen(NewPacient.NascMes)+1);
		textDataNascMesBuffer[2] = 0;
		textDataNascMes.invalidate();
		break;
	case NASC_ANO:
		Unicode::strncpy(NewPacient.NascAno, buff,4);
		Unicode::strncpy(textDataNascAnoBuffer, NewPacient.NascAno, Unicode::strlen(NewPacient.NascAno)+1);
		textDataNascAnoBuffer[4] = 0;
		textDataNascAno.invalidate();
		break;
	default:
		break;
	}
	FecharTeclado();
}

void NovoPacienteView::NewNamePacient()
{
	if(NewPacient.nome[0]!= 0){
		keyboard.setDataBuffer(NewPacient.nome);
	}
	AbrirTeclado();
	DadoSelecionado = NOME;
}

void NovoPacienteView::NewContatoPacient()
{
	if(NewPacient.contato[0]!= 0){
		keyboard.setDataBuffer(NewPacient.contato);
	}
	AbrirTeclado();
	DadoSelecionado = CONTATO;
}

void NovoPacienteView::NewDataNascDia()
{
	if(NewPacient.NascDia[0]!= 0){
		keyboard.setDataBuffer(NewPacient.NascDia);
	}
	AbrirTeclado();
	DadoSelecionado = NASC_DIA;
}

void NovoPacienteView::NewDataNascMes()
{
	if(NewPacient.NascMes[0]!= 0){
		keyboard.setDataBuffer(NewPacient.NascMes);
	}
	AbrirTeclado();
	DadoSelecionado = NASC_MES;
}

void NovoPacienteView::NewDataNascAno()
{
	if(NewPacient.NascAno[0]!= 0){
		keyboard.setDataBuffer(NewPacient.NascAno);
	}
	AbrirTeclado();
	DadoSelecionado = NASC_ANO;
}

void NovoPacienteView::MaculinoSelected()
{
	NewPacient.genero = 'M';
}

void NovoPacienteView::FemininoSelected()
{
	NewPacient.genero = 'F';
}

void NovoPacienteView::AbrirTeclado()
{
	keyboard.setVisible(true);
	buttonSalvarDados.setVisible(true);
	voltar.setTouchable(false);
	buttonSalvarCadastro.setTouchable(false);
	keyboard.invalidate();
	buttonSalvarDados.invalidate();
	voltar.invalidate();
	buttonSalvarCadastro.invalidate();
}

void NovoPacienteView::FecharTeclado()
{
	keyboard.clearKeyboard();
	keyboard.setVisible(false);
	buttonSalvarDados.setVisible(false);
	voltar.setTouchable(true);
	buttonSalvarCadastro.setTouchable(true);
	keyboard.invalidate();
	buttonSalvarDados.invalidate();
	voltar.invalidate();
	buttonSalvarCadastro.invalidate();
}

void NovoPacienteView::LimpaDados()
{
	for(uint8_t i=0; i<32;i++){
		NewPacient.nome[i] = 0;
		NewPacient.contato[i] = 0;
	}
	for(uint8_t i=0; i<3;i++){
		NewPacient.NascDia[i] = 0;
		NewPacient.NascMes[i] = 0;
	}
	for(uint8_t i=0; i<5;i++){
		NewPacient.NascAno[i] = 0;
	}
	NewPacient.genero = 0;

	Unicode::strncpy(textNomeBuffer, NewPacient.nome, Unicode::strlen(NewPacient.nome)+1);
	textNomeBuffer[Unicode::strlen(NewPacient.nome)+2] = 0;
	Unicode::strncpy(textContatoBuffer, NewPacient.contato, Unicode::strlen(NewPacient.contato)+1);
	textContatoBuffer[Unicode::strlen(NewPacient.contato)+2] = 0;
	Unicode::strncpy(textDataNascDiaBuffer, NewPacient.NascDia, Unicode::strlen(NewPacient.NascDia)+1);
	textDataNascDiaBuffer[2] = 0;
	Unicode::strncpy(textDataNascMesBuffer, NewPacient.NascMes, Unicode::strlen(NewPacient.NascMes)+1);
	textDataNascMesBuffer[2] = 0;
	Unicode::strncpy(textDataNascAnoBuffer, NewPacient.NascAno, Unicode::strlen(NewPacient.NascAno)+1);
	textDataNascAnoBuffer[3] = 0;

	textNome.invalidate();
	textContato.invalidate();
	textDataNascDia.invalidate();
	textDataNascMes.invalidate();
	textDataNascAno.invalidate();

	radioButtonSexoF.setSelected(false);
	radioButtonSexoM.setSelected(false);

	radioButtonSexoF.invalidate();
	radioButtonSexoM.invalidate();

}
