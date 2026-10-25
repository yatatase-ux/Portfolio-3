#pragma once
#include <vector>
#include <string>

#include "FloatXY.h"
#include "FloatXYZ.h"
#include "IntXY.h"
#include "GravityDir.h"

class Camera;

// 各マスのサイズ。必ず正方形にする
const float CELL_SIZE = 40.0f;

enum class BlockType
{
	None = 0,		// 空白
	Ground = 1,		// 地面

	Max				// ブロックの種類の最大数
};

class Stage
{
	const int GOAL_GROUND_ROWS = 1;
	const int GOAL_MARGIN_COLS = 10;

	std::vector<std::vector<BlockType>> sideStage;
	std::vector<std::vector<BlockType>> topStage;

	GravityDir gravityDir;

	FloatXY goalPos;

public:

	Stage();
	~Stage();

	/// <summary>
	/// ステージ情報を読み込む関数
	/// </summary>
	/// <param name="name">ステージ番号("Stage〇"の形で)</param>
	/// <returns>正しく読み込めたらTRUE、読み込みに失敗したらFALSE</returns>
	bool Load(const std::string& name);

	void SetGravityDir(GravityDir g);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

	/// <summary>
	/// 左右の移動後のマスが地形ブロックかをみる関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="boxSize">当たり判定のサイズ</param>
	/// <param name="xDiff">横方向の移動差分</param>
	/// <returns>移動先が地形ならTRUE、それ以外ならFALSE</returns>
	bool HitHorizontal(FloatXYZ pos, FloatXY boxSize, float xDiff);
	/// <summary>
	/// 上下の移動後のマスが地形ブロックかを見る関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="boxSize">当たり判定のサイズ</param>
	/// <param name="yDiff">縦方向の移動差分</param>
	/// <returns>移動先が地形ならTRUE、それ以外ならFALSE</returns>
	bool HitVertical(FloatXYZ pos, FloatXY boxSize, float yDiff);
	/// <summary>
	/// 奥の移動後のマスが地形ブロックかをみる関数
	/// </summary>
	/// <param name="pos">座標</param>
	/// <param name="boxSize">当たり判定のサイズ</param>
	/// <param name="xDiff">横方向の移動差分</param>
	/// <returns>移動先が地形ならTRUE、それ以外ならFALSE</returns>
	bool OnFloor(FloatXY pos, FloatXY boxSize) const;

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

	bool SideLoad(const std::string& name);
	bool TopLoad(const std::string& name);
	void CreateGoal();

	/// <summary>
	/// 指定したマスのブロックが固い(通れない)ブロックかを見る関数
	/// </summary>
	/// <param name="row">縦</param>
	/// <param name="col">横</param>
	/// <returns></returns>
	bool IsSolid(int row, int col) const;

	bool IsFloor(int row, int col) const;

	void DrawLayer(const std::vector<std::vector<BlockType>>& layer,
		BlockType type, unsigned int color, int alpha, const Camera& camera) const;
};