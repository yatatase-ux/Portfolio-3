#include "SelectStage.h"
#include "EditStage.h"
#include "SceneManager.h"

#include "DxLib.h"
#include <iostream>
#include <algorithm>
#include "StageData.h"
#include "ScreenSize.h"
#include "Shape.h"
#include "StringF.h"
#include "ColorRGB.h"

SelectStage::SelectStage()
{

}

SCENE_ENTER(SelectStage)
{
	// =======================================
	//  初期化処理
	// =======================================
	stageCount = StageData::CountStages();
//	stageCount = 20;
	baseY = 0.0f;
	selectStage = -1;
	GetMouseWheelRotVol();
}

/// <summary>
/// 入力処理
/// </summary>
SCENE_INPUT(SelectStage)
{
	MouseScroll(mouse);

	if (mouse->Push(MouseButton::Left))
	{
		bool newData;
		for (int i = 0; i < stageCount + 1; i++)
		{
			FloatXY boxPos = IndexToBoxPos(i);
			if (mouse->CheckInBox(boxPos, boxSize))
			{
				selectStage = i;
			    newData = (i == stageCount);
			}
		}
		// どれかの項目をクリックしたときだけ切り替える
		if (selectStage >= 0)
		{
			manager->ChangeState(std::make_unique<EditStage>(selectStage, newData));
		}
	}
}
void SelectStage::MouseScroll(MouseAction* mouse)
{
	// 前回この関数を呼んでから、ホイールがどれだけ回ったかを返す
	// 奥(画面の上の方向)に回すとプラス、手前に回すとマイナス
	// 1目盛り回すごとに 1 増える(減る)
	const int wheel = mouse->GetWheel();
	// 1目盛りで何ピクセル動かすか
	const float SCROLL_SPEED = 60.0f;
	// 奥に回す(プラス)と一覧が下に動いて、上のほうの項目が見えるようにする
	// → 一覧の位置(baseY:どれだけ下まで見ているか)は減らす
	baseY -= wheel * SCROLL_SPEED;
	// ---- 一番上と一番下で止める ----
	// 項目の数(New Stage の分も含めて +1)× 1項目分の高さ = 一覧全体の高さ
	const float listHeight = boxSpace + (stageCount + 1) * (boxSize.y + boxSpace);
	// スクロールできる一番下(一覧が画面より短いときは 0 で止める)
	const float maxScroll = (std::max)(0.0f, listHeight - WINDOW_H);
	baseY = std::clamp(baseY, 0.0f, maxScroll);
}

/// <summary>
/// 更新処理
/// </summary>
SCENE_UPDATE(SelectStage)
{
	
}

SCENE_DRAW(SelectStage)
{
	for (int i = 0; i < stageCount + 1; i++)
	{
		FloatXY boxPos = IndexToBoxPos(i);
//		FloatXY textPos = { boxPos.x - 500.0f, boxPos.y};

		DrawCenterBox(boxPos, boxSize, WHITE, FALSE, 2.0f);
		if (i < stageCount)
		{
			// ステージの番号を入れて描く(例:Stage0)
			DrawCenterFormat(boxPos.x, boxPos.y, YELLOW, 50.0f,  "Stage%d", i);
		}
		else
		{
			// 一番下は新しいステージを作る項目
			DrawCenterFormat(boxPos.x, boxPos.y, YELLOW, 50.0f, "New Stage", i);
		}
	}

//	DrawFormatString(20, 20, GetColor(255, 255, 0), "selectStage = %d", selectStage);
}

SCENE_EXIT(SelectStage)
{

}

FloatXY SelectStage::IndexToBoxPos(int i)
{
	return { WINDOW_W / 2.0f,
			((float)i * boxSize.y + boxSpace * (float)(i + 1)) + boxSize.y / 2.0f - baseY };
}