#pragma once
#include "Player.h"
#include "Stage.h"
#include <memory>
#include <string>
#include "KeyAction.h"
#include "GravityDir.h"


class GravityManager
{
	GravityDir gravity;

	Stage stage;
	std::unique_ptr<Player> player;
	KeyAction& key;

public:

	GravityManager(const std::string stageNum, KeyAction& arg_key);
	~GravityManager();

	void Input();
	PlayerState Update();
	void Draw();


};

