#include "Player.h"
#include "Shape.h"
#include "StringF.h"
#include <algorithm>

Player::Player(KeyAction& arg_key, Stage& arg_stage)
	:key(arg_key), stage(arg_stage), stageSize(stage.GetStageSize())
{
	gravityDiff = 1.0f;

	pos = START_POS;
	moveDiff = { 0.0f, 0.0f };
	onGround = false;
	velocityY = 0.0f;

	state = PlayerState::Alive;
	isOffScreen = false;
}

Player::~Player()
{

}

void Player::GravityChange(int gravityDir)
{
	gravityDiff = gravityDir;
	velocityY = 0.0f;
	onGround = false;
}

void Player::Input()
{
	moveDiff = { 0.0f, 0.0f };
	if(onGround)
	{
		if (key.Push(SPACE) || key.Push(UP))
		{
			velocityY = JUMP_HEIGHT;
			onGround = false;
		}
	}
	moveDiff += Move();
}

void Player::Update()
{
	moveDiff += Gravity();
	FloatXY nextPos = pos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);
	moveDiff.x = nextPos.x - pos.x;	// 収めた位置までの差を、実際の横の移動量にする

	float stageTop = (gravityDiff > 0) ? stageSize.y + CELL_SIZE / 2.0f : 0.0f;
	float stageUnder = (gravityDiff > 0) ? 0.0f : stageSize.y + CELL_SIZE / 2.0f;

	ResolveHorizontal();
	ResolveVertical();

	pos += moveDiff;

	if (gravityDiff > 0)
	{
		if (pos.y - size.y / 2.0f > stageSize.y + CELL_SIZE / 2.0f)
		{
			state = PlayerState::Died;
		}

		isOffScreen = (pos.y + size.y / 2.0f <= 0.0f);
	} 
	else if(gravityDiff < 0)
	{
		if (pos.y + size.y / 2.0f < 0.0f)
		{
			state = PlayerState::Died;
		}

		isOffScreen = (pos.y - size.y / 2.0f >= stageSize.y + CELL_SIZE / 2.0f);
	}

	// すべてが終わったら移動差分を０にする
	moveDiff = { 0.0f, 0.0f };
}

void Player::Draw()
{
	DrawCenterBox(pos, size, GetColor(255, 0, 0), TRUE);

//	DrawLeftFormat(20, 20, GetColor(255, 255, 255), 20, "%f, %f", pos.x, pos.y);

	if (isOffScreen)
	{
		float x = pos.x;
		x = std::clamp(x, 50.0f, 1280 - 50.0f);
		float y = 50.0f;
		DrawCircle(x, y, 50.0f, GetColor(255, 0, 0), TRUE);
		DrawCircle(x, y, 45.0f, GetColor(255, 255, 0), TRUE);
		DrawCenterBox({ x, y }, {size.x / 2.0f, size.y / 2.0f}, GetColor(255, 0, 0), TRUE);
	}
}

FloatXY Player::Move()
{
	FloatXY diff = { 0.0f, 0.0f };
	if (key.Check(LEFT) || key.Check(A))
	{
		diff.x -= MOVE_SPEED;
	}
	if (key.Check(RIGHT) || key.Check(D))
	{
		diff.x += MOVE_SPEED;
	}

	return diff;
}

FloatXY Player::Gravity()
{
	FloatXY diff = { 0.0f, 0.0f };
	if (!onGround)
	{
		diff.y += velocityY * gravityDiff;
		velocityY += GRAVITY_SPEED;
		velocityY = std::clamp(velocityY, -100.0f, MAX_GRAVITY);
	}
	else
	{
		velocityY = 0.0f;
	}

	return diff;
}

void Player::ResolveHorizontal()
{
	FloatXY nextPos = pos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);

	bool hitHorizontal = stage.HitHorizontal({ nextPos.x, pos.y }, size, moveDiff.x);
	if (hitHorizontal)
	{
		bool dir = (moveDiff.x > 0.0f);
		float side = (dir) ? nextPos.x - 0.01f + size.x / 2.0f : nextPos.x - size.x / 2.0f;
		float blockX = CELL_SIZE * Stage::ToCellIndex(side);
		if (dir)	pos.x = blockX - size.x / 2.0f;
		else	pos.x = blockX + CELL_SIZE + size.x / 2.0f;

		moveDiff.x = 0.0f;
		nextPos = pos + moveDiff;
		nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);
	}
}

void Player::ResolveVertical()
{
	FloatXY nextPos = pos + moveDiff;
	float checkDir = (moveDiff.y != 0.0f) ? moveDiff.y : gravityDiff;

	if (!stage.HitVertical(nextPos, size, checkDir))
	{
		// 何にも当たっていない → 空中
		onGround = false;
		return;
	}

	// ---- 当たった面に合わせる ----
	bool isUp = (checkDir < 0.0f); // 上側の端を調べたなら true

	// Stage と同じ計算で行を求める(上端は少し引く)
	float edgeY = (isUp) ? nextPos.y - size.y / 2.0f - 0.01f : nextPos.y + size.y / 2.0f;
	float blockY = CELL_SIZE * Stage::ToCellIndex(edgeY);

	if (isUp) pos.y = blockY + CELL_SIZE + size.y / 2.0f; // ブロックの下面に頭(上端)を合わせる
	else      pos.y = blockY - size.y / 2.0f;             // ブロックの上面に足元(下端)を合わせる

	// ---- 着地か、頭をぶつけたか ----
	// 調べた向きと重力の向きが同じ(掛けて正)なら、重力に引かれて当たった → 着地
	if (checkDir * gravityDiff > 0.0f)
	{
		onGround = true;
	}
	else
	{
		// ジャンプ中に頭をぶつけた → すぐ落ち始める
		velocityY = 0.0f;
	}

	moveDiff.y = 0.0f;
}