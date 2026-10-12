#include "Shape.h"
#include "DxLib.h"

void DrawCenterBox(IntXY pos, IntXY size, unsigned int color, bool flag, int LineSize)
{
	float sX = pos.x - size.x / 2.0f;
	float sY = pos.y - size.y / 2.0f;
	float eX = sX + size.x;
	float eY = sY + size.y;

	DrawBoxAA(sX, sY, eX, eY, color, flag, LineSize);

}
void DrawCenterBox(FloatXY pos, FloatXY size, unsigned int color, bool flag, float LineSize)
{
	float sX = pos.x - size.x / 2.0f;
	float sY = pos.y - size.y / 2.0f;
	float eX = sX + size.x;
	float eY = sY + size.y;

	DrawBoxAA(sX, sY, eX, eY, color, flag, LineSize);

}