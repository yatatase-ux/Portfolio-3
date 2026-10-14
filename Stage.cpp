#include "Stage.h"
#include <fstream>   // std::ifstream(ファイル読み込み)
#include <sstream>   // std::stringstream(文字列の分割)
#include "DxLib.h"   // printfDx(デバッグ表示)
#include "Shape.h"

Stage::Stage()
{

}

Stage::~Stage()
{

}

bool Stage::Load(const std::string& name)
{
	// ---- 1. ファイルパスを組み立てる ----
	// 例: name が "Stage0" なら "StageInfoCSV/Stage0_side.csv" になる
	const std::string path = "StageInfoCSV/" + name + "_side.csv";

	// ---- 2. ファイルを開く ----
	std::ifstream file(path);
	if (!file.is_open())
	{
		// パス間違い・ファイルがない・Excelで開いたまま、などで失敗する
		printfDx("ステージ読み込み失敗: %s\n", path.c_str());
		return false;
	}

	// 前に読み込んだステージが残っていたら消しておく(読み直し対策)
	sideStage.clear();

	// ---- 3. 1行ずつ読む ----
	std::string line;
	while (std::getline(file, line))
	{
		// この行のマス(左から右へ)を入れる配列
		std::vector<BlockType> row;

		// 行の文字列を、カンマ区切りで取り出せるようにする
		std::stringstream ss(line);
		std::string cell;

		// ---- 4. カンマで区切って1マスずつ取り出す ----
		while (std::getline(ss, cell, ','))
		{
			int number = 0; // 空セルは 0(空白)として扱う

			if (!cell.empty())
			{
				// 文字列 → 数字に変換
				// 数字以外(全角文字など)が入っていると例外が出るので try で囲む
				try
				{
					number = std::stoi(cell);
				}
				catch (...)
				{
					number = 0; // 読めないセルは空白扱い
				}
			}

			// ---- 5. 定義されていない番号を弾く ----
			// Max は「種類の数」を表す目印
			if (number < 0 || number >= static_cast<int>(BlockType::Max))
			{
				number = static_cast<int>(BlockType::None);
			}

			// ---- 6. int → BlockType に変換して、この行に追加 ----
			row.push_back(static_cast<BlockType>(number));
		}

		// 1行分ができたので、ステージに追加
		sideStage.push_back(row);
	}

	return true; // 読み込み成功
}

void Stage::Draw()
{
	for (int h = 0; h < sideStage.size(); h++)
	{
		for (int w = 0; w < sideStage[h].size(); w++)
		{
			FloatXY pos = { CELL_SIZE * w + 40.0f, CELL_SIZE * h + 40.0f };
			FloatXY size = { CELL_SIZE, CELL_SIZE };
			switch (sideStage[h][w])
			{
			case BlockType::None:
				DrawCenterBox(pos, size, GetColor(185, 255, 255), TRUE);
				break;
			case BlockType::Ground:
				DrawCenterBox(pos, size, GetColor(165, 42, 42), TRUE);
				break;
			}
		}
	}
}

bool Stage::IsGround(FloatXY pos, FloatXY boxSize)
{
	float left = pos.x - boxSize.x / 2.0f;
	float right = pos.x + boxSize.x / 2.0f;
	float bottom = pos.y + boxSize.y / 2.0f;

	int indexY = (int)bottom / 80;
	int indexX_L = (int)left / 80;
	int indexX_R = (int)right / 80;

	if (sideStage[indexY][indexX_L] == BlockType::Ground ||
		sideStage[indexY][indexX_R] == BlockType::Ground)
	{
		return true;
	}

	return false;
}

bool Stage::HitBlock(FloatXY pos, FloatXY boxSize)
{
	float left = pos.x - boxSize.x / 2.0f;
	float right = pos.x + boxSize.x / 2.0f;
	float top = pos.y - boxSize.y / 2.0f;
	float bottom = pos.y + boxSize.y / 2.0f;

	int indexTop = (int)top / 80;
	int indexBottom = (int)bottom / 80;
	int indexLeft = (int)left / 80;
	int indexRight = (int)right / 80;

	if (sideStage[indexTop][indexLeft] == BlockType::Ground ||
		sideStage[indexTop][indexRight] == BlockType::Ground ||
		sideStage[indexBottom][indexLeft] == BlockType::Ground ||
		sideStage[indexBottom][indexRight] == BlockType::Ground)
	{

	}
}

FloatXY Stage::GetStageSize()
{
	// ステージが読み込まれていないときは、大きさ 0 として返す
	// (sideStage[0] を読むと範囲外になるため)
	if (sideStage.empty())
	{
		return { 0.0f, 0.0f };
	}

	// 行の数 = 縦のマス数、先頭の行の長さ = 横のマス数
	// 0 埋めしてあるので、どの行も同じ長さになっている前提
	const size_t rows = sideStage.size();
	const size_t cols = sideStage[0].size();

	// 個数 × マスの大きさ = 端の座標
	return { cols * CELL_SIZE, rows * CELL_SIZE };
}