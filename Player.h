#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "Stage.h"

enum class PlayerState
{
	Alive,
	Died
};

class Player
{
	KeyAction& key;
	Stage& stage;

	float gravityDiff;

	const FloatXY START_POS = { 120.0f, 570.0f };
	FloatXY pos;
	const FloatXY size = { 60.0f, 140.0f };
	const float HalfW = size.x / 2.0f;
	const float JUMP_HEIGHT = -20.0f;
	const float GRAVITY_SPEED = 1.0f;
	const float MAX_GRAVITY = 50.0f;

	FloatXY moveDiff;
	const float MOVE_SPEED = 5.0f;

	bool onGround;
	float velocityY;

	PlayerState state;
	const FloatXY stageSize;

	bool isOffScreen;

public:

	Player(KeyAction& arg_key, Stage& arg_stage);
	~Player();

	void GravityChange(int gravityDir);

	void Input();
	void Update();
	void Draw();

	FloatXY Move();
	FloatXY Gravity();

	void ResolveHorizontal();
	void ResolveVertical();

	PlayerState GetState() { return state; };
};

