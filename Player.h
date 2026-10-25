#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "FloatXYZ.h"
#include "Stage.h"
#include "GravityDir.h"
#include "ScreenSize.h"
#include "PlayerState.h"

class Camera;

class Player
{
	KeyAction& key;
	Stage& stage;

	GravityDir currentGravity;

	const FloatXYZ START_POS = { 120.0f, 570.0f , 100.0f};
	FloatXYZ worldPos;

	const FloatXY sideSize = { 30.0f, 70.0f };
	const FloatXY topSize = { 30.0f, 30.0f };
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
	void Draw(const Camera& camera);
	PlayerState GetState() { return state; };
	FloatXYZ GetPos() { return worldPos; };

private:
	FloatXYZ Move();
	FloatXYZ Fall();

	void ResolveHorizontal(FloatXY size);
	void ResolveVertical(FloatXY size);
	void ResolveDepth(FloatXY size);
	void CheckOutOfStage(FloatXY size);
};

