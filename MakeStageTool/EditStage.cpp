#include "EditStage.h"
#include "SelectStage.h"

#include <ctime>	
#include "DxLib.h"
#include <iostream>
#include "StringF.h"
#include "ScreenSize.h"
#include "PanelLayout.h"
#include "SceneManager.h"

EditStage::EditStage(int s, bool isN)
	:stageNum(s), isNewData(isN)
{

}

SCENE_ENTER(EditStage)
{
	// =======================================
	//  初期化処理
	// =======================================
	bool isLoaded;
	if (isNewData)
	{
		isLoaded = stage.CreateFromTemplate(stageNum);
	}
	else
	{
		isLoaded = stage.OpenFile(stageNum);
	}
	if (!isLoaded)
	{
		manager->ChangeState(std::make_unique<SelectStage>());
		return;
	}

	stageScreen = std::make_unique<StageScreen>(stage);
	controlPanel = std::make_unique<ControlPanel>();
	currentLayer = Layer::Side;
	messageTimer = 0;
	prevSaveKey = false;
}

SCENE_INPUT(EditStage)
{
	if (CheckHitKey(KEY_INPUT_1)) currentLayer = Layer::Side;
	if (CheckHitKey(KEY_INPUT_2)) currentLayer = Layer::Side;
	if (CheckHitKey(KEY_INPUT_3)) currentLayer = Layer::Top;

	if (key->Push(TAB))
	{
		manager->ChangeState(std::make_unique<SelectStage>());
		return;
	}

	stageScreen->Input(mouse->GetPos(), controlPanel->GetSelectedBlock(), currentLayer);
	controlPanel->Input(mouse->GetPos(), currentLayer);

	// ---- Ctrl + S で保存 ----
	const bool ctrl = CheckHitKey(KEY_INPUT_LCONTROL) || CheckHitKey(KEY_INPUT_RCONTROL);
	const bool saveKey = ctrl && CheckHitKey(KEY_INPUT_S);
	// 押した瞬間だけ保存する(押しっぱなしで毎フレーム保存しないように)
	if (saveKey && !prevSaveKey)
	{
		Save();
	}
	prevSaveKey = saveKey;

	// ---- 保存ボタンで保存 ----
	// controlPanel->Input(...) を呼んだあとに確かめる
	if (controlPanel->IsSaveClicked())
	{
		Save();
	}
}

SCENE_UPDATE(EditStage)
{
	// メッセージの残り時間を減らす
	if (messageTimer > 0)
	{
		messageTimer--;
	}

}

SCENE_DRAW(EditStage)
{
	stageScreen->Draw(currentLayer);
	controlPanel->Draw(currentLayer);

	// ---- 保存の結果のメッセージ ----
	// 保存ボタンのすぐ下に出す(パネルを描いたあとに描くので、上に重なる)
	if (messageTimer > 0)
	{
		DrawCenterString(STAGE_SCREEN_W + PANEL_THICKNESS / 2.0f,
			PanelLayout::SAVE_Y + PanelLayout::SAVE_H + 30.0f,
			saveMessage.c_str(), GetColor(255, 255, 0), 16.0f);
	}
}

SCENE_EXIT(EditStage)
{

}

void EditStage::Save()
{
	// 保存してよいかを確かめて、結果ごとにメッセージを決める
	switch (stage.CanSave())
	{
	case SaveResult::OK:
		// 確認が通ったときだけ書き出す。書き出し自体の失敗も伝える
		if (stage.OutPutCSV())
		{
			// CSV を書き出せたら、背景画像も用意する(なければ共通の背景をコピー)
			saveMessage = stage.PrepareStageBG(stageNum) ? "保存しました"
				: "保存しました(背景画像の用意に失敗)";
		}
		else
		{
			saveMessage = "保存に失敗しました";
		}
		break;

	case SaveResult::NoStart:
		saveMessage = "スタート地点がありません";
		break;

	case SaveResult::ManyStart:
		saveMessage = "スタート地点が複数あります";
		break;
	}

	// メッセージを 2 秒(60fps × 2)表示する
	messageTimer = 120;
}