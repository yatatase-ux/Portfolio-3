#include "GoalLine.h"
GoalLine::GoalLine(float iLine)
	:lineX(iLine)
{
	draw = std::make_unique <DrawGoal>();
}

void GoalLine::Input()
{

}

void GoalLine::Update()
{

}

void GoalLine::Draw()
{
//	draw->Line()
}