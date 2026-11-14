#include "VPanel.h"
#include "DxLib.h"
#include "ScreenSize.h"

VPanel::VPanel()
{
	tabColor = GetColor(50, 50, 50);
}

void VPanel::Draw()
{
	DrawBox(STAGE_SCREEN_W, 0, WINDOW_W, WINDOW_H, tabColor, TRUE);
	DrawBox(0, STAGE_SCREEN_H, WINDOW_W, WINDOW_H, tabColor, TRUE);
}