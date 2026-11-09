#include "StageDraw.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"

StageDraw::StageDraw(StageData& s)
	:stage(s)
{
	size = stage.GetStageSize();
	color[0] = GetColor(165, 42, 42);
	color[1] = GetColor(0, 0, 0);
}

void StageDraw::Draw(Layer layer)
{
	Layer front = (layer == Layer::Side) ? Layer::Top : Layer::Side;
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = 0; x < size; x++)
		{
			int dX = CELL_SIZE * x;
			int dY = CELL_SIZE * y;

			BlockType b = stage.GetBlockType(layer, x, y);
			if(b == BlockType::Ground)
			{
				DrawBox(dX, dY, dX + CELL_SIZE, dY + CELL_SIZE, color[static_cast<int>(layer)], TRUE);
			}

			BlockType f = stage.GetBlockType(front, x, y);
			if (f == BlockType::Ground)
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
				DrawBox(dX, dY, dX + CELL_SIZE, dY + CELL_SIZE, color[static_cast<int>(front)], TRUE);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			}

			if (stage.GetBlockType(Layer::Side, x, y) == BlockType::DStart || 
				stage.GetBlockType(Layer::Side, x, y) == BlockType::UStart || 
				stage.GetBlockType(Layer::Side, x, y) == BlockType::BStart )
			{
				DrawBox(dX, dY, dX + CELL_SIZE, dY + CELL_SIZE, GetColor(255,0,0), TRUE);
			}
		}
	}

}