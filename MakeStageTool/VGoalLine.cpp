#include "VGoalLine.h"
#include "Shape.h"
#include "ColorRGB.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"

VGoalLine::VGoalLine()
{

}

void VGoalLine::Draw(int stageWidth, int offsetX) const
{
	// ---- ゴールの位置(ワールド座標) ----
	// ゴールは「ステージの幅」の列 = ステージ本編のすぐ右
	const float worldX = stageWidth * CELL_SIZE;

	// ---- 画面上の座標に直す ----
	// ループで最後に描いたマスではなく、幅から直接求めるので、
	// どこまでスクロールしていても正しい位置に描かれる
	const float screenX = worldX - offsetX;

	// ---- 線の上端と下端(画面の上から下まで) ----
	const FloatXY topPos = { screenX, 0.0f };
	const FloatXY bottomPos = { screenX + CELL_SIZE / 2.0f, static_cast<float>(STAGE_SCREEN_H) };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(topPos.x, topPos.y, topPos.x + CELL_SIZE, bottomPos.y, BLACK, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	DrawCheckered(topPos, bottomPos, BLACK, WHITE, CELL_SIZE / 4.0f);

	DrawGoalGround(screenX);

}

void VGoalLine::DrawGoalGround(const float screenX) const
{	
	// ゴールの地面の厚さ(ゲームと同じ定数を使う)
	const float groundH = GOAL_GROUND_ROWS * CELL_SIZE;
	// 上の地面:画面の上端から、厚さの分だけ
	DrawBox(screenX, 0, screenX + CELL_SIZE, groundH, BROWN, TRUE);

	// 下の地面:ステージの下端から、厚さの分だけ上
	const float stageBottom = STAGE_HEIGHT * CELL_SIZE;
	DrawBox(screenX, stageBottom - groundH, screenX + CELL_SIZE, stageBottom, BROWN, TRUE);
}