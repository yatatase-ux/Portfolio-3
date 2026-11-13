#pragma once
#include <memory>
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"
#include "StageScreen.h"
#include "TabDraw.h"

class GameLoop
{
	StageData stage;
	std::unique_ptr<StageScreen> sd;
	TabDraw td;

	Layer currentLayer;

public:
	GameLoop();

	void Loop();
};