#ifndef CONTAINERLISTADEINSTRUCOES_HPP
#define CONTAINERLISTADEINSTRUCOES_HPP

#include <gui_generated/containers/ContainerListaDeInstrucoesBase.hpp>

class ContainerListaDeInstrucoes : public ContainerListaDeInstrucoesBase
{
public:
    ContainerListaDeInstrucoes();
    virtual ~ContainerListaDeInstrucoes() {}

    virtual void initialize();

    void setListElements(int item);
protected:
};

#endif // CONTAINERLISTADEINSTRUCOES_HPP
