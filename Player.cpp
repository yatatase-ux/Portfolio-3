#include "Player.h"
#include "Shape.h"

Player::Player(KeyAction& arg_key)
	:key(arg_key)
{

}
Player::~Player()
{

}

void Player::Input()
{

}

void Player::Update()
{
	
}

void Player::Draw()
{
	DrawCenterBox(pos, { 50.0f, 100.0f }, GetColor(255, 0, 0), TRUE);
}