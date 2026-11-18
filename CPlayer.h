#pragma once
#include "KeyAction.h"
#include "MPlayer.h"

// キーを読んで、「プレイヤーがどうしたいか」(PlayerInput)に変える
// プレイヤーを直接動かすことはしない
class CPlayer
{
	KeyAction& key;

public:
	CPlayer(KeyAction& arg_key) : key(arg_key) {}

	// 今の重力によって、上キーの意味が変わる(横視点ではジャンプ、見下ろしでは移動)
	PlayerInput Read(GravityDir gravity);
};