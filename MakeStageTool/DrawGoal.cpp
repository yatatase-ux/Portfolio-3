#include "DrawGoal.h"
#include "Shape.h"
#include "ColorRGB.h"
#include "DxLib.h"
#include "StageConst.h"

DrawGoal::DrawGoal()
{

}


void DrawGoal::Line(FloatXY topPos, FloatXY bottomPos)
{
	DrawCheckered(topPos, bottomPos, BLACK, WHITE, CELL_SIZE / 4.0f);
}