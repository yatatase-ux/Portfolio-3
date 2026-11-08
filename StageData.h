#pragma once
#include <vector>
#include <string>
#include "BlockType.h"

class StageData
{
	int stageNum;

	std::vector<std::vector<BlockType>> sideStage;
	std::vector<std::vector<BlockType>> topStage;

public:

	bool OpenFile(int arg_stageNum);
	int GetStageSize() const;
	BlockType GetBlockType(int layer, int x, int y) const;
	void SetBlockType(int layer, int x, int y, BlockType block);
	void OutPutCSV();
};