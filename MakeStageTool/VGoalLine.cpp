#include "VGoalLine.h"
#include "Shape.h"
#include "ColorRGB.h"
#include "DxLib.h"
#include "StageConst.h"

VGoalLine::VGoalLine()
{

}

void VGoalLine::Line(int index_x, int index_top, int index_bottom)
{
	FloatXY topPos = { index_x * CELL_SIZE, index_top * CELL_SIZE };
	FloatXY bottomPos = { index_x * CELL_SIZE + CELL_SIZE / 2.0f, index_bottom * CELL_SIZE };

	DrawCheckered(topPos, bottomPos, BLACK, WHITE, CELL_SIZE / 4.0f);
}