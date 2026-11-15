#include "CPalette.h"
#include "DxLib.h"
#include "ColorRGB.h"
#include "PaletteLayout.h"

CPalette::CPalette()
{
	// 選んでいる項目の番号(最初は先頭=地面を選んだ状態にする)
	onPalette = -1;
	selected = 0;

	// ---- パレットに並べる項目を、並べたい順に追加する ----
	//			   {   置くブロック,     表示名,     横の色,縦の色,SIDEで使えるか, TOPで使えるか }
	items.push_back({ BlockType::None,   "消しゴム",   WHITE, WHITE, true,  true });
	items.push_back({ BlockType::Ground, "地面",       BROWN, BLACK, true,  true });
	items.push_back({ BlockType::DStart, "下スタート", RED,   RED,   true,  false });
	items.push_back({ BlockType::UStart, "上スタート", RED,   RED,   true,  false });
	items.push_back({ BlockType::BStart, "奥スタート", RED,   RED,   true,  false });
}

void CPalette::Input(FloatXY mouse, Layer layer)
{
	onPalette = -1;
	const bool leftDown = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

	for (int i = 0; i < static_cast<int>(items.size()); i++)
	{
		// 描いている四角と同じ範囲で判定する
		if (PaletteLayout::IsInsideItem(i, mouse.x, mouse.y))
		{
			onPalette = i;

			// 今のレイヤーで使える項目だけ選べる
			if (leftDown && CanUse(items[i], layer))
			{
				selected = i;
			}
			break; // 項目は重ならないので、見つかったら終わり
		}
	}

	// ---- レイヤーを切り替えて、選んでいる項目が使えなくなったとき ----
	// 並び順に頼らず、「使えるか」で確かめる
	if (!CanUse(items[selected], layer))
	{
		// 使える項目のうち、先頭のものを選び直す
		for (int i = 0; i < static_cast<int>(items.size()); i++)
		{
			if (CanUse(items[i], layer))
			{
				selected = i;
				break;
			}
		}
	}
}