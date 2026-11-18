#include "MPlayer.h"
#include <algorithm>

MPlayer::MPlayer(Stage& arg_stage, GravityDir gra)
	: stage(arg_stage)
	, stageSize(arg_stage.GetStageSize())
	, currentGravity(gra)
{
	// ---- スタート地点(Stage からは足元の座標が返るので、中心に直す) ----
	worldPos = stage.GetStartPos();
	worldPos.y -= (currentGravity == GravityDir::Down) ? sideSize.y / 2.0f :
		(currentGravity == GravityDir::Up) ? -sideSize.y / 2.0f :
		0.0f;
	worldPos.z = (currentGravity == GravityDir::Back) ? 0.0f : START_POS.z;

	moveDiff = { 0.0f, 0.0f, 0.0f };
	moveVelo = { 0.0f, 0.0f };   // ← 前は初期化されていなかったので、ここで 0 にする
	velocityY = 0.0f;
	onGround = false;
	isCrouched = false;
	isMoving = false;
	facingLeft = false;
	topDir = TopDir::Right;
	state = PlayerState::Alive;
	isOffScreen = false;
}

void MPlayer::GravityChange(GravityDir gravityDir)
{
	const GravityDir prev = currentGravity; // 切り替える前の重力

	currentGravity = gravityDir;
	velocityY = 0.0f;
	onGround = false;

	// ---- 見下ろしから横に戻るとき:立てるか、しゃがむかを決める ----
	if (prev == GravityDir::Back && gravityDir != GravityDir::Back)
	{
		// 足元の側の向き(下重力なら +1:下、上重力なら -1:上)
		const float footDir = (gravityDir == GravityDir::Down) ? 1.0f : -1.0f;

		// 足元はそのままで、頭の側へ伸ばして立ったときの中心
		const float shift = (sideSize.y - crouchSize.y) / 2.0f;
		const FloatXY standPos = { worldPos.x, worldPos.y - shift * footDir };

		if (stage.IsBoxFree(standPos, sideSize))
		{
			worldPos.y = standPos.y;  // 立てる空きがある → 立って戻す
			isCrouched = false;
		}
		else
		{
			isCrouched = true;        // 空きがない → しゃがんで戻す
		}
	}

	// ---- 横から見下ろしにするとき:しゃがみは解除する ----
	if (gravityDir == GravityDir::Back)
	{
		isCrouched = false;
	}
}

void MPlayer::ApplyInput(const PlayerInput& in)
{
	moveDiff = { 0.0f, 0.0f, 0.0f };

	Jump(in);
	moveDiff += Move(in);

	if (currentGravity != GravityDir::Back)
	{
		// 横視点で動いたら、奥行き(z)を基準の高さまで少しずつ戻す
		if (moveDiff.x != 0 || moveDiff.y != 0)
		{
			if (worldPos.z < START_POS.z) moveDiff.z += 1.0f;
		}

		SetCrouch(in.crouch);
	}
}

FloatXYZ MPlayer::Move(const PlayerInput& in)
{
	FloatXYZ diff = { 0.0f, 0.0f, 0.0f };

	if (currentGravity == GravityDir::Back)
	{
		// ---- 見下ろし:上下左右に動く。向きも変える ----
		if (!isCrouched)
		{
			if (in.up) { diff.y -= MOVE_SPEED; topDir = TopDir::Up; }
			if (in.down) { diff.y += MOVE_SPEED; topDir = TopDir::Down; }
			if (in.right) { diff.x += MOVE_SPEED; topDir = TopDir::Right; }
			if (in.left) { diff.x -= MOVE_SPEED; topDir = TopDir::Left; }
		}
	}
	else
	{
		// ---- 横視点:目標の速さに向かって、加速・減速する(慣性) ----
		float targetSpeedX = 0.0f;
		if (!isCrouched)
		{
			if (in.left)  targetSpeedX -= MOVE_SPEED;
			if (in.right) targetSpeedX += MOVE_SPEED;
		}

		// しゃがんでいるときは、減速をゆるやかにする(少し滑る)
		const float sDownDir = (isCrouched) ? 0.5f : 1.0f;
		const float step = (targetSpeedX != 0.0f) ? sUP : sDown * sDownDir;
		const float diffSpeed = targetSpeedX - moveVelo.x;
		moveVelo.x += std::clamp(diffSpeed, -step, step);

		diff.x = moveVelo.x;
	}

	return diff;
}

void MPlayer::Jump(const PlayerInput& in)
{
	// 押した瞬間に地面の上なら跳ぶ。上昇中に離したら上昇を止める(長押しで高さが変わる)
	if (in.jumpPushed && onGround)
	{
		velocityY = JUMP_SPEED;
		onGround = false;
	}
	else if (!in.jumpHeld && velocityY < 0.0f)
	{
		velocityY = 0.0f;
	}
}

void MPlayer::Update()
{
	moveDiff += Fall();                                            // 重力による落下
	FloatXYZ nextPos = worldPos + moveDiff;                        // 移動先(仮)
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW); // ステージの左右の外に出ない
	moveDiff.x = nextPos.x - worldPos.x;                           // 収めた分を実際の移動量に

	// ---- 当たり判定 ----
	const FloatXY pSize = (currentGravity == GravityDir::Back) ? topSize :
		(isCrouched) ? crouchSize : sideSize;
	ResolveHorizontal(pSize);
	ResolveVertical(pSize);
	ResolveDepth(pSize);

	worldPos += moveDiff;   // 全部の判定のあとで、位置を決める

	// ---- 画面外・ミス ----
	CheckOutOfStage(pSize);

	// ---- ゴール(クリア) ----
	if (stage.ArriveGoal(worldPos.x + pSize.x / 2.0f))
	{
		state = PlayerState::Clear;
	}

	// ---- 向きと、動いたか(View がアニメーションに使う) ----
	if (moveVelo.x < 0.0f) facingLeft = true;
	if (moveVelo.x > 0.0f) facingLeft = false;
	isMoving = (moveDiff.x != 0.0f || moveDiff.y != 0.0f);

	moveDiff = { 0.0f, 0.0f, 0.0f };
}

FloatXYZ MPlayer::Fall()
{
	FloatXYZ diff = { 0.0f, 0.0f, 0.0f };
	if (!onGround)
	{
		if (currentGravity == GravityDir::Back)
		{
			diff.z -= velocityY;   // 奥重力:z が減る(奥へ落ちる)
		}
		else
		{
			diff.y += velocityY * static_cast<float>(currentGravity);  // 上下重力
		}
		velocityY += GRAVITY;
		velocityY = std::clamp(velocityY, -100.0f, MAX_FALL_SPEED);
	}
	else
	{
		velocityY = 0.0f;
	}
	return diff;
}

void MPlayer::ResolveHorizontal(FloatXY size)
{
	FloatXYZ nextPos = worldPos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);

	if (stage.HitHorizontal({ nextPos.x, worldPos.y, worldPos.z }, size, moveDiff.x))
	{
		const bool dir = (moveDiff.x > 0.0f);  // 右へ動いていたか
		const float side = (dir) ? nextPos.x - 0.01f + size.x / 2.0f : nextPos.x - size.x / 2.0f;
		const float blockX = CELL_SIZE * Stage::ToCellIndex(side);

		// 壁の面に、プレイヤーの端を合わせる
		if (dir) worldPos.x = blockX - size.x / 2.0f;
		else     worldPos.x = blockX + CELL_SIZE + size.x / 2.0f;

		moveDiff.x = 0.0f;
	}
}

void MPlayer::ResolveVertical(FloatXY size)
{
	const FloatXYZ nextPos = worldPos + moveDiff;
	// 動いていればその向き、止まっていれば重力の向き(足元の側)を調べる
	const float checkDir = (moveDiff.y != 0.0f) ? moveDiff.y : static_cast<float>(currentGravity);

	if (!stage.HitVertical(nextPos, size, checkDir))
	{
		if (currentGravity != GravityDir::Back)
		{
			onGround = false;  // 横視点:何にも当たっていない → 空中
		}
		return;
	}

	// ---- 当たった面に合わせる ----
	const bool isUp = (checkDir < 0.0f);
	const float edgeY = (isUp) ? nextPos.y - size.y / 2.0f - 0.01f : nextPos.y + size.y / 2.0f;
	const float blockY = CELL_SIZE * Stage::ToCellIndex(edgeY);

	if (isUp) worldPos.y = blockY + CELL_SIZE + size.y / 2.0f;  // ブロックの下面に頭を合わせる
	else      worldPos.y = blockY - size.y / 2.0f;              // ブロックの上面に足を合わせる

	// ---- 着地か、頭をぶつけたか(横視点だけ) ----
	if (currentGravity != GravityDir::Back)
	{
		if (checkDir * static_cast<float>(currentGravity) > 0.0f)
		{
			onGround = true;   // 重力に引かれて当たった → 着地
		}
		else
		{
			velocityY = 0.0f;  // ジャンプ中に頭をぶつけた → すぐ落ち始める
		}
	}

	moveDiff.y = 0.0f;
}

void MPlayer::ResolveDepth(FloatXY size)
{
	// z の判定は見下ろし(奥重力)のときだけ
	if (currentGravity != GravityDir::Back) return;

	const float nextZ = worldPos.z + moveDiff.z;
	const bool onFloor = stage.OnFloor({ worldPos.x + moveDiff.x, worldPos.y + moveDiff.y }, size);

	// 上から落ちてきて z = 0 をまたいだ、かつ床がある → 着地
	if (onFloor && worldPos.z >= 0.0f && nextZ <= 0.0f)
	{
		moveDiff.z = 0.0f - worldPos.z;
		velocityY = 0.0f;
		onGround = true;
		return;
	}

	onGround = false;  // 床がない、または空中 → 落ち続ける
}

void MPlayer::CheckOutOfStage(FloatXY size)
{
	isOffScreen = false;

	if (currentGravity == GravityDir::Down)
	{
		if (worldPos.y - size.y / 2.0f > stageSize.y + CELL_SIZE / 2.0f) state = PlayerState::Dead;
		isOffScreen = (worldPos.y + size.y / 2.0f <= 0.0f);
	}
	else if (currentGravity == GravityDir::Up)
	{
		if (worldPos.y + size.y / 2.0f < 0.0f) state = PlayerState::Dead;
		isOffScreen = (worldPos.y - size.y / 2.0f >= stageSize.y);
	}
	else
	{
		if (worldPos.z < -300.0f) state = PlayerState::Dead;
	}
}

void MPlayer::SetCrouch(bool wantCrouch)
{
	if (currentGravity == GravityDir::Back) { isCrouched = false; return; }
	if (wantCrouch == isCrouched) return;   // 状態が変わらないなら何もしない

	const float footDir = (currentGravity == GravityDir::Down) ? 1.0f : -1.0f;
	const float shift = (sideSize.y - crouchSize.y) / 2.0f;

	if (wantCrouch)
	{
		worldPos.y += shift * footDir;   // 足元はそのまま、頭の側だけ縮む
		isCrouched = true;
	}
	else
	{
		const FloatXY standPos = { worldPos.x, worldPos.y - shift * footDir };
		if (stage.IsBoxFree(standPos, sideSize))
		{
			worldPos.y = standPos.y;     // 頭の上が空いていれば立つ
			isCrouched = false;
		}
	}
}

bool MPlayer::IsOverFloor() const
{
	return stage.OnFloor({ worldPos.x, worldPos.y }, topSize);
}