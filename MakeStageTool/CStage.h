#pragma once
#include "FloatXY.h"
#include "BlockType.h"
#include "LayerEnum.h"
#include "StageData.h"

class CStage
{
	StageData& stage;      // 書き換える先のステージ

	bool isLeftDown;       // 左ボタンが押されているか
	bool isPainting;       // ステージ枠の中で押し始めた「塗る操作」の途中か

public:
	CStage(StageData& s);

	// cellX, cellY :マウスが指しているマス
	// onCell       :置いていいマスを指しているか(ステージの中かゴールの列)
	// inStageArea  :マウスがステージ枠の中か(押し始めの判定に使う)
	// block, layer :何を、どのレイヤーに置くか
	void Input(int cellX, int cellY, bool onCell, bool inStageArea,
		BlockType block, Layer layer);

	// 塗っている途中か(StageScreen が端の自動スクロールに使う)
	bool IsPainting() const { return isPainting; }

private:
};

