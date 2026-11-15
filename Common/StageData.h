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

	bool OpenFile(int arg_stageNum);
	int GetStageSize() const;
	BlockType GetBlockType(Layer layer, int x, int y) const;
	void SetBlockType(Layer layer, int x, int y, BlockType block);
	bool HasStart() const;
	SaveResult CanSave() const;
	bool OutPutCSV() const;

private:
	bool LoadCSV(const std::string& path, std::vector<std::vector<BlockType>>& out);
	void ExtendWidth(int newWidth);
	int CountStarts() const;
	bool SaveCSV(const std::string& path, const std::vector<std::vector<BlockType>>& data) const;
};