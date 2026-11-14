#include "StageScreen.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"
#include "Shape.h"
#include <algorithm>   // std::clamp, std::min, std::max
#include "ColorRGB.h"

StageScreen::StageScreen(StageData& s)
	:stage(s)
{
	size = stage.GetStageSize();
	color[0] = BROWN;
	color[1] = BLACK;

	selectX = -1;
	selectY = -1;

	isInScreen = false;

	cGoal = std::make_unique<CGoalLine>(stage);
	vGoal = std::make_unique<VGoalLine>();
}

void StageScreen::Input(float mouseX, float mouseY)
{
	// マスの大きさ(整数で計算したいので int にしておく)
	const int cell = static_cast<int>(CELL_SIZE);

	// ステージの横幅は、別のステージを開いたときに変わるので毎回取り直す
	size = stage.GetStageSize();

	// マウスがステージの表示領域の中にあるか
	// (1280 や 720 ちょうどは帯の側なので、< で比べる)
	const bool inStageArea = (mouseX >= 0 && mouseY >= 0 &&
		mouseX < STAGE_SCREEN_W && mouseY < STAGE_SCREEN_H);

	// ---- 右ドラッグでスクロール ----
	const bool rightDown = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;

	if (rightDown && !isDragging && inStageArea)
	{
		// 押した瞬間:ドラッグ開始。開始時のマウス位置と offsetX を覚える
		isDragging = true;
		dragStartMouseX = mouseX;
		dragStartOffsetX = offsetX;
	}
	else if (!rightDown)
	{
		// 離したら:ドラッグ終了
		isDragging = false;
	}

	if (isDragging)
	{
		// マウスを右に動かすと、ステージをつかんで右に引っぱる感覚にする
		// → 映す場所(offsetX)は左に戻る、なので「引く」
		offsetX = dragStartOffsetX - (mouseX - dragStartMouseX);
	}

	// ステージの外が映らないように収める
	ClampOffset();

	// ---- 選択中のマス ----
	// 画面上の位置 → ワールドの位置 → マスの番号
	selectX = ScreenToWorldX(mouseX) / CELL_SIZE;
	selectY = mouseY / CELL_SIZE;

	// ステージの表示領域の中で、かつマスがある場所だけを「選択中」にする
	// (ドラッグ中は選択の枠を出さない)
	isInScreen = inStageArea && !isDragging &&
		selectX >= 0 && selectX < size + 1 &&
		selectY >= 0 && selectY < STAGE_HEIGHT;
}

void StageScreen::ClampOffset()
{
	const int cell = static_cast<int>(CELL_SIZE);

	// スクロールできる一番右(ステージの幅 − 表示領域の幅)
	// ステージが表示領域より狭いときはマイナスになるので、0 で止める
	// (下限 > 上限 で clamp を呼ぶとエラーになるのを防ぐ)
	const int maxOffset = (std::max)(0, (size + 1) * cell - STAGE_SCREEN_W);

	offsetX = std::clamp(offsetX, 0, maxOffset);
}

void StageScreen::Draw(Layer layer)
{
	// ステージのマス目(描き方は VStage に任せる)
	vStage->Draw(stage, layer, offsetX);

	// ゴールライン
	vGoal->Draw(size, offsetX);

	// ---- 選択中のマスの枠 ----(今はここに残す)
	if (isInScreen)
	{
		const int cell = static_cast<int>(CELL_SIZE);
		const int sX = WorldToScreenX(selectX * cell);
		const int sY = selectY * cell;
		DrawBox(sX, sY, sX + cell, sY + cell, GetColor(255, 255, 0), FALSE, 5);
	}
}