#pragma once
#include "LayerEnum.h"
#include <memory>

#include "VPanel.h"

#include "CPalette.h"
#include "VPalette.h"

class ControlPanel
{
	std::unique_ptr<VPanel> vPanel;
	std::unique_ptr<CPalette> cPalette;
	std::unique_ptr<VPalette> vPalette;


public:

	ControlPanel();
	
	void Input(FloatXY mouse, Layer layer);
	void Update();
	void Draw(Layer layer);

};

