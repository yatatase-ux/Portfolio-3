#pragma once
#include "SceneBase.h"
#include <memory>
#include "GravityManager.h"
#include "Player.h"
#include "Stage.h"
#include "Camera.h"

enum class InGameState
{
	Playing,    // 遊んでいる最中(操作できる)
	Clearing,   // クリアの演出中(自動で歩く、エフェクト、ズーム)
	Missing,    // ミスの演出中(あとで作るなら)
};

class SceneInGame : public SceneBase
{
	// どのステージを遊ぶか("Stage0" など)
	// コンストラクタで受け取り、Enter で Load に渡す
	std::string stageName;

	std::unique_ptr<Stage> stage;
	std::unique_ptr<GravityManager> mGravity;
	std::unique_ptr<Camera> camera;
	std::unique_ptr<Player> player;

	InGameState state;

public:

	// ステージ名を受け取るコンストラクタ(マクロとは別に宣言する)
	// explicit: 文字列から勝手に SceneInGame へ変換されるのを防ぐ
	explicit SceneInGame(const std::string& arg_stageName);

	SCENE_CLASS(SceneInGame);

};

