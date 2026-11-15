#include "VPanel.h"
#include "DxLib.h"
#include "ScreenSize.h"

VPanel::VPanel()
{
	bgColor = GetColor(50, 50, 50);
}

void VPanel::Draw(Layer layer)
{
	DrawBackGround();
}

void VPanel::DrawBackGround()
{
	DrawBox(STAGE_SCREEN_W, 0, WINDOW_W, WINDOW_H, bgColor, TRUE);
	DrawBox(0, STAGE_SCREEN_H, WINDOW_W, WINDOW_H, bgColor, TRUE);
}