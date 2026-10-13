#pragma once
#include "SceneBase.h"
#include "Player.h"
#include <memory>
#include "Stage.h"

class SceneInGame : public SceneBase
{
	std::unique_ptr<Player> player;

	// ステージ本体
	// Stage はコンストラクタに引数がないので、unique_ptr にせずそのまま持てる
	Stage stage;

	// どのステージを遊ぶか("Stage0" など)
	// コンストラクタで受け取り、Enter で Load に渡す
	std::string stageName;

public:

	// ステージ名を受け取るコンストラクタ(マクロとは別に宣言する)
	// explicit: 文字列から勝手に SceneInGame へ変換されるのを防ぐ
	explicit SceneInGame(const std::string& arg_stageName);

	SCENE_CLASS(SceneInGame);

};

