// PaletteLayout.h
#pragma once
#include "FloatXY.h"
#include "ScreenSize.h"

// パレットの並べ方(描く側とクリック判定の側で、同じ値・同じ計算を使う)
namespace PaletteLayout
{
	// 項目の四角の大きさ
	constexpr float BOX_SIZE = 100.0f;

	// 項目どうしのすき間
	constexpr float X_INTERVAL = 20.0f;

	// 1つ目の項目の左端(下のパネルの左端からの位置)
	constexpr float BASE_X = 0.0f;

	// 項目の中心の y(下のパネルの上端から少し下げたところ)
	constexpr float CENTER_Y = STAGE_SCREEN_H + BOX_SIZE / 2.0f + 10.0f;

	// index 番目の項目の、中心の位置を返す
	inline FloatXY GetItemCenter(int index)
	{
		// 左端 + すき間 + (四角 + すき間)× 番号 + 四角の半分
		const float x = BASE_X + X_INTERVAL
			+ (BOX_SIZE + X_INTERVAL) * index
			+ BOX_SIZE / 2.0f;
		return { x, CENTER_Y };
	}

	// (mx, my) が index 番目の項目の四角の中にあるか
	inline bool IsInsideItem(int index, float mx, float my)
	{
		const FloatXY c = GetItemCenter(index);
		const float half = BOX_SIZE / 2.0f;
		return (mx >= c.x - half && mx < c.x + half &&
			my >= c.y - half && my < c.y + half);
	}
}