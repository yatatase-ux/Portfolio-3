#pragma once
#include <memory>
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"
#include "StageScreen.h"
#include "VPanel.h"

class GameLoop
{
	StageData stage;
	std::unique_ptr<StageScreen> sd;
	VPanel td;

	Layer currentLayer;

public:
	GameLoop();

	void Loop();
};