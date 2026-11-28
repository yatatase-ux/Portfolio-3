#pragma once
#include "FloatXY.h"
#include "FloatXYZ.h"
#include "Stage.h"
#include "GravityDir.h"
#include "PlayerState.h"

// 見下ろしのときに向いている方向
enum class TopDir
{
	Down,
	Left,
	Up,
	Right
};

// Controller が作り、Model が受け取る「プレイヤーがどうしたいか」
// (キーの種類は知らなくていいように、操作の意味だけを持たせる)
struct PlayerInput
{
	bool left = false;			// 左へ動きたい
	bool right = false;			// 右へ動きたい
	bool up = false;			// 上へ動きたい(見下ろしのとき)
	bool down = false;			// 下へ動きたい(見下ろしのとき)
	bool dash = false;			// ダッシュ
	bool jumpPushed = false;	// ジャンプを押した瞬間
	bool jumpHeld = false;		// ジャンプを押し続けている
	bool crouch = false;		// しゃがみたい(横視点のとき)
};

// プレイヤーのデータと、動きのルール(画面やキーのことは知らない)
class MPlayer
{
	Stage& stage;               // ステージの参照(当たり判定に使う)
	const FloatXY stageSize;    // ステージの大きさ

	// ---- 大きさ・速さの定数 ----
	const FloatXYZ START_POS = { 120.0f, 570.0f, 200.0f };  // 仮のスタート座標(z は横視点の基準)
	const FloatXY sideSize = { 30.0f, 60.0f };            // 上下重力のときの大きさ
	const FloatXY crouchSize = { 30.0f, 30.0f };            // しゃがみのときの大きさ
	const FloatXY topSize = { 30.0f, 30.0f };            // 奥重力のときの大きさ
	const float HalfW = sideSize.x / 2.0f;                  // 横幅の半分

	const float JUMP_SPEED = -20.0f;  // ジャンプの速さ
	const float GRAVITY = 1.0f;    // 重力のかかり方
	const float MAX_FALL_SPEED = 50.0f;   // 落下の速さの上限
	const float MOVE_SPEED = 2.5f;    // 移動の速さ
	const float sUP = 0.4f;    // 加速の量
	const float sDown = 0.2f;    // 減速の量

	// ---- 状態 ----
	GravityDir currentGravity;  // 今の重力
	FloatXYZ worldPos;          // ワールド座標
	FloatXYZ moveDiff;          // このフレームの移動量
	FloatXY  moveVelo;          // 横の速さ(慣性用)
	float velocityY;            // 重力の向きに対する速さ
	bool onGround;              // 地面に乗っているか
	bool isCrouched;            // しゃがんでいるか
	bool isDash;
	bool isMoving;              // このフレームで動いたか(アニメーション用)
	bool facingLeft;            // 左を向いているか
	TopDir topDir;              // 見下ろしで向いている方向
	PlayerState state;          // 生きている・ミス・クリア
	bool isOffScreen;           // 画面の外にいるか

public:
	MPlayer(Stage& arg_stage, GravityDir gra);

	void GravityChange(GravityDir gravityDir);  // 重力を切り替える
	void ApplyInput(const PlayerInput& in);     // 入力を受け取って、動きたい量を決める
	void Update();                              // 落下・当たり判定・ミスとクリアの判定

	// ---- View が読むための関数(どれも書き換えない) ----
	FloatXYZ    GetPos()        const { return worldPos; }
	GravityDir  GetGravity()    const { return currentGravity; }
	PlayerState GetState()      const { return state; }
	bool  IsOnGround()          const { return onGround; }
	bool  IsCrouched()          const { return isCrouched; }
	bool  IsDash()				const { return isDash; };
	bool  IsMoving()            const { return isMoving; }
	bool  IsFacingLeft()        const { return facingLeft; }
	bool  IsOffScreen()         const { return isOffScreen; }
	TopDir GetTopDir()          const { return topDir; }
	FloatXY GetSideSize()       const { return sideSize; }
	FloatXY GetCrouchSize()     const { return crouchSize; }
	FloatXY GetTopSize()        const { return topSize; }
	float   GetStartZ()         const { return START_POS.z; }

	// 見下ろしで、今の位置の下に床があるか(影を描くかどうかに使う)
	bool IsOverFloor() const;

private:
	FloatXYZ Move(const PlayerInput& in);
	void Jump(const PlayerInput& in);
	FloatXYZ Fall();

	void ResolveHorizontal(FloatXY size);
	void ResolveVertical(FloatXY size);
	void ResolveDepth(FloatXY size);
	void CheckOutOfStage(FloatXY size);

	void SetCrouch(bool wantCrouch);
};