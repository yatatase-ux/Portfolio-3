#pragma once
#include "FloatXY.h"
#include <memory>

class StageData;

class CGoalLine
{
	StageData& stage;

	bool isGrabbing;    // つかんでいる途中か
	int  targetX;       // 移動先の列(= 離したときのステージの幅)

public:

	CGoalLine(StageData& d);
	void Begin(int cellX);
	void Drag(int cellX);
	void End();

	// VGoalLine が描くときに使う
	bool IsGrabbing() const { return isGrabbing; }
	int  GetTargetX() const { return targetX; }


};

