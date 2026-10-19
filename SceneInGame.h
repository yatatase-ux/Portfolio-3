#pragma once
#include "SceneBase.h"
#include <memory>
#include "GravityManager.h"

class SceneInGame : public SceneBase
{
	// どのステージを遊ぶか("Stage0" など)
	// コンストラクタで受け取り、Enter で Load に渡す
	std::string stageName;

	std::unique_ptr<GravityManager> mGravity;

public:

	// ステージ名を受け取るコンストラクタ(マクロとは別に宣言する)
	// explicit: 文字列から勝手に SceneInGame へ変換されるのを防ぐ
	explicit SceneInGame(const std::string& arg_stageName);

	SCENE_CLASS(SceneInGame);

};

