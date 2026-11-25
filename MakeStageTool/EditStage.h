#pragma once
#include "SceneBase.h"
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"
#include "StageScreen.h"
#include "ControlPanel.h"

class EditStage : public SceneBase
{
	StageData stage;
	bool isNewData;
	int stageNum;

	std::unique_ptr<StageScreen> stageScreen;
	std::unique_ptr<ControlPanel> controlPanel;

	Layer currentLayer;

	std::string saveMessage;   // 画面に出す、保存の結果
	int  messageTimer;     // メッセージを出しておく残りフレーム数
	bool prevSaveKey;  // 前のフレームで Ctrl+S が押されていたか


public:

	EditStage(int s, bool isN);

	SCENE_CLASS(EditStage);

private:

	// 保存してよいか確かめて、書き出す(ボタンからもキーからも、ここを通る)
	void Save();

};

