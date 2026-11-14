#pragma once
#include <memory>
#include "StageData.h"

#include "CStage.h"
#include "VStage.h"

#include "CGoalLine.h"
#include "VGoalLine.h"

class StageScreen
{
	StageData& stage;
	int size;
	int color[2];

	int selectX;
	int selectY;

	bool isInScreen;

	// ---- スクロール ----
	// どこを映しているか(ステージの左端から何ピクセル右が、画面の左端に来ているか)
	// 小数だとブロックの間に隙間が出るので int で持つ
	int offsetX = 0;

	// 右ドラッグ用
	bool isDragging = false;   // 右ボタンでドラッグ中か
	int dragStartMouseX = 0;   // ドラッグを始めたときのマウスの x
	int dragStartOffsetX = 0;  // ドラッグを始めたときの offsetX

	// ---- ステージ ----
	std::unique_ptr<CStage> cStage;
	std::unique_ptr<VStage> vStage;

	// ---- ゴールライン ----
	std::unique_ptr<CGoalLine> cGoal;
	std::unique_ptr<VGoalLine> vGoal;

public:
	StageScreen(StageData& s);

	void Input(float mouseX, float mouseY);
	void Draw(Layer layer);

private:
	// offsetX をステージの範囲に収める
	void ClampOffset();

	// 画面上の x <--> ワールドの x(描くときと、マウスで指すときで逆向きに使う)
	int WorldToScreenX(int worldX) const { return worldX - offsetX; }
	int ScreenToWorldX(int screenX) const { return screenX + offsetX; }
};