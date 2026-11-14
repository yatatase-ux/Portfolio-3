// VStage.h
#pragma once
#include "LayerEnum.h"

class StageData;

// ステージのマス目を描く(見た目だけ。データは書き換えない)
class VStage
{
	unsigned int sideColor;  // 横用の地面の色
	unsigned int topColor;   // 見下ろし用の床の色
	unsigned int startColor; // スタート地点の色

	// レイヤーに対応する色を返す
	unsigned int LayerColor(Layer layer) const;

public:
	VStage();

	// stage   :描くステージのデータ(読むだけ)
	// editing :今編集しているレイヤー(こちらを不透明、もう片方を半透明で手前に描く)
	// offsetX :スクロールの量
	void Draw(const StageData& stage, Layer editing, int offsetX) const;
};