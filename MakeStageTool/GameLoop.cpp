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

		// =======================================
		//  更新処理
		// =======================================

		// =======================================
		//  画面描画処理
		// =======================================
		for (int y = 0; y < 18; y++)
		{
			for (int x = 0; x < size; x++)
			{
				BlockType b = stage.GetBlockType(Layer::Side, x, y);
				DrawFormatString(x * 20, y * 20, GetColor(255, 255, 255), "%d, ", static_cast<int>(b));
			}
		}


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