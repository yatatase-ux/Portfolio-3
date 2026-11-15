#pragma once
#include <memory>
#include "StageData.h"
#include "LayerEnum.h"
#include "FloatXY.h"
#include "BlockType.h"

#include "CStage.h"
#include "VStage.h"

#include "CGoalLine.h"
#include "VGoalLine.h"

class StageScreen
{
	StageData& stage;
	int size;

	int selectX;
	int selectY;

	bool isInScreen;

	// ---- スクロール ----
	// どこを映しているか(ステージの左端から何ピクセル右が、画面の左端に来ているか)
	// 小数だとブロックの間に隙間が出るので int で持つ
	int offsetX;

	// 右ドラッグ用
	bool isRightDragging;    // 右ボタンでドラッグ中か
	bool isRightDown;		 // 右クリックが押されているか
	float dragStartMouseX;   // ドラッグを始めたときのマウスの x
	float dragStartOffsetX;  // ドラッグを始めたときの offsetX

	// ---- ステージ ----
	std::unique_ptr<CStage> cStage;
	std::unique_ptr<VStage> vStage;

	// ---- ゴールライン ----
	std::unique_ptr<CGoalLine> cGoal;
	std::unique_ptr<VGoalLine> vGoal;

public:
	StageScreen(StageData& s);

	void Input(FloatXY mouse, BlockType selectBlock, Layer layer);
	void Draw(Layer layer);

private:
	// 画面スクロール処理
	void ScrollScreen(FloatXY mouse, bool inStageArea);
	// offsetX をステージの範囲に収める
	void ClampOffset();

	// 画面上の x <--> ワールドの x(描くときと、マウスで指すときで逆向きに使う)
	int WorldToScreenX(int worldX) const { return worldX - offsetX; }
	int ScreenToWorldX(int screenX) const { return screenX + offsetX; }
};