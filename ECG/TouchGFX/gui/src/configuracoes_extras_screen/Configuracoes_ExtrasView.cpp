#include <gui/configuracoes_extras_screen/Configuracoes_ExtrasView.hpp>

bool PanTompkinsAlgorithmSet = true;
bool BatteryChargeLevelVisibleSet = true;

Configuracoes_ExtrasView::Configuracoes_ExtrasView()
{

}

void Configuracoes_ExtrasView::setupScreen()
{
    Configuracoes_ExtrasViewBase::setupScreen();
    if(PanTompkinsAlgorithmSet) toggleButtonPanTompkins.forceState(true);
    else toggleButtonPanTompkins.forceState(false);

    if(BatteryChargeLevelVisibleSet) toggleButtonBatteryLevelVisible.forceState(true);
    else toggleButtonBatteryLevelVisible.forceState(true);

}

void Configuracoes_ExtrasView::tearDownScreen()
{
    Configuracoes_ExtrasViewBase::tearDownScreen();
}

void Configuracoes_ExtrasView::TogglePanTompkinsSet()
{
	if(toggleButtonPanTompkins.getState()) PanTompkinsAlgorithmSet = true;
	else PanTompkinsAlgorithmSet = false;
}

void Configuracoes_ExtrasView::BatteryLevelSetVisible()
{
	if(toggleButtonBatteryLevelVisible.getPressedState()) BatteryChargeLevelVisibleSet = true;
	else BatteryChargeLevelVisibleSet = false;
}
