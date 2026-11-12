#pragma once
#include "FloatXY.h"
#include "DrawGoal.h"
#include <memory>

class GoalLine
{
	float lineX;
	float topY;
	float bottomY;

	std::unique_ptr<DrawGoal> draw;

	GoalLine(float iLine);

public:

	~GoalLine() {};
	// ÉRÉsÅ[ã÷é~
	GoalLine(const GoalLine&) = delete;
	GoalLine& operator=(const GoalLine&) = delete;

	static GoalLine& Instance(float iLine)
	{
		static GoalLine instance(iLine);
		return instance;
	}

	void Input();

	void Update();

	void Draw();

};

