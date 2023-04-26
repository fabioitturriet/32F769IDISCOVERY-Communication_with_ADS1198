#ifndef CONTAINERBATTERYCHARGE_HPP
#define CONTAINERBATTERYCHARGE_HPP

#include <gui_generated/containers/ContainerBatteryChargeBase.hpp>

class ContainerBatteryCharge : public ContainerBatteryChargeBase
{
public:
    ContainerBatteryCharge();
    virtual ~ContainerBatteryCharge() {}

    virtual void initialize();

    virtual void SetChargeLevelBattery();
protected:
};

#endif // CONTAINERBATTERYCHARGE_HPP
