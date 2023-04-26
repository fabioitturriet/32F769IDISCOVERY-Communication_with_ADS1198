#include <gui/containers/ContainerBatteryCharge.hpp>
#include <cstring>

extern uint8_t Percent_battery;

ContainerBatteryCharge::ContainerBatteryCharge()
{

}

void ContainerBatteryCharge::initialize()
{
    ContainerBatteryChargeBase::initialize();
}

void ContainerBatteryCharge::SetChargeLevelBattery()
{
	Unicode::snprintf(textArea1Buffer, TEXTAREA1_SIZE, "%u", Percent_battery);
	textArea1.invalidate();
}
