#include <gui/menu_screen/MENUView.hpp>

#include <gui/containers/ContainerBatteryCharge.hpp>


extern bool BatteryChargeLevelVisibleSet;
extern bool UpdateBatChargeLevel;

MENUView::MENUView()
{

}

void MENUView::setupScreen()
{
    MENUViewBase::setupScreen();

    if (BatteryChargeLevelVisibleSet){
    	containerBatteryCharge1.setVisible(true);
    	UpdateBatChargeLevel = true;
    }
    else {
    	containerBatteryCharge1.setVisible(false);
        }
}

void MENUView::tearDownScreen()
{
    MENUViewBase::tearDownScreen();
}

void MENUView::UpdateBatteryChargeLevel()
{
	containerBatteryCharge1.SetChargeLevelBattery();
}
