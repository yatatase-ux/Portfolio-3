#pragma once
#include <vector>
#include <string>

enum class BlockType
{
	None = 0,		// ‹ó”’
	Ground = 1		// ’n–Ê
};

class Stage
{

	std::vector<std::vector<BlockType>> sideStage;

public:

	Stage();
	~Stage();

	bool Load(const std::string& name);

	void Draw();
};