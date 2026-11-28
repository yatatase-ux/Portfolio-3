#include "VPlayer.h"
#include "Camera.h"
#include "Shape.h"
#include "DxLib.h"
#include <algorithm>
#include "MPlayer.h"

static constexpr float DEPTH_SCALE_RATE = 0.002f;  // z に対する大きさの変わり方
static constexpr float MIN_DEPTH_SCALE = 0.1f;    // 小さくなりすぎないように
static constexpr float MAX_DEPTH_SCALE = 1.5f;    // 大きくなりすぎないように

VPlayer::VPlayer(GravityDir startGravity)
	: prevGravity(startGravity)   // 最初の重力で始めれば、開始時に切り替えアニメが出ない
{
	LoadDivGraph("Data/image/Player/player_walk_side.png", 8, 4, 2, 384, 512, walkImageSide);
	LoadDivGraph("Data/image/Player/gravity_change.png", 10, 5, 2, 1983 / 5, 793 / 2, graChangeImage);
	LoadDivGraph("Data/image/Player/player_walk_top.png", 8, 4, 2, 1536 / 4, 1024 / 2, walkImageTop);
	jumpImageSide = LoadGraph("Data/image/Player/player_jump_side.png");
	jumpImageTop = LoadGraph("Data/image/Player/player_jump_top.png");
	crouchImage = LoadGraph("Data/image/Player/player_crouch.png");
}

VPlayer::~VPlayer()
{
	for (int i = 0; i < 8; i++)
	{
		DeleteGraph(walkImageSide[i]);
		DeleteGraph(walkImageTop[i]);
	}
	for (int i = 0; i < 10; i++)
	{
		DeleteGraph(graChangeImage[i]);
	}
	DeleteGraph(jumpImageSide);
	DeleteGraph(jumpImageTop);
	DeleteGraph(crouchImage);
}

void VPlayer::Update(const MPlayer& model)
{
	// ---- 重力が切り替わったら、切り替えのアニメーションを始める ----
	if (model.GetGravity() != prevGravity)
	{
		graAnim = true;
		graAnimCount = 0;
	}
	prevGravity = model.GetGravity();

	// ---- 重力切り替えのアニメーション(歩いていても進める) ----
	if (graAnim)
	{
		graAnimCount++;
		if (graAnimCount > 9)   // 10コマ(0～9)を出し終わったら終わる
		{
			graAnim = false;
			graAnimCount = 0;
		}
	}

	// ---- 歩き・ダッシュのアニメーション ----
	if (model.IsMoving())
	{
		// ダッシュ中は速く、歩いているときはゆっくりコマを進める
		const int interval = model.IsDash() ? DASH_ANIM_INTERVAL : WALK_ANIM_INTERVAL;

		animCount++;
		if (animCount >= interval)
		{
			animCount = 0;
			walkAnim = (walkAnim + 1) % 7;   // 今までと同じく 0～6 で回す
		}
	}
	else
	{
		animCount = 0;
		walkAnim = 0;
	}
}

void VPlayer::Draw(const MPlayer& model, const Camera& camera) const
{
	const FloatXYZ pos = model.GetPos();

	if (model.GetGravity() == GravityDir::Back)
	{
		// ==== 見下ろし ====
		const float scale = std::clamp(1.0f + pos.z * DEPTH_SCALE_RATE, MIN_DEPTH_SCALE, MAX_DEPTH_SCALE);
		const FloatXY sPos = camera.WorldToScreen({ pos.x, pos.y });

		// ---- 影(浮いていて、下に床があるときだけ) ----
		if (pos.z > 0.0f && model.IsOverFloor())
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCenterBox(sPos, model.GetTopSize(), GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// ---- 本体 ----
		const float angle = static_cast<int>(model.GetTopDir()) * (DX_PI_F / 2.0f);
		const float drawSizePx = model.GetTopSize().y * scale;
		const int alpha = (pos.z < 0.0f)
			? std::clamp(static_cast<int>(255 + pos.z * 1.2f), 0, 255)
			: 255;

		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		if (graAnim)
		{
			DrawRotaGraphF(sPos.x, sPos.y, 0.3, 0.0, graChangeImage[graAnimCount], TRUE);
		}
		else if (model.IsOnGround())
		{
			DrawTopSprite(walkImageTop[walkAnim], WALK_TOP_INFO, sPos, drawSizePx, angle);
		}
		else
		{
			DrawTopSprite(jumpImageTop, JUMP_TOP_INFO, sPos, drawSizePx, angle);
		}
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// ==== 横視点 ====
		const FloatXY pSize = model.IsCrouched() ? model.GetCrouchSize() : model.GetSideSize();
		const float scale = std::clamp(1.0f + (pos.z - model.GetStartZ()) * DEPTH_SCALE_RATE,
			MIN_DEPTH_SCALE, MAX_DEPTH_SCALE);

		// 足元(下重力なら下端、上重力なら上端)
		const bool isUpGravity = (model.GetGravity() == GravityDir::Up);	// 上向き重力ならTRUE
		const float footY = isUpGravity ? pos.y - pSize.y / 2.0f : pos.y + pSize.y / 2.0f;
		const FloatXY footPos = camera.WorldToScreen({ pos.x, footY });

		// 立ったときの背丈を基準にする(しゃがんでも大きさは変わらない)
		const float height = model.GetSideSize().y * scale;
		const bool faceLeft = model.IsFacingLeft();

		const int image = model.IsCrouched() ? crouchImage :
			model.IsOnGround() ? walkImageSide[walkAnim] : jumpImageSide;
		const SideSpriteInfo info = model.IsCrouched() ? CROUCH_INFO :
			model.IsOnGround() ? WALK_SIDE_INFO : JUMP_SIDE_INFO;
		const double size = model.IsOnGround() ? 0.16 : 0.20;

		if (graAnim)
			DrawSideSprite(graChangeImage[graAnimCount], GRA_CHANGE_INFO, footPos, height, isUpGravity, faceLeft);
		else if (model.IsCrouched())
			DrawSideSprite(crouchImage, CROUCH_INFO, footPos, height, isUpGravity, faceLeft);
		else if (model.IsOnGround())
			DrawSideSprite(walkImageSide[walkAnim], WALK_SIDE_INFO, footPos, height, isUpGravity, faceLeft);
		else
			DrawSideSprite(jumpImageSide, JUMP_SIDE_INFO, footPos, height, isUpGravity, faceLeft);

		if (model.IsOffScreen())
		{
			FloatXY icon = camera.WorldToScreen({ pos.x, 50.0f });
			icon.x = std::clamp(icon.x, 50.0f, 1280 - 50.0f);
			icon.y = (model.GetGravity() == GravityDir::Down) ? 50.0f : 720.0f - 50.0f;
			DrawCircle(icon.x, icon.y, 50.0f, GetColor(255, 0, 0), TRUE);
			DrawCircle(icon.x, icon.y, 45.0f, GetColor(255, 255, 0), TRUE);

			FloatXY pDraw = icon;
			pDraw.y = isUpGravity ? pDraw.y - height / 2.0f : pDraw.y + height / 2.0f;
			DrawSideSprite(image, JUMP_SIDE_INFO, pDraw, height, isUpGravity, faceLeft);
		}
	}

	// ==== 画面外にいるときの吹き出し ====
	if (model.IsOffScreen())
	{
		//FloatXY icon = camera.WorldToScreen({ pos.x, 50.0f });
		//icon.x = std::clamp(icon.x, 50.0f, 1280 - 50.0f);
		//icon.y = (model.GetGravity() == GravityDir::Down) ? 50.0f : 720.0f - 50.0f;
		//DrawCircle(icon.x, icon.y, 50.0f, GetColor(255, 0, 0), TRUE);
		//DrawCircle(icon.x, icon.y, 45.0f, GetColor(255, 255, 0), TRUE);

		//const bool isUpGravity = (model.GetGravity() == GravityDir::Up);
		//const int image = model.IsCrouched() ? crouchImage :
		//	              model.IsOnGround() ? walkImageSide[walkAnim] : jumpImageSide;
		//const SideSpriteInfo info = model.IsCrouched() ? CROUCH_INFO :
		//	model.IsOnGround() ? WALK_SIDE_INFO : JUMP_SIDE_INFO;
		//const double size = model.IsOnGround() ? 0.16 : 0.20;
//		DrawSideSprite(image, JUMP_SIDE_INFO, footPos, height, isUpGravity, faceLeft);
	}

	DrawFormatString(20, 45, GetColor(255, 255, 255), "%f", pos.z);
}

void VPlayer::DrawSideSprite(int handle, const SideSpriteInfo& info, FloatXY footPos,
	float height, bool upsideDown, bool faceLeft) const
{
	int w = 0, h = 0;
	GetGraphSize(handle, &w, &h);

	// 立ったときの背丈が height になる拡大率
	const double rate = height / info.standHeight;

	// 回転の中心 = 画像の中の足元
	const float cx = w / 2.0f;
	const float cy = h - info.footOffset;

	if (!upsideDown)
	{
		DrawRotaGraph3F(footPos.x, footPos.y, cx, cy, rate, rate, 0.0, handle, TRUE, faceLeft, FALSE);
	}
	else
	{
		// 足元を中心に 180 度回す(回すと左右も入れ替わるので、反転は逆にする)
		DrawRotaGraph3F(footPos.x, footPos.y, cx, cy, rate, rate, DX_PI, handle, TRUE, !faceLeft, FALSE);
	}
}

void VPlayer::DrawTopSprite(int handle, const TopSpriteInfo& info, FloatXY centerPos,
	float size, float angle) const
{
	const double rate = size / info.charSize;
	DrawRotaGraph3F(centerPos.x, centerPos.y, info.centerX, info.centerY,
		rate, rate, angle, handle, TRUE, FALSE, FALSE);
}