#include "VPanel.h"
#include "DxLib.h"
#include "ScreenSize.h"
#include "StringF.h"
#include "Shape.h"
#include "ColorRGB.h"
#include "PanelLayout.h"

VPanel::VPanel()
{

}

void VPanel::Draw(Layer layer)
{
	DrawBackGround();
	DrawLayer(layer);
	DrawSaveButton();
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

void VPanel::DrawSaveButton()
{
	using namespace PanelLayout;

	// マウスが乗っているときは少し明るくする(押せることが分かるように)
	int mx = 0, my = 0;
	GetMousePoint(&mx, &my);
	const bool hover = IsInsideSaveButton(static_cast<float>(mx), static_cast<float>(my));
	const unsigned int bg = hover ? GetColor(90, 90, 90) : GetColor(70, 70, 70);

	// ボタンの四角と枠
	DrawBoxAA(SAVE_X, SAVE_Y, SAVE_X + SAVE_W, SAVE_Y + SAVE_H, bg, TRUE);
	DrawBoxAA(SAVE_X, SAVE_Y, SAVE_X + SAVE_W, SAVE_Y + SAVE_H, GetColor(200, 200, 200), FALSE, 2.0f);

	// 文字(ボタンの中央)
	DrawCenterString(SAVE_X + SAVE_W / 2.0f, SAVE_Y + SAVE_H / 2.0f,
		"保存 (Ctrl+S)", GetColor(255, 255, 255), 20.0f);
}