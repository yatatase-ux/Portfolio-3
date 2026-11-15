#include "StageScreen.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"
#include "Shape.h"
#include <algorithm>   // std::clamp, std::min, std::max
#include "ColorRGB.h"

/// <summary>
/// コンストラクタ
/// </summary>
/// <param name="s">ステージのデータ</param>
StageScreen::StageScreen(StageData& s)
	:stage(s)
{
	size = stage.GetStageSize();

	selectX = -1;
	selectY = -1;

	isInScreen = false;

	offsetX = 0;

	isRightDragging = false;   
	isRightDown = false;
	dragStartMouseX = 0.0f;
	dragStartOffsetX = 0.0f;

	vStage = std::make_unique<VStage>();
	cStage = std::make_unique<CStage>(stage);

	cGoal = std::make_unique<CGoalLine>(stage);
	vGoal = std::make_unique<VGoalLine>();
}

/// <summary>
/// 入力処理
/// </summary>
/// <param name="mouseX">マウスのX座標</param>
/// <param name="mouseY">マウスのY座標</param>
void StageScreen::Input(FloatXY mouse, BlockType block, Layer layer)
{
	// マスの大きさ(整数で計算したいので int にしておく)
	const int cell = static_cast<int>(CELL_SIZE);

	// ステージの横幅は、別のステージを開いたときに変わるので毎回取り直す
	size = stage.GetStageSize();

	// マウスがステージの表示領域の中にあるか
	// (1280 や 720 ちょうどは帯の側なので、< で比べる)
	const bool inStageArea = (mouse.x >= 0 && mouse.y >= 0 &&
		mouse.x < STAGE_SCREEN_W && mouse.y < STAGE_SCREEN_H);

	// ---- 右ドラッグでスクロール ----
	ScrollScreen(mouse, inStageArea);

	// ---- 選択中のマス ----
	// 画面上の位置 → ワールドの位置 → マスの番号
	selectX = ScreenToWorldX(mouse.x) / CELL_SIZE;
	selectY = mouse.y / CELL_SIZE;

	// ステージの表示領域の中で、かつマスがある場所だけを「選択中」にする
	// (ドラッグ中は選択の枠を出さない)
	isInScreen = inStageArea && !isRightDragging &&
		selectX >= 0 && selectX < size + 1 &&
		selectY >= 0 && selectY < STAGE_HEIGHT;


	if (cStage->IsPainting())
	{
		// 端から何ピクセル以内でスクロールを始めるか
		const int EDGE = 40;
		// 1フレームにスクロールする量(大きいほど速い)
		const int EDGE_SCROLL_SPEED = 8;

		if (mouse.x > STAGE_SCREEN_W - EDGE)
		{
			// 右端に近い → 右へスクロール(映す場所を右へずらす)
			offsetX += EDGE_SCROLL_SPEED;
		}
		else if (mouse.x < EDGE)
		{
			// 左端に近い → 左へスクロール
			offsetX -= EDGE_SCROLL_SPEED;
		}

		// スクロールしすぎないように収める
		ClampOffset();
	}

	cStage->Input(selectX, selectY, isInScreen, inStageArea, block, layer);
}

void StageScreen::ScrollScreen(FloatXY mouse, bool inStageArea)
{
	const bool inputRight = (GetMouseInput() & MOUSE_INPUT_RIGHT) != 0;

	if (inputRight && !isRightDragging)
	{
		// 押した瞬間:ドラッグ開始。開始時のマウス位置と offsetX を覚える
		if(!isRightDown && inStageArea)
		{
			isRightDragging = true;
			dragStartMouseX = mouse.x;
			dragStartOffsetX = offsetX;
		}
		isRightDown = true;
	}
	else if (!inputRight)
	{
		// 離したら:ドラッグ終了
		isRightDragging = false;
		isRightDown = false;
	}

	if (isRightDragging)
	{
		// マウスを右に動かすと、ステージをつかんで右に引っぱる感覚にする
		// → 映す場所(offsetX)は左に戻る、なので「引く」
		offsetX = dragStartOffsetX - (mouse.x - dragStartMouseX);
	}

	// ステージの外が映らないように収める
	ClampOffset();
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