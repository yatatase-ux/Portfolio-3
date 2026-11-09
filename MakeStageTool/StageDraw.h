#pragma once
#include "StageData.h"

class StageDraw
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

public:
	StageDraw(StageData& s);

	void Input(float mouseX, float mouseY);
	void Draw(Layer layer);

private:
	// offsetX をステージの範囲に収める
	void ClampOffset();

	// 画面上の x <--> ワールドの x(描くときと、マウスで指すときで逆向きに使う)
	int WorldToScreenX(int worldX) const { return worldX - offsetX; }
	int ScreenToWorldX(int screenX) const { return screenX + offsetX; }
};

