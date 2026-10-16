#include "Player.h"
#include "Shape.h"
#include <algorithm>

Player::Player(KeyAction& arg_key, Stage& arg_stage)
	:key(arg_key), stage(arg_stage)
{
	pos = { 50.0f, 200.0f };
	moveDiff = { 0.0f, 0.0f };
	onGround = false;
	velocityY = 0.0f;
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
			velocityY = -20.0f;
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

	bool hitBlock = stage.HitBlock({nextPos.x, pos.y}, size, moveDiff.x);
	if (hitBlock)
	{
		bool dir = (moveDiff.x > 0.0f);
		float side = (dir) ? nextPos.x - 0.01f + size.x / 2.0f : nextPos.x - size.x / 2.0f;
		int indexX = (int)(side / CELL_SIZE);
		float blockX = CELL_SIZE * indexX;
		if(dir)	pos.x = blockX - size.x / 2.0f;
		else	pos.x = blockX + CELL_SIZE + size.x / 2.0f;

		moveDiff.x = 0.0f;
		nextPos = pos + moveDiff;
		nextPos.x = std::clamp(nextPos.x, halfW, stageSize.x - halfW);
	}

	bool isGround = stage.IsGround(nextPos, size, moveDiff.y);
	if (isGround)
	{
		bool dir = (moveDiff.y < 0.0f);
		float bottom = (dir) ? nextPos.y - size.y / 2.0f : nextPos.y + size.y / 2.0f;
		int indexY = bottom / CELL_SIZE;
		float floarY = CELL_SIZE * indexY;
		if (dir)
		{
			pos.y = floarY + CELL_SIZE + size.y / 2.0f;
			velocityY = 0.0f;
		}
		else
		{
			onGround = true;
			pos.y = floarY - size.y / 2.0f;
		}

		moveDiff.y = 0.0f;
	}
	else
	{
		onGround = false;
	}

	pos += moveDiff;

	moveDiff = { 0.0f, 0.0f };
}

void Player::Draw()
{
	DrawCenterBox(pos, size, GetColor(255, 0, 0), TRUE);
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
		velocityY += 1.0f;
	}
	else
	{
		velocityY = 0.0f;
	}

	return diff;
}