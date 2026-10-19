#pragma once
#include "Player.h"
#include "Stage.h"
#include <memory>
#include <string>
#include "KeyAction.h"

enum class GravityDir
{
	Up   = -1, // ‰æ–Ê‚Ìã‚Ö(ã‰º‹t‚³)
	Back =  0, // ‰æ–Ê‚Ì‰œ‚Ö(Œ©‰º‚ë‚µ)
	Down =  1, // ‰æ–Ê‚Ì‰º‚Ö(’Êí)
};

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

