#include "Stage.h"
#include <fstream>   // std::ifstream(ファイル読み込み)
#include <sstream>   // std::stringstream(文字列の分割)
#include <cmath>
#include "DxLib.h"   // printfDx(デバッグ表示)
#include "Shape.h"

#include "Camera.h"

Stage::Stage()
{
	startGravity = GravityDir::Down;
	currentGravity = GravityDir::Down;
	goalPos = { 0.0f, 0.0f };
	startIndex = { -1, -1 };
	hasStart = false;
	BG = -1;
}

Stage::~Stage()
{

}

bool Stage::Load(const std::string& name)
{
	// 両方読み込めたときだけ、ゴールと余白を付け足す
	if (!SideLoad(name) || !TopLoad(name) || !BGLoad(name))
	{
		return false;
	}

	CreateGoal();
	return true;
}

FloatXYZ Stage::GetStartPos() const
{
	FloatXYZ pos;
	pos.x = CELL_SIZE * startIndex.x + CELL_SIZE / 2.0f;
	float yDir = (startGravity == GravityDir::Down) ? CELL_SIZE :
				 (startGravity == GravityDir::Up)   ? 0.0f : 
													  CELL_SIZE / 2.0f;
	pos.y = CELL_SIZE * startIndex.y + yDir;
	pos.z = (startGravity == GravityDir::Back) ? 0.0f : 200.0f;

	return pos;
}

GravityDir Stage::GetStartGravity() const
{
	return startGravity;
}

bool Stage::SideLoad(const std::string& name)
{
	startIndex = { -1, -1 };
	hasStart = false;

	// ---- 1. ファイルパスを組み立てる ----
	// 例: name が "Stage0" なら "StageInfoCSV/Stage0_side.csv" になる
	const std::string path = "StageInfo/CSV/" + name + "_side.csv";

	// ---- 2. ファイルを開く ----
	std::ifstream file(path);
	if (!file.is_open())
	{
		// パス間違い・ファイルがない・Excelで開いたまま、などで失敗する
		printfDx("横ステージ読み込み失敗: %s\n", path.c_str());
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

			// ---- スタート地点(マイナスの値)を見つけたら ----
			if (number < 0)
			{
				// このマスの番号(push_back する前に数えるのがポイント)
				// 行:これまでに追加し終わった行の数
				// 列:この行に、これまでに追加したマスの数
				startIndex = { static_cast<int>(row.size()),        // x = 列
							   static_cast<int>(sideStage.size()) }; // y = 行

				// (ここで startGravity を -1～-3 から決める)
				// -1～-3 を重力の向きに対応させる
				// (enum の値の並びに頼らないよう、static_cast ではなく1つずつ対応させる)
				switch (number)
				{
				case -1: startGravity = GravityDir::Down; hasStart = true; break;
				case -2: startGravity = GravityDir::Up;   hasStart = true; break;
				case -3: startGravity = GravityDir::Back; hasStart = true; break;
				default:
					// 決めていないマイナス(-4 以下)は、スタート地点として扱わない
					printfDx("不明なスタート番号: %d\n", number);
					break;
				}

				// スタート地点のマスは、地形としては空白にする
				number = static_cast<int>(BlockType::None);
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

bool Stage::TopLoad(const std::string& name)
{
	// ---- 1. ファイルパスを組み立てる ----
	// 例: name が "Stage0" なら "StageInfoCSV/Stage0_top.csv" になる
	const std::string path = "StageInfo/CSV/" + name + "_top.csv";

	// ---- 2. ファイルを開く ----
	std::ifstream file(path);
	if (!file.is_open())
	{
		// パス間違い・ファイルがない・Excelで開いたまま、などで失敗する
		printfDx("縦ステージ読み込み失敗: %s\n", path.c_str());
		return false;
	}

	// 前に読み込んだステージが残っていたら消しておく(読み直し対策)
	topStage.clear();

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
		topStage.push_back(row);
	}

	return true; // 読み込み成功
}

bool Stage::BGLoad(const std::string& name)
{
	// ---- 1. ファイルパスを組み立てる ----
	const std::string path = "Data/image/BG/" + name + "_BG.jpg";

	// ---- 2. ファイルを開く ----
	// 画像を読み込む(失敗すると -1 が返る)
	BG = LoadGraph(path.c_str());
	if (BG == -1)
	{
		printfDx("背景画像読み込み失敗: %s\n", path.c_str());
		return false;
	}
	return true;
}

void Stage::CreateGoal()
{
	// ---- 1. ゴール線の位置(本編の右端 = 本編と余白の境目) ----
	// 付け足す前に計算しておく(付け足すとステージの幅が変わるため)
	goalPos.x = GetStageSize().x;

	const int rows = static_cast<int>(sideStage.size()); // 縦のマス数

	// ---- 2. 横用:上下の地面だけ、間は空白 ----
	for (int r = 0; r < rows; r++)
	{
		// 上から GOAL_GROUND_ROWS 行、または下から GOAL_GROUND_ROWS 行なら地面
		const bool isGroundRow = (r < GOAL_GROUND_ROWS) || (r >= rows - GOAL_GROUND_ROWS);
		const BlockType type = isGroundRow ? BlockType::Ground : BlockType::None;

		// この行の末尾に、余白の幅(GOAL_MARGIN_COLS)の分だけ足す
		sideStage[r].insert(sideStage[r].end(), GOAL_MARGIN_COLS, type);
	}

	// ---- 3. 見下ろし用:全部床 ----
	// auto& の「&」を忘れると、行のコピーに足すだけになり、元の配列は変わらない
	for (auto& row : topStage)
	{
		row.insert(row.end(), GOAL_MARGIN_COLS, BlockType::Ground);
	}
}

void Stage::SetGravityDir(GravityDir g)
{
	currentGravity = g;
}

void Stage::DrawLayer(const std::vector<std::vector<BlockType>>& layer,
	BlockType type, unsigned int color, int alpha, const Camera& camera) const
{
	// マスの大きさは全マス共通なので、ループの外で1回だけ作る
	const FloatXY size = { CELL_SIZE, CELL_SIZE };

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
	for (size_t h = 0; h < layer.size(); h++)
	{
		for (size_t w = 0; w < layer[h].size(); w++)
		{
			// 描きたい種類以外は飛ばす
			if (layer[h][w] != type) continue;

			// マスの中心座標
			const FloatXY worldPos = { CELL_SIZE * w + CELL_SIZE / 2.0f,
								  CELL_SIZE * h + CELL_SIZE / 2.0f };

			FloatXY screenPos = camera.WorldToScreen( worldPos );

			DrawCenterBox(screenPos, size, color, TRUE);
		}
	}
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void Stage::Draw(const Camera& camera)
{
	DrawExtendGraphF(0.0f, 0.0f, SCREEN_W, SCREEN_H, BG, TRUE);

	int alpha = (currentGravity == GravityDir::Back) ? 0 : 128;
	// 下から順に重ねる(あとに描いたものが上に来る)
//	DrawLayer(topStage,  BlockType::None,   GetColor(185, 255, 255), 255 - alpha, camera);	// 1. 見下ろしの穴
//	DrawLayer(sideStage, BlockType::None,   GetColor(185, 255, 255), 128 + alpha, camera);	// 2. 横の空白
	DrawLayer(topStage,  BlockType::Ground, GetColor(  0,   0,   0), 255 - alpha, camera);	// 3. 見下ろしの床
	
	// ゴールライン
	FloatXY g_sPos = camera.WorldToScreen({ goalPos.x, 0.0f });
	FloatXY g_ePos = camera.WorldToScreen({ goalPos.x + CELL_SIZE / 2.0f, GetStageSize().y });
	DrawCheckered(g_sPos, g_ePos, GetColor(255, 255, 255), GetColor(0, 0, 0), CELL_SIZE / 4.0f);

	DrawLayer(sideStage, BlockType::Ground, GetColor(165,  42,  42), 128 + alpha, camera);	// 4. 横の地面


}

bool Stage::HitVertical(FloatXYZ pos, FloatXY boxSize, float yDiff)
{
	bool dir = (yDiff < 0.0f);	// Y変化量がマイナス（上向き）ならTRUE

	float left = pos.x - boxSize.x / 2.0f;
	float right = pos.x + boxSize.x / 2.0f;
	// 上端は「境目ちょうど」だと下の行に逃げるので、少し引いて接しているマスを拾う
	// 下端はそのままで、接している床のマスに入る
	float y = (dir) ? pos.y - boxSize.y / 2.0f - 0.01f : pos.y + boxSize.y / 2.0f;

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

bool Stage::HitHorizontal(FloatXYZ pos, FloatXY boxSize, float xDiff)
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

// 足跡(x と y の四角形)の四隅のどれか1つでも床の上なら true
// 横視点の「左右どちらかの足が乗っていれば着地」と同じ考え方
bool Stage::OnFloor(FloatXY pos, FloatXY boxSize) const
{
	// 右端と下端は、境目ちょうどなら内側のマスとして扱う(少し引く)
	const int left = ToCellIndex(pos.x - boxSize.x / 2.0f);
	const int right = ToCellIndex(pos.x + boxSize.x / 2.0f - 0.01f);
	const int top = ToCellIndex(pos.y - boxSize.y / 2.0f);
	const int bottom = ToCellIndex(pos.y + boxSize.y / 2.0f - 0.01f);

	return IsFloor(top, left) || IsFloor(top, right) ||
		IsFloor(bottom, left) || IsFloor(bottom, right);
}

bool Stage::ArriveGoal(float wPos_x) const
{
	return (wPos_x > goalPos.x);
}

FloatXY Stage::GetStageSize() const
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

bool Stage::IsFloor(int row, int col) const
{
	if (row < 0 || row >= (int)topStage.size()) return false;
	if (col < 0 || topStage.empty() || col >= (int)topStage[0].size()) return false;
	return topStage[row][col] == BlockType::Ground;
}