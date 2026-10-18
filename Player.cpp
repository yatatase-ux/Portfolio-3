#include "Player.h"
#include "Shape.h"
#include "StringF.h"
#include <algorithm>

Player::Player(KeyAction& arg_key, Stage& arg_stage)
	:key(arg_key), stage(arg_stage)
{
	pos = START_POS;
	moveDiff = { 0.0f, 0.0f };
	onGround = false;
	velocityY = 0.0f;

	state = pState::Alive;
	isOffScreen = false;
}

Player::~Player()
{

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
	const float halfW = size.x / 2.0f;              // プレイヤーの幅の半分
	const FloatXY stageSize = stage.GetStageSize();

	moveDiff += Gravity();
	FloatXY nextPos = pos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, halfW, stageSize.x - halfW);
	moveDiff.x = nextPos.x - pos.x;	// 収めた位置までの差を、実際の横の移動量にする

	bool hitHorizontal = stage.HitHorizontal({nextPos.x, pos.y}, size, moveDiff.x);
	if (hitHorizontal)
	{
		bool dir = (moveDiff.x > 0.0f);
		float side = (dir) ? nextPos.x - 0.01f + size.x / 2.0f : nextPos.x - size.x / 2.0f;
		float blockX = CELL_SIZE * Stage::ToCellIndex(side);
		if(dir)	pos.x = blockX - size.x / 2.0f;
		else	pos.x = blockX + CELL_SIZE + size.x / 2.0f;

		moveDiff.x = 0.0f;
		nextPos = pos + moveDiff;
		nextPos.x = std::clamp(nextPos.x, halfW, stageSize.x - halfW);
	}

	bool hitVertical = stage.HitVertical(nextPos, size, moveDiff.y);
	if (hitVertical)
	{
		bool dir = (moveDiff.y < 0.0f);
		float bottom = (dir) ? nextPos.y - size.y / 2.0f : nextPos.y + size.y / 2.0f;
		float blockY = CELL_SIZE * Stage::ToCellIndex(bottom);
		if (dir)
		{
			pos.y = blockY + CELL_SIZE + size.y / 2.0f;
			velocityY = 0.0f;
		}
		else
		{
			onGround = true;
			pos.y = blockY - size.y / 2.0f;
		}

		moveDiff.y = 0.0f;
	}
	else
	{
		onGround = false;
	}

	pos += moveDiff;

	if (pos.y - size.y / 2.0f > stageSize.y + CELL_SIZE / 2.0f)
	{
		state = pState::Died;
	}

	isOffScreen = (pos.y + size.y / 2.0f <= 0.0f);

	moveDiff = { 0.0f, 0.0f };
}

void Player::Draw()
{
	DrawCenterBox(pos, size, GetColor(255, 0, 0), TRUE);

	DrawLeftFormat(20, 20, GetColor(255, 255, 255), 20, "%f, %f", pos.x, pos.y);

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
		diff.y += velocityY;
		velocityY += GRAVITY_SPEED;
	}
	else
	{
		velocityY = 0.0f;
	}

	return diff;
}