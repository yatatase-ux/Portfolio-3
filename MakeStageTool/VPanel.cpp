#include "VPanel.h"
#include "DxLib.h"
#include "ScreenSize.h"
#include "StringF.h"
#include "Shape.h"
#include "ColorRGB.h"

VPanel::VPanel()
{

}

void VPanel::Draw(Layer layer)
{
	DrawBackGround();
	DrawLayer(layer);
}

void VPanel::DrawBackGround()
{
	DrawBox(STAGE_SCREEN_W, 0, WINDOW_W, WINDOW_H, GRAY50, TRUE);
	DrawBox(0, STAGE_SCREEN_H, WINDOW_W, WINDOW_H, GRAY50, TRUE);
}

void VPanel::DrawLayer(Layer layer)
{
	const char* txt = (layer == Layer::Side) ? "SIDE" : "TOP";
	float x = STAGE_SCREEN_W + PANEL_THICKNESS / 2.0f;
	float y = STAGE_SCREEN_H + PANEL_THICKNESS / 2.0f;

	DrawCenterBox({ x, y }, { PANEL_THICKNESS , PANEL_THICKNESS }, GRAY200, FALSE, 5.0f);
	DrawCenterString(x, y, txt, GetColor(255, 255, 0), 50.0f);
}
