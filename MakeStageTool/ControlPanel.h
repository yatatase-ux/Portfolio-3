#pragma once
#include <memory>
#include "VPanel.h"

class ControlPanel
{
	std::unique_ptr<VPanel> vPanel;

public:

	ControlPanel();

	void Update();
	void Draw();

};

