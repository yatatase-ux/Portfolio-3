#pragma once
#include <memory>
#include "BlockType.h"
#include "StageConst.h"
#include "StageData.h"
#include "StageScreen.h"
#include "ControlPanel.h"
#include <string>

class GameLoop
{
	StageData stage;
	std::unique_ptr<StageScreen> stageScreen;
	std::unique_ptr<ControlPanel> controlPanel;

	Layer currentLayer;

	std::string saveMessage;   // 画面に出す、保存の結果
	int  messageTimer = 0;     // メッセージを出しておく残りフレーム数
	bool prevSaveKey = false;  // 前のフレームで Ctrl+S が押されていたか

public:
	GameLoop();

	void Loop();

private:
	// 保存してよいか確かめて、書き出す(ボタンからもキーからも、ここを通る)
	void Save();

};