#include "ControlPanel.h"

ControlPanel::ControlPanel()
{
	vPanel = std::make_unique<VPanel>();
}

void ControlPanel::Update()
{

}

void ControlPanel::Draw(Layer layer)
{
	vPanel->Draw(layer);
}