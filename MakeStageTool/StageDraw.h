#pragma once
#include "StageData.h"

class StageDraw
{
	StageData& stage;
	int size;
	int color[2];

public:
	StageDraw(StageData& s);

	void Draw(Layer layer);
};

