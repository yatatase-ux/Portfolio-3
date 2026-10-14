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

public:

	Stage();
	~Stage();

	bool Load(const std::string& name);

	void Draw();

	bool IsGround(FloatXY pos, FloatXY boxSize);
	bool HitBlock(FloatXY pos, FloatXY boxSize);

	FloatXY GetStageSize();
};