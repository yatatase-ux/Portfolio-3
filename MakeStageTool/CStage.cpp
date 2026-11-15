#include "CStage.h"
#include "DxLib.h"

CStage::CStage(StageData& s)
	:stage(s)
{
	isLeftDown = false;
	isPainting = false;
}

void CStage::Input(int cellX, int cellY, bool onCell, bool inStageArea,
	BlockType block, Layer layer)
{
	const bool inputLeft = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;
	if (inputLeft && !isPainting)
	{
		if (!isLeftDown && inStageArea)
		{
			isPainting = true;
		}
		isLeftDown = true;
	}
	else if (!inputLeft)
	{
		isLeftDown = false;
		isPainting = false;
	}

	if (isPainting && onCell)
	{
		stage.SetBlockType(layer, cellX, cellY, block);
	}

}