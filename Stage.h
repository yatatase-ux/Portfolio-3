#pragma once
#include <vector>
#include <string>
#include <fstream>   // std::ifstream(ファイル読み込み)
#include <sstream>   // std::stringstream(文字列の分割)

#include "FloatXY.h"
#include "IntXY.h"

// 各マスのサイズ。必ず正方形にする
const float CELL_SIZE = 80.0f;

enum class BlockType
{
	None = 0,		// 空白
	Ground = 1,		// 地面

	Max				// ブロックの種類の最大数
};

class Stage
{
	std::vector<std::vector<BlockType>> sideStage;
	std::vector<std::vector<BlockType>> topStage;

public:

	Stage();
	~Stage();

	/// <summary>
	/// ステージ情報を読み込む関数
	/// </summary>
	/// <param name="name">ステージ番号("Stage〇"の形で)</param>
	/// <returns>正しく読み込めたらTRUE、読み込みに失敗したらFALSE</returns>
	bool Load(const std::string& name);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 上下の移動後のマスが地形ブロックかを見る関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="boxSize">当たり判定のサイズ</param>
	/// <param name="yDiff">縦方向の移動差分</param>
	/// <returns>移動先が地形ならTRUE、それ以外ならFALSE</returns>
	bool HitVertical(FloatXY pos, FloatXY boxSize, float yDiff);
	/// <summary>
	/// 左右の移動後のマスが地形ブロックかをみる関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="boxSize">当たり判定のサイズ</param>
	/// <param name="xDiff">横方向の移動差分</param>
	/// <returns>移動先が地形ならTRUE、それ以外ならFALSE</returns>
	bool HitHorizontal(FloatXY pos, FloatXY boxSize, float xDiff);

	/// <summary>
	/// ステージのサイズを受け取る関数
	/// </summary>
	/// <returns>ステージサイズ</returns>
	FloatXY GetStageSize();

	/// <summary>
	/// 座標を基にマスの番号を得る関数
	/// </summary>
	/// <param name="pos"></param>
	/// <returns></returns>
	static int ToCellIndex(float pos);

private:
	/// <summary>
	/// 指定したマスのブロックが固い(通れない)ブロックかを見る関数
	/// </summary>
	/// <param name="row">縦</param>
	/// <param name="col">横</param>
	/// <returns></returns>
	bool IsSolid(int row, int col) const;
};