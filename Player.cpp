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
	moveDiff += Move();
}

void Player::Update()
{
	const float halfW = size.x / 2.0f;              // プレイヤーの幅の半分
	const FloatXY stageSize = stage.GetStageSize();

	moveDiff += Gravity();
	FloatXY nextPos = pos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, halfW, stageSize.x - halfW);

	// 収めた位置までの差を、実際の横の移動量にする
	moveDiff.x = nextPos.x - pos.x;

	bool hitBox = stage.IsGround(nextPos, size);
	if (hitBox)
	{
		onGround = true;

		float bottom = nextPos.y + size.y / 2.0f;
		int indexY = bottom / CELL_SIZE;
		float floarY = CELL_SIZE * indexY;
		pos.y = floarY - size.y / 2.0f;

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
	if (key.Check(LEFT))
	{
		diff.x -= MOVE_SPEED;
	}
	if (key.Check(RIGHT))
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