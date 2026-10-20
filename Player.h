#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "FloatXYZ.h"
#include "Stage.h"
#include "GravityDir.h"

enum class PlayerState
{
	Alive,
	Dead
};

class Player
{
	KeyAction& key;
	Stage& stage;

	GravityDir currentGravity;

	const FloatXYZ START_POS = { 120.0f, 570.0f , 100.0f};
	FloatXYZ pos;

	const FloatXY sideSize = { 60.0f, 140.0f };
	const FloatXY topSize = { 60.0f, 60.0f };
	const float HalfW = sideSize.x / 2.0f;

	const float JUMP_SPEED = -20.0f;
	const float GRAVITY = 1.0f;
	const float MAX_FALL_SPEED = 50.0f;

	FloatXYZ moveDiff;
	const float MOVE_SPEED = 5.0f;

	bool onGround;
	float velocityY;

	PlayerState state;
	const FloatXY stageSize;

	bool isOffScreen;

public:

	Player(KeyAction& arg_key, Stage& arg_stage);
	~Player();

	void GravityChange(GravityDir gravityDir);

	void Input();
	void Update();
	void Draw();
	PlayerState GetState() { return state; };

private:
	FloatXYZ Move();
	FloatXYZ Fall();

	void ResolveHorizontal(FloatXY size);
	void ResolveVertical(FloatXY size);
	void ResolveDepth(FloatXY size);
};

