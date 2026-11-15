#include "ControlPanel.h"

ControlPanel::ControlPanel()
{
	vPanel = std::make_unique<VPanel>();

	cPalette = std::make_unique<CPalette>();
	vPalette = std::make_unique<VPalette>();
}

void ControlPanel::Input(FloatXY mouse, Layer layer)
{
	cPalette->Input(mouse, layer);
}

void ControlPanel::Update()
{

}

void ControlPanel::Draw(Layer layer)
{
	vPanel->Draw(layer);
	vPalette->Draw(cPalette->GetItems(), layer, cPalette->GetOnPalette(), cPalette->GetSelected());
}