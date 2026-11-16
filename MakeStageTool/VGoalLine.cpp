#include "VGoalLine.h"
#include "Shape.h"
#include "ColorRGB.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"

VGoalLine::VGoalLine()
{

}

void VGoalLine::Draw(int stageWidth, int offsetX, bool isGrabbing, int targetX) const
{
	// 今のゴールの位置(ワールド座標 → 画面上の座標)
	const float screenX = stageWidth * CELL_SIZE - offsetX;

	// ゴールの列の背景(半透明の黒)
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(screenX, 0, screenX + CELL_SIZE, STAGE_SCREEN_H, BLACK, TRUE);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	// 今のゴール(不透明)
	DrawGoalAt(screenX, 255);

	// つかんでいる間は、移動先にも半透明で描く
	if (isGrabbing)
	{
		const float targetScreenX = targetX * CELL_SIZE - offsetX;
		DrawGoalAt(targetScreenX, 100);
	}
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

// 画面上の x の位置に、ゴールを1本描く(alpha:255 で不透明、小さいほど透ける)
void VGoalLine::DrawGoalAt(float screenX, int alpha) const
{
	const FloatXY topPos = { screenX, 0.0f };
	const FloatXY bottomPos = { screenX + CELL_SIZE / 2.0f, static_cast<float>(STAGE_SCREEN_H) };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	DrawCheckered(topPos, bottomPos, BLACK, WHITE, CELL_SIZE / 4.0f);
	DrawGoalGround(screenX);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}