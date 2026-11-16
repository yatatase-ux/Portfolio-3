#pragma once
#include "KeyAction.h"
#include "FloatXY.h"
#include "FloatXYZ.h"
#include "Stage.h"
#include "GravityDir.h"
#include "ScreenSize.h"
#include "PlayerState.h"

class Camera;

// 画像の中で、キャラクターがどこにどれくらいの大きさで描かれているか
// (画像編集ソフトで1コマを開いて、ピクセル数を測って入れる)
struct SpriteInfo
{
	float footOffset;  // 1コマの下端から、足の裏までの透明な余白(ピクセル)
	float charHeight;  // 頭のてっぺんから足の裏までの高さ(ピクセル)
};

// 画像ごとの情報(値は仮。実際の画像で測って置き換える)
static constexpr SpriteInfo WALK_SIDE_INFO = { 67.0f, 390.0f };
static constexpr SpriteInfo JUMP_SIDE_INFO = { 20.0f, 254.0f };
static constexpr SpriteInfo GRA_CHANGE_INFO = { 20.0f, 360.0f };

class Player
{
	KeyAction& key;				// キー入力の参照	
	Stage& stage;				// ステージの参照

	int walkImageSide[8];
	int walkImageTop[8];
	int walkAnim;
	int GraChengeImage[10];
	bool graAnim = false;
	int animCount;

	bool reverseX;				// TRUEなら左向き
	bool reverseY;				// TUREなら上下逆さ
	int jumpImageSide;
	int jumpImageTop;

	GravityDir currentGravity;	// 現在の重力状態

	const FloatXYZ START_POS = { 120.0f, 570.0f , 200.0f};	// 仮のスタート座標
	FloatXYZ worldPos;	// ワールド座標

	const FloatXY sideSize = { 30.0f, 60.0f };		// 上下重力時のプレイヤーのサイズ
	const FloatXY crouchSize = { 30.0f, 30.0f };	// しゃがみ状態のプレイヤーのサイズ
	const FloatXY topSize = { 30.0f, 30.0f };		// 壁重力時のプレイヤーのサイズ
	const float HalfW = sideSize.x / 2.0f;			// プレイヤーの横幅の半分

	const float JUMP_SPEED = -20.0f;	// ジャンプのスピード
	float jumpHeight;
	bool downJumpKey;


	const float GRAVITY = 1.0f;			// 重力のかかり方
	const float MAX_FALL_SPEED = 50.0f;	// 落下スピードの上限

	FloatXYZ moveDiff;				// 移動分の合計
	FloatXY moveVelo;
	const float MOVE_SPEED = 5.0f;	// プレイヤーの移動スピード
	const float sUP = 0.5f;
	const float sDown = 1.0f;

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
	void Jump();
	FloatXYZ Fall();

	void ResolveHorizontal(FloatXY size);
	void ResolveVertical(FloatXY size);
	void ResolveDepth(FloatXY size);
	void CheckOutOfStage(FloatXY size);

	void SetCrouch(bool wantCrouch);

	void DrawSprite(int handle, const SpriteInfo& info,
		FloatXY footPos, float height, bool upsideDown) const;

};

