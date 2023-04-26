#ifndef CONTAINERPOPUP_HPP
#define CONTAINERPOPUP_HPP

#include <gui_generated/containers/ContainerPopupBase.hpp>

class ContainerPopup : public ContainerPopupBase
{
public:
    ContainerPopup();
    virtual ~ContainerPopup() {}

    virtual void initialize();

    virtual void AtualizaMsgErrorPopup();
protected:
};

#endif // CONTAINERPOPUP_HPP
