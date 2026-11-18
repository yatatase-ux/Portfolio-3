#pragma once
#include "GravityDir.h"
#include "FloatXY.h"

class Camera;

class MPlayer;

// 横視点の画像の情報
struct SideSpriteInfo
{
	float footOffset;   // 1コマの下端から、足の裏までの透明な余白(ピクセル)
	float standHeight;  // その絵の縮尺で、立ったときの背丈(ピクセル)
};

// 見下ろしの画像の情報
struct TopSpriteInfo
{
	float centerX;   // 1コマの左上から、キャラクターの中心までの横の距離
	float centerY;   // 1コマの左上から、キャラクターの中心までの縦の距離
	float charSize;  // キャラクターの大きさ(縦か横の長いほう)
};

// 画像ごとの情報
static constexpr SideSpriteInfo WALK_SIDE_INFO = { 67.0f, 390.0f };
static constexpr SideSpriteInfo JUMP_SIDE_INFO = { 25.0f, 472.0f };
static constexpr SideSpriteInfo CROUCH_INFO = { 59.0f, 366.0f };
static constexpr SideSpriteInfo GRA_CHANGE_INFO = { 20.0f, 360.0f };
static constexpr TopSpriteInfo  WALK_TOP_INFO = { 240.0f, 233.0f, 300.0f };
static constexpr TopSpriteInfo  JUMP_TOP_INFO = { 99.0f, 100.0f, 200.0f };

// モデルを読んで、プレイヤーを描く(モデルは書き換えない)
class VPlayer
{
	// ---- 画像 ----
	int walkImageSide[8];
	int walkImageTop[8];
	int graChangeImage[10];
	int jumpImageSide;
	int jumpImageTop;
	int crouchImage;

	// ---- アニメーション(見た目だけの状態) ----
	int  walkAnim = 0;       // 歩きのコマ
	int  animCount = 0;      // 歩きのコマを進めるためのカウント
	bool graAnim = false;    // 重力切り替えのアニメーション中か
	int  graAnimCount = 0;   // 重力切り替えのコマ
	GravityDir prevGravity;  // 前のフレームの重力(切り替わったことに気づくため)

public:
	VPlayer(GravityDir startGravity);
	~VPlayer();

	// モデルの状態を見て、アニメーションを進める
	void Update(const MPlayer& model);

	// モデルを読んで描く
	void Draw(const MPlayer& model, const Camera& camera) const;

private:
	void DrawSideSprite(int handle, const SideSpriteInfo& info, FloatXY footPos,
		float height, bool upsideDown, bool faceLeft) const;
	void DrawTopSprite(int handle, const TopSpriteInfo& info, FloatXY centerPos,
		float size, float angle) const;
};