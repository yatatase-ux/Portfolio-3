#pragma once
#include <vector>
#include <string>
#include "BlockType.h"
#include "LayerEnum.h"

enum class SaveResult
{
	OK,
	NoStart,
	ManyStart
};

class StageData
{
	int stageNum = -1;

	std::vector<std::vector<BlockType>> sideStage;
	std::vector<std::vector<BlockType>> topStage;
	
public:

	static int CountStages();

	/// <summary>
	/// ファイル読み込み
	/// </summary>
	/// <param name="arg_stageNum">ステージ番号</param>
	/// <returns>成功したらTRUE,失敗したらFALSE</returns>
	bool OpenFile(int arg_stageNum);

	bool CreateFromTemplate(int newNum);

	/// <summary>
	/// ステージサイズを得る関数
	/// </summary>
	/// <returns>ステージの横幅</returns>
	int GetStageSize() const;
	/// <summary>
	/// 指定したレイヤーの指定したマスのブロックが何かを得る関数
	/// </summary>
	/// <param name="layer">レイヤー</param>
	/// <param name="x">Xの配列番号</param>
	/// <param name="y">Yの配列番号</param>
	/// <returns>ブロックのタイプ</returns>
	BlockType GetBlockType(Layer layer, int x, int y) const;

	/// <summary>
	/// ステージの横幅を伸ばす
	/// </summary>
	/// <param name="newWidth">新しい幅</param>
	void ExtendWidth(int newWidth);
	/// <summary>
	/// ステージの横幅縮める
	/// </summary>
	/// <param name="newWidth">新しい横幅</param>
	void ShrinkWidth(int newWidth);
	/// <summary>
	/// 指定したレイヤーの指定したマスのブロックを変更する
	/// </summary>
	/// <param name="layer">レイヤー</param>
	/// <param name="x">Xの配列番号</param>
	/// <param name="y">Yの配列番号</param>
	/// <param name="block">変更後のブロック</param>
	void SetBlockType(Layer layer, int x, int y, BlockType block);

	/// <summary>
	/// スタートを持っているかをチェックする関数
	/// </summary>
	/// <returns>スタートが１つでもあればTRUE</returns>
	bool HasStart() const;

	/// <summary>
	/// 正しくセーブできる状況か
	/// </summary>
	/// <returns>セーブできるならOK, スタートが多ければManyStart, スタートがなければNoStart</returns>
	SaveResult CanSave() const;
	/// <summary>
	/// CSVに書き出す
	/// </summary>
	/// <returns>正しく書き出せたらTRUE</returns>
	bool OutPutCSV() const;

private:
	bool LoadCSV(const std::string& path, std::vector<std::vector<BlockType>>& out);
	int CountStarts() const;
	bool SaveCSV(const std::string& path, const std::vector<std::vector<BlockType>>& data) const;
	static std::string MakeBasePath(int num)
	{
		return "StageInfo/CSV/Stage" + std::to_string(num);
	}
};