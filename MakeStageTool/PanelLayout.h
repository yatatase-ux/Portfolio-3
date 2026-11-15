#pragma once
#include "ScreenSize.h"

// パネルの中のボタンの並べ方(描く側とクリック判定の側で、同じ値を使う)
namespace PanelLayout
{
	// 保存ボタン(右のパネルの一番上)
	constexpr float SAVE_X = STAGE_SCREEN_W + 20.0f;        // 左端(右のパネルの左端から少し内側)
	constexpr float SAVE_Y = 20.0f;                         // 上端
	constexpr float SAVE_W = PANEL_THICKNESS - 40.0f;       // 幅(左右に 20 ずつ余白)
	constexpr float SAVE_H = 60.0f;                         // 高さ

	// (mx, my) が保存ボタンの上にあるか
	inline bool IsInsideSaveButton(float mx, float my)
	{
		return (mx >= SAVE_X && mx < SAVE_X + SAVE_W &&
			my >= SAVE_Y && my < SAVE_Y + SAVE_H);
	}
}