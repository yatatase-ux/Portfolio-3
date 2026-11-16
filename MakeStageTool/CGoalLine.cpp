#include "CGoalLine.h"
#include "StageData.h"
#include <algorithm>

CGoalLine::CGoalLine(StageData& d)
	:stage(d)
{
	isGrabbing = false;
	targetX = 0;
}

void CGoalLine::Begin(int cellX)
{
	isGrabbing = true;
	// 最初の移動先は、今のゴールの位置(= 今のステージの幅)
	targetX = stage.GetStageSize();
}

void CGoalLine::Drag(int cellX)
{
	if (!isGrabbing) return;

	// マウスが指している列を、移動先として覚える
	// ただし、短くしすぎないように下限で止める(下限の決め方は下で説明)
	const int minWidth = 1; // ← 仮。あとでルールを決めて置き換える
	targetX = (std::max)(cellX, minWidth);
}

void CGoalLine::End()
{
	if (!isGrabbing) return;

	const int width = stage.GetStageSize();

	if (targetX > width)
	{
		// 右へ動かした → 伸ばす
		stage.ExtendWidth(targetX);
	}
	else if (targetX < width)
	{
		// 左へ動かした → 縮める(StageData に縮める関数を足す必要がある)
		stage.ShrinkWidth(targetX);
	}

	isGrabbing = false;
}