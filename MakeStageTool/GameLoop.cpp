#include "GameLoop.h"
#include <ctime>	
#include "DxLib.h"
#include <iostream>
GameLoop::GameLoop()
{
	
}

void GameLoop::Loop()
{
	// =======================================
	//  初期化処理
	// =======================================
	stage.OpenFile(0);
	int size = stage.GetStageSize();
	sd = std::make_unique<StageScreen>(stage);
	currentLayer = Layer::Side;

	// ------------------------------
	//  ゲームループ
	// ------------------------------
	while (ProcessMessage() == 0)
	{
		// リフレッシュレートを設定するための処理
		clock_t check_fps = clock() + CLOCKS_PER_SEC / 60;

		// マウスカーソル表示設定
		SetMouseDispFlag(TRUE);

		// 画面上の描画を初期化（画面を消去）
		ClearDrawScreen();

		// =======================================
		//  入力処理
		// =======================================
		if (CheckHitKey(KEY_INPUT_3)) currentLayer = Layer::Top;
		if (CheckHitKey(KEY_INPUT_1)) currentLayer = Layer::Side;
		
		// =======================================
		//  更新処理
		// =======================================
		int mouseX;
		int mouseY;
		GetMousePoint(&mouseX, &mouseY);
		sd->Input(mouseX, mouseY);

		// =======================================
		//  画面描画処理
		// =======================================
		sd->Draw(currentLayer);
		td.Draw();

		// =======================================
		//  音声再生処理
		// =======================================

		// リフレッシュレートが一定になるまで待つ処理
		while (clock() < check_fps) {}

		// 裏画面の描画を表に反映
		ScreenFlip();

		// ESCキーでループから抜ける
		if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			break;
		}

	}
}