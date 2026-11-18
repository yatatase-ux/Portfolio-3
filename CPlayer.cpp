#include "CPlayer.h"

PlayerInput CPlayer::Read(GravityDir gravity)
{
	PlayerInput in;
	const bool isBack = (gravity == GravityDir::Back);

	// ---- 移動 ----
	in.left = key.Check(LEFT);
	in.right = key.Check(RIGHT);
	in.up = key.Check(UP);
	in.down = key.Check(DOWN);

	// ---- ジャンプ ----
	// Push は毎フレーム呼ぶ必要があるので、ここで必ず呼ぶ
	in.jumpPushed = key.Push(SPACE) || (!isBack && key.Push(UP));
	in.jumpHeld = key.Check(SPACE) || (!isBack && key.Check(UP));

	// ---- しゃがみ(横視点で下キー) ----
	in.crouch = key.Check(DOWN);

	return in;
}