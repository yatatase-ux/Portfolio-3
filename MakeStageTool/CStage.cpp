#include "CStage.h"
#include "DxLib.h"

CStage::CStage(StageData& s)
	:stage(s)
{
	isLeftDown = false;
	isPainting = false;
}

void CStage::Paint(int cellX, int cellY, bool onCell, BlockType block, Layer layer)
{
	// パネルの上など、置いてはいけない場所を指しているときは何もしない
	if (!onCell) return;

	// 置く(ゴールの列なら、StageData がステージを1列伸ばしてから置く)
	stage.SetBlockType(layer, cellX, cellY, block);
}