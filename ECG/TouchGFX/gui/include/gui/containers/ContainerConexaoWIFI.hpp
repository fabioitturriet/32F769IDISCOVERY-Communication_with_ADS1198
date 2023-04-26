#ifndef CONTAINERCONEXAOWIFI_HPP
#define CONTAINERCONEXAOWIFI_HPP

#include <gui_generated/containers/ContainerConexaoWIFIBase.hpp>

class ContainerConexaoWIFI : public ContainerConexaoWIFIBase
{
public:
    ContainerConexaoWIFI();
    virtual ~ContainerConexaoWIFI() {}

    virtual void initialize();

    void setListElements(int item);

protected:
};

#endif // CONTAINERCONEXAOWIFI_HPP
