#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "Stage.h"

class Player
{
	KeyAction& key;
	Stage& stage;

	const FloatXY size = { 60.0f, 140.0f };

	FloatXY pos;
	FloatXY moveDiff;
	const float MOVE_SPEED = 5.0f;

	bool onGround;
	float velocityY;

public:

	Player(KeyAction& arg_key, Stage& arg_stage);
	~Player();

	void Input();
	void Update();
	void Draw();

	FloatXY Move();
	FloatXY Gravity();
};

