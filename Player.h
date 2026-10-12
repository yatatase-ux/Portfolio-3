#pragma once
#include "KeyAction.h"
#include "FloatXY.h"

class Player
{
	KeyAction& key;

	FloatXY pos = {50.0f, 200.0f};

public:

	Player(KeyAction& arg_key);
	~Player();

	void Input();
	void Update();
	void Draw();
};

