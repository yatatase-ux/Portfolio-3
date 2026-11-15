#pragma once
#include "BlockType.h"
#include <vector>
#include "FloatXY.h"
#include "LayerEnum.h"

// パレットの1項目の情報(どこに並べるかは含まない)
struct PaletteItem
{
	BlockType   block;     // 置いたときのブロック(消しゴムなら None)
	const char* name;      // 表示する名前
	unsigned int sideColor;   // 横の表示する色
	unsigned int topColor;   // 縦の表示する色
	bool        canSide;   // SIDE レイヤーで使えるか
	bool        canTop;    // TOP レイヤーで使えるか
};

inline bool CanUse(const PaletteItem& item, Layer layer)
{
	return (layer == Layer::Side) ? item.canSide : item.canTop;
};

class CPalette
{
	std::vector<PaletteItem> items;
	

	int onPalette;
	int selected;

public:
	CPalette();

	void Input(FloatXY mouse, Layer layer);


	// ---- ゲッター関数 ----
	// 並べる項目の一覧(描くときに使う。書き換えられないよう const の参照で返す)
	const std::vector<PaletteItem>& GetItems() const { return items; }
	// マウスが乗ってるパレット
	int GetOnPalette() const { return onPalette; }
	// 選んでいる項目の番号(描くときに、枠を付ける項目を知るため)
	int GetSelected() const { return selected; };
	// 選んでいるブロック(ステージに置くときに使う)
	BlockType GetSelectedBlock() const { return items[selected].block; }

};

