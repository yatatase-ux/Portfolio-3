#include "TabDraw.h"
#include "DxLib.h"
#include "ScreenSize.h"

TabDraw::TabDraw()
{
	tabColor = GetColor(50, 50, 50);
}

void TabDraw::Draw()
{

	DrawBox(STAGE_SCREEN_W, 0, MAIN_SCREEN_W, MAIN_SCREEN_H, tabColor, TRUE);
	DrawBox(0, STAGE_SCREEN_H, MAIN_SCREEN_W, MAIN_SCREEN_H, tabColor, TRUE);
}