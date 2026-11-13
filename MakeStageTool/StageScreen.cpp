#include "StageScreen.h"
#include "DxLib.h"
#include "StageConst.h"
#include "ScreenSize.h"
#include "Shape.h"
#include <algorithm>   // std::clamp, std::min, std::max

StageScreen::StageScreen(StageData& s)
	:stage(s)
{
	size = stage.GetStageSize();
	color[0] = GetColor(165, 42, 42);
	color[1] = GetColor(0, 0, 0);

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
		selectX >= 0 && selectX < size &&
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
	Layer front = (layer == Layer::Side) ? Layer::Top : Layer::Side;

	// ---- 画面に映っている列だけ描く ----
	// 一番左に映っている列と、その右に何列映るか
	const int startX = offsetX / CELL_SIZE;
	// 端の列が半分だけ映ることがあるので +1 する。ステージの幅を超えないようにする
	const int endX = (std::min)(size, (offsetX + STAGE_SCREEN_W) / (int)CELL_SIZE + 1);

	float finalX;
	for (int y = 0; y < STAGE_HEIGHT; y++)
	{
		for (int x = startX; x < endX; x++)
		{
			// マスのワールド座標 → 画面上の座標
			const float dX = WorldToScreenX(x * CELL_SIZE);
			const float dY = y * CELL_SIZE; // 縦はスクロールしないのでそのまま

			BlockType b = stage.GetBlockType(layer, x, y);
			if (b == BlockType::Ground)
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

			const BlockType s = stage.GetBlockType(Layer::Side, x, y);
			if (s == BlockType::DStart || s == BlockType::UStart || s == BlockType::BStart)
			{
				DrawBox(dX, dY, dX + CELL_SIZE, dY + CELL_SIZE, GetColor(255, 0, 0), TRUE);
			}

			finalX = dX + CELL_SIZE;
		}
	}

	DrawCheckered({ finalX, 0 }, { finalX + CELL_SIZE / 2.0f, STAGE_SCREEN_H },
		GetColor(255, 255, 255), GetColor(0, 0, 0), CELL_SIZE / 4.0f);

	// ---- 選択中のマスの枠 ----
	if (isInScreen)
	{
		// 選択中のマスもワールド座標なので、画面上の座標に直して描く
		const int sX = WorldToScreenX(selectX * CELL_SIZE);
		const int sY = selectY * CELL_SIZE;
		DrawBox(sX, sY, sX + CELL_SIZE, sY + CELL_SIZE, GetColor(255, 255, 0), FALSE, 5);
	}
}