#pragma once
#include <memory>
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"

class GameLoop
{
	StageData stage;

public:
	GameLoop();

	void Loop();
};