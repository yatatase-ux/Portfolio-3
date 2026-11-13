#pragma once
#include "FloatXY.h"

class VGoalLine
{
public:
	VGoalLine();

	// stageWidth:ステージの横のマス数(ゴールはこの列に描く)
	// offsetX   :スクロールの量(ワールド座標 → 画面上の座標に直すのに使う)
	void Draw(int stageWidth, int offsetX) const;

private:
	void DrawGoalGround(const float screenX) const;
};

