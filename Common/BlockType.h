#pragma once
enum class BlockType
{
	BStart = -3,
	UStart = -2,
	DStart = -1,
	None = 0,		// 空白
	Ground = 1,		// 地面

	Max				// ブロックの種類の最大数
};