#pragma once
#include "FloatXY.h"
#include <memory>

class StageData;

class CGoalLine
{
	float lineX;
	float topY;
	float bottomY;

	StageData& data;

public:

	CGoalLine(StageData& d);

	~CGoalLine() {};

	void Input();

	void Update();

};

