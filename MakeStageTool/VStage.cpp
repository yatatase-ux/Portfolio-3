// VStage.cpp
#include "VStage.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"
#include <algorithm>
#include "StageData.h"


VStage::VStage()
{
	sideColor = GetColor(165, 42, 42);
	topColor = GetColor(0, 0, 0);
	startColor = GetColor(255, 0, 0);
}

unsigned int VStage::LayerColor(Layer layer) const
{
	// enum の数値に頼らず、レイヤーごとに色を対応させる
	return (layer == Layer::Side) ? sideColor : topColor;
}

void VStage::Draw(const StageData& stage, Layer editing, int offsetX) const
{
	const int cell = static_cast<int>(CELL_SIZE);
	const int width = stage.GetStageSize();

	// 編集していないほうのレイヤー(半透明で手前に重ねる)
	const Layer front = (editing == Layer::Side) ? Layer::Top : Layer::Side;

	// ---- 画面に映っている列だけ描く ----
	const int startX = offsetX / cell;
	const int endX = (std::min)(width, (offsetX + STAGE_SCREEN_W) / cell + 1);

	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			// マスのワールド座標 → 画面上の座標(縦はスクロールしない)
			const int dX = x * cell - offsetX;
			const int dY = y * cell;

			// 編集中のレイヤー(不透明)
			if (stage.GetBlockType(editing, x, y) == BlockType::Ground)
			{
				DrawBox(dX, dY, dX + cell, dY + cell, LayerColor(editing), TRUE);
			}

			// 編集していないレイヤー(半透明で手前に)
			if (stage.GetBlockType(front, x, y) == BlockType::Ground)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(dX, dY, dX + cell, dY + cell, LayerColor(front), TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			// スタート地点(横用のレイヤーにだけある)
			const BlockType s = stage.GetBlockType(Layer::Side, x, y);
			if (s == BlockType::DStart || s == BlockType::UStart || s == BlockType::BStart)
			{
				DrawBox(dX, dY, dX + cell, dY + cell, startColor, TRUE);
			}
		}
	}
}