#include "Player.h"

Player::Player(KeyAction& arg_key, Stage& arg_stage, GravityDir gra)
	: model(arg_stage, gra)
	, controller(arg_key)
	, view(gra)
{
}

void Player::Input()
{
	// キーを読んで「どうしたいか」に変え、モデルに渡す
	const PlayerInput in = controller.Read(model.GetGravity());
	model.ApplyInput(in);
}

void Player::Update()
{
	model.Update();     // 動き・当たり判定
	view.Update(model); // アニメーションを進める
}

void Player::Draw(const Camera& camera) const
{
	view.Draw(model, camera);
}

void Player::Clear()
{
	PlayerInput in;
	in.right = true;
	model.ApplyInput(in);
	Update();
}