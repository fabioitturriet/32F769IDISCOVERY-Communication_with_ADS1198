#ifndef NOVOPACIENTEVIEW_HPP
#define NOVOPACIENTEVIEW_HPP

#include <gui_generated/novopaciente_screen/NovoPacienteViewBase.hpp>
#include <gui/novopaciente_screen/NovoPacientePresenter.hpp>

#include <gui/common/CustomKeyboard.hpp> //add

class NovoPacienteView : public NovoPacienteViewBase
{
public:
    NovoPacienteView();
    virtual ~NovoPacienteView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();

    virtual void SalvarCadastro();

    virtual void NewNamePacient();
    virtual void NewSobrenomePacient();
	virtual void NewContatoPacient();
	virtual void NewDataNascDia();
	virtual void NewDataNascMes();
	virtual void NewDataNascAno();
	virtual void MaculinoSelected();
	virtual void FemininoSelected();
	virtual void SalvarDadosTeclado();

	void AbrirTeclado();
	void FecharTeclado();

	virtual void LimpaDados();

protected:
	CustomKeyboard keyboard; // add
};

#endif // NOVOPACIENTEVIEW_HPP
