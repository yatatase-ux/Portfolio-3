#pragma once
#include <memory>
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"
#include "StageScreen.h"
#include "ControlPanel.h"

class GameLoop
{
	StageData stage;
	std::unique_ptr<StageScreen> stageScreen;
	std::unique_ptr<ControlPanel> controlPanel;

	Layer currentLayer;

public:
	GameLoop();

	void Loop();
};