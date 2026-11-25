#include "StageData.h"
#include "DxLib.h" 
#include <fstream>   // std::ifstream(ファイル読み込み)
#include <sstream>   // std::stringstream(文字列の分割)
#include "StageConst.h"

int StageData::CountStages()
{
	int count = 0;

	while (true)
	{
		// 今の番号のファイル名を組み立てる(Load と同じルール)
		const std::string path = MakeBasePath(count) + "_side.csv";

		// 開けなければ、そこでステージは終わり
		std::ifstream file(path);
		if (!file.is_open()) break;

		count++;
	}

	return count;
}

bool StageData::OpenFile(int arg_stageNum)
{
	stageNum = arg_stageNum;

	// ファイル名の共通部分(例:"StageInfo/CSV/Stage0")
	const std::string base = MakeBasePath(stageNum);

	// 横用と見下ろし用を、同じ関数で読み込む
	return LoadCSV(base + "_side.csv", sideStage) &&
		   LoadCSV(base + "_top.csv", topStage);
}
bool StageData::CreateFromTemplate(int newNum)
{
	stageNum = newNum;

	return LoadCSV("StageInfo/CSV/Template_side.csv", sideStage) &&
		   LoadCSV("StageInfo/CSV/Template_top.csv", topStage);
}
bool StageData::LoadCSV(const std::string& path, std::vector<std::vector<BlockType>>& out)
{
	// ---- 2. ファイルを開く ----
	std::ifstream file(path);
	if (!file.is_open())
	{
		// パス間違い・ファイルがない・Excelで開いたまま、などで失敗する
		printfDx("横ステージ読み込み失敗: %s\n", path.c_str());
		return false;
	}

	// 前に読み込んだステージが残っていたら消しておく(読み直し対策)
	out.clear();

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
			if (number < -3 || number >= static_cast<int>(BlockType::Max))
			{
				number = static_cast<int>(BlockType::None);
			}

			// ---- 6. int → BlockType に変換して、この行に追加 ----
			row.push_back(static_cast<BlockType>(number));
		}

		// 1行分ができたので、ステージに追加
		out.push_back(row);
	}

	return true;
}

int StageData::GetStageSize() const
{
	// ステージが読み込まれていないときは、大きさ 0 として返す
	// (sideStage[0] を読むと範囲外になるため)
	if (sideStage.empty())
	{
		return 0;
	}

	return sideStage[0].size();
}

BlockType StageData::GetBlockType(Layer layer, int x, int y) const
{
	const auto& stage = (layer == Layer::Side) ? sideStage : topStage;

	return stage[y][x];
}

// ステージの横幅を newWidth まで伸ばす(今より短い値なら何もしない)
// 横用と見下ろし用を必ず一緒に伸ばして、2つの幅をそろえておく
void StageData::ExtendWidth(int newWidth)
{
	// 横用:伸ばした部分は空白
	for (auto& row : sideStage)
	{
		if (static_cast<int>(row.size()) < newWidth)
		{
			row.resize(newWidth, BlockType::None);
		}
	}

	// 見下ろし用:伸ばした部分は床(穴にならないように)
	for (auto& row : topStage)
	{
		if (static_cast<int>(row.size()) < newWidth)
		{
			row.resize(newWidth, BlockType::Ground);
		}
	}
}

// ステージの横幅を newWidth まで縮める(今より長い値なら何もしない)
// 横用と見下ろし用を必ず一緒に縮めて、2つの幅をそろえておく
void StageData::ShrinkWidth(int newWidth)
{
	// 0 以下に縮めると、ステージがなくなってしまうので止める
	if (newWidth < 31) return;

	// 横用:newWidth より長い行は、後ろを削る
	for (auto& row : sideStage)
	{
		if (static_cast<int>(row.size()) > newWidth)
		{
			// 短くするときは、後ろが削られるだけなので、埋める値は要らない
			row.resize(newWidth);
		}
	}

	// 見下ろし用:同じく、後ろを削る
	for (auto& row : topStage)
	{
		if (static_cast<int>(row.size()) > newWidth)
		{
			row.resize(newWidth);
		}
	}
}

void StageData::SetBlockType(Layer layer, int x, int y, BlockType block)
{
	const int width = GetStageSize();

	// 上下の外と、左の外には置かない
	if (y < 0 || y >= STAGE_HEIGHT) return;
	if (x < 0) return;

	// ---- ゴールの列(ステージのすぐ右)に置いたら、1列だけ伸ばす ----
	// それより右は、ゴールより先なので置かない(一気に何列も伸びないようにする)
	if (x == width)
	{
		ExtendWidth(width + 1);
	}
	else if (x > width)
	{
		return;
	}

	auto& stage = (layer == Layer::Side) ? sideStage : topStage;
	stage[y][x] = block;
}

int StageData::CountStarts() const
{ 
	/* 今の CanSave の数える部分 */ 
	int start = 0;
	// すべての行の、すべてのマスを見る(添字を使わないので、書き間違いが起きない)
	for (const auto& row : sideStage)
	{
		for (BlockType block : row)
		{
			// スタート地点はマイナスの値(-1〜-3)
			if (static_cast<int>(block) < 0)
			{
				start++;
			}
		}
	}

	return start;
}

bool StageData::HasStart() const
{
	const int start = CountStarts();
	return (start > 0);
}

SaveResult StageData::CanSave() const
{	
	const int start = CountStarts();
	if (start == 0) return SaveResult::NoStart;
	if (start > 1) return SaveResult::ManyStart;
	return SaveResult::OK;
}
// 今開いているステージを、横用と見下ろし用の CSV に書き出す
bool StageData::OutPutCSV() const
{
	// ファイル名のルールは、OpenFile と同じ(例:"StageInfo/CSV/Stage0")
	const std::string base = MakeBasePath(stageNum);

	return SaveCSV(base + "_side.csv", sideStage) &&
		SaveCSV(base + "_top.csv", topStage);
}
bool StageData::SaveCSV(const std::string& path,
	const std::vector<std::vector<BlockType>>& data) const
{
	// ---- 1. 書き込み用にファイルを開く ----
	// すでにファイルがあれば、中身を消して最初から書き直す
	std::ofstream file(path);
	if (!file.is_open())
	{
		// フォルダがない、ファイルが Excel などで開かれている、などで失敗する
		printfDx("ステージ書き出し失敗: %s\n", path.c_str());
		return false;
	}

	// ---- 2. 1行ずつ書く ----
	for (const auto& row : data)
	{
		for (int x = 0; x < static_cast<int>(row.size()); x++)
		{
			// 2マス目からは、前に「,」を付けて区切る
			// (行の最後に余計な「,」が付かないようにするため)
			if (x > 0)
			{
				file << ",";
			}

			// BlockType を数字に直して書く(読み込みのときの逆)
			file << static_cast<int>(row[x]);
		}

		// ---- 3. 行の終わりで改行 ----
		file << "\n";
	}

	return true; // 書き出し成功
}