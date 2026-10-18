#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "Stage.h"

enum class pState
{
	Alive,
	Died
};

class Player
{
	KeyAction& key;
	Stage& stage;

	const FloatXY START_POS = { 120.0f, 640.0f };
	FloatXY pos;
	const FloatXY size = { 60.0f, 140.0f };
	const float JUMP_HEIGHT = -20.0f;
	const float GRAVITY_SPEED = 1.0f;

	FloatXY moveDiff;
	const float MOVE_SPEED = 5.0f;

	bool onGround;
	float velocityY;

	pState state;

	bool isOffScreen;

public:

	Player(KeyAction& arg_key, Stage& arg_stage);
	~Player();

	void Input();
	void Update();
	void Draw();

	FloatXY Move();
	FloatXY Gravity();

	pState GetState() { return state; };
};

