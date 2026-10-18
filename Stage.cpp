#include "Stage.h"
#include <fstream>   // std::ifstream(ファイル読み込み)
#include <sstream>   // std::stringstream(文字列の分割)
#include <cmath>
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
	const std::string path = "StageInfo/CSV/" + name + "_side.csv";

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
			FloatXY pos = { CELL_SIZE * w + CELL_SIZE / 2.0f, CELL_SIZE * h + CELL_SIZE / 2.0f };
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

bool Stage::HitVertical(FloatXY pos, FloatXY boxSize, float yDiff)
{
	bool dir = (yDiff < 0.0f);	// Y変化量がマイナス（上向き）ならTRUE

	float left = pos.x - boxSize.x / 2.0f;
	float right = pos.x + boxSize.x / 2.0f;
	float y = (dir) ? pos.y - boxSize.y / 2.0f : pos.y + boxSize.y / 2.0f;

	int indexY = ToCellIndex(y);
	int indexX_L = ToCellIndex(left);
	// 右端と下端は「境目ちょうど」なら、内側のマスとして扱う
	// ほんの少し(0.01)引いてからマスの番号に変える
	int indexX_R = ToCellIndex(right - 0.01f);

	if (IsSolid(indexY, indexX_L) || IsSolid(indexY, indexX_R))
	{
		return true;
	}

	return false;
}

bool Stage::HitHorizontal(FloatXY pos, FloatXY boxSize, float xDiff)
{
	if (xDiff == 0.0f) return false;

	bool dir = (xDiff > 0.0f);	// 右向きならTRUE、左向きならFALSE

	float top = pos.y - boxSize.y / 2.0f;
	float bottom = pos.y + boxSize.y / 2.0f;
	float side = (dir) ?  pos.x - 0.01f + boxSize.x / 2.0f : pos.x - boxSize.x / 2.0f;

	int indexTop = ToCellIndex(top);
	int indexBottom = ToCellIndex(bottom - 0.01f);
	int indexSide = ToCellIndex(side);

	for(int i = indexTop; i <= indexBottom; i++)
	{
		if (IsSolid(i, indexSide))
		{
			return true;
		}
	}

	return false;
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

int Stage::ToCellIndex(float pos)
{
	return static_cast<int>(std::floor(pos / CELL_SIZE));
}

// 指定した行・列のマスが「固い」かどうかを返す
// 範囲外は決めておいたルールで答える
bool Stage::IsSolid(int row, int col) const
{
	// 左右の外:見えない壁なので固い
	if (col < 0 || sideStage.empty() || col >= (int)sideStage[0].size())
	{
		return true;
	}

	// 上下の外:穴(何もない)なので固くない
	if (row < 0 || row >= (int)sideStage.size())
	{
		return false;
	}

	// 範囲内なら、マスの種類で判断する
	return sideStage[row][col] == BlockType::Ground;
}