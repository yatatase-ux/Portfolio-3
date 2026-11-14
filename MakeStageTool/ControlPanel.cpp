#include "ControlPanel.h"

ControlPanel::ControlPanel()
{
	vPanel = std::make_unique<VPanel>();
}

void ControlPanel::Update()
{

}

void ControlPanel::Draw()
{
	vPanel->Draw();
}