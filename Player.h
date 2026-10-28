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
	KeyAction& key;				// キー入力の参照	
	Stage& stage;				// ステージの参照

	GravityDir currentGravity;	// 現在の重力状態

	const FloatXYZ START_POS = { 120.0f, 570.0f , 200.0f};	// 仮のスタート座標
	FloatXYZ worldPos;	// ワールド座標

	const FloatXY sideSize = { 30.0f, 60.0f };		// 上下重力時のプレイヤーのサイズ
	const FloatXY crouchSize = { 30.0f, 30.0f };	// しゃがみ状態のプレイヤーのサイズ
	const FloatXY topSize = { 30.0f, 30.0f };		// 壁重力時のプレイヤーのサイズ
	const float HalfW = sideSize.x / 2.0f;			// プレイヤーの横幅の半分

	const float JUMP_SPEED = -20.0f;	// ジャンプのスピード
	const float GRAVITY = 1.0f;			// 重力のかかり方
	const float MAX_FALL_SPEED = 50.0f;	// 落下スピードの上限

	FloatXYZ moveDiff;				// 移動分の合計
	const float MOVE_SPEED = 5.0f;	// プレイヤーの移動スピード

	bool onGround;		// 地面に乗っているかどうか
	bool isCrouched;	// しゃがみ状態
	float velocityY;	// 重力時のY座標変更差分

	PlayerState state;	// 生きてるか死んだかクリアしたかの状態

	const FloatXY stageSize;	// ステージのサイズ

	bool isOffScreen;			// 画面外にいるかどうか

public:

	Player(KeyAction& arg_key, Stage& arg_stage, GravityDir gra);
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

	void SetCrouch(bool wantCrouch);

};

