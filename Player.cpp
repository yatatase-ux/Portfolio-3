#include "Player.h"
#include "Shape.h"
#include "StringF.h"
#include <algorithm>

#include "Camera.h"

static constexpr float DEPTH_SCALE_RATE = 0.002f;
static constexpr float MIN_DEPTH_SCALE = 0.1f;  // 穴の底のほうで小さくなりすぎないように
static constexpr float MAX_DEPTH_SCALE = 1.5f;  // 手前に大きく出すぎないように

Player::Player(KeyAction& arg_key, Stage& arg_stage, GravityDir gra)
	:key(arg_key), stage(arg_stage), stageSize(stage.GetStageSize()), currentGravity(gra)
{
	worldPos = stage.GetStartPos();
	worldPos.y -= (currentGravity == GravityDir::Down) ? sideSize.y / 2.0f :
				  (currentGravity == GravityDir::Up)   ?(sideSize.y / 2.0f) * -1 :
															0.0f;
	worldPos.z = (currentGravity == GravityDir::Back) ? 0.0f : 200.0f;
	moveDiff = { 0.0f, 0.0f };
	onGround = false;
	velocityY = 0.0f;

	state = PlayerState::Alive;
	isOffScreen = false;
}

Player::~Player()
{

}

void Player::GravityChange(GravityDir gravityDir)
{
	currentGravity = gravityDir;
	velocityY = 0.0f;
	onGround = false;
}

void Player::Input()
{
	moveDiff = { 0.0f, 0.0f, 0.0f };

	if(onGround)
	{
		if (currentGravity == GravityDir::Back)
		{
			if (key.Push(SPACE))
			{
				velocityY = JUMP_SPEED;
				onGround = false;
			}
		}
		else
		{
			if (key.Push(SPACE) || key.Push(UP))
			{
				velocityY = JUMP_SPEED;
				onGround = false;
			}
		}
	}

	moveDiff += Move();

	if (currentGravity != GravityDir::Back)
	{
		if (moveDiff.x != 0 || moveDiff.y != 0)
		{
			if (worldPos.z < START_POS.z)	moveDiff.z += 1.0f;
		}
	}
}

void Player::Update()
{
	moveDiff += Fall();												// 重力による落下処理
	FloatXYZ nextPos = worldPos + moveDiff;							// 移動先の座標を仮で保管しておく
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);	// プレイヤーがステージの外にいかないようにクランプ
	moveDiff.x = nextPos.x - worldPos.x;							// 収めた位置までの差を、実際の横の移動量にする

	//---- 当たり判定 ----
	FloatXY pSize = (currentGravity == GravityDir::Back) ? topSize : sideSize;	// 現在の重力に応じた当たり判定
	ResolveHorizontal(pSize);		// 横方向の当たり判定
	ResolveVertical(pSize);			// 縦方向の当たり判定
	ResolveDepth(pSize);			// 奥方向の当たり判定

	worldPos += moveDiff;			// 当たり判定など全ての移動処理を終えた上で最終的に worldPos に合算

	// ---- 画面外処理 ----
	CheckOutOfStage(pSize);

	// ---- ゴール処理(クリア) ----
	if (stage.ArriveGoal(worldPos.x + pSize.x / 2.0f)) {
		state = PlayerState::Clear;
	}

	// すべてが終わったら移動差分を０にしておく
	moveDiff = { 0.0f, 0.0f };
}

void Player::Draw(const Camera& camera)
{
	if (currentGravity == GravityDir::Back)
	{
		// ---- z に応じた見た目の倍率 ----
		// z = 0(床の上)で 1 倍、手前(z > 0)ほど大きく、奥(z < 0)ほど小さく
		const float scale = std::clamp(1.0f + worldPos.z * DEPTH_SCALE_RATE,
			MIN_DEPTH_SCALE, MAX_DEPTH_SCALE);
		const FloatXY drawSize = { topSize.x * scale, topSize.y * scale };

		// ワールド座標からスクリーン座標に変換
		FloatXY sPos = camera.WorldToScreen({ worldPos.x, worldPos.y });

		// ---- 影(浮いているときだけ、床の上に元の大きさで描く) ----
		// どこに着地するかが分かるように、足元の位置に半透明の黒を置く
		// 穴の上では床がないので影は描かない
		if (worldPos.z > 0.0f && stage.OnFloor({ worldPos.x, worldPos.y }, topSize))
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
			DrawCenterBox( sPos, topSize, GetColor(0, 0, 0), TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

		// ---- 本体 ----
		// 穴に落ちているとき(z < 0)は、深くなるほど薄くして「奥へ消えていく」感じにする
		// z = 0 で不透明(255)、z = -200 あたりでほぼ見えなくなる
		const int alpha = (worldPos.z < 0.0f)
			? std::clamp(static_cast<int>(255 + worldPos.z * 1.2f), 0, 255)
			: 255;


		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		DrawCenterBox(sPos, drawSize, GetColor(255, 0, 0), TRUE);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}
	else
	{
		// ---- z に応じた見た目の倍率 ----
		// 横視点では z = SIDE_BASE_Z(100)で 1 倍
		// それより小さい(奥にいる)と小さく、大きい(手前にいる)と大きくなる
		const float scale = std::clamp(1.0f + (worldPos.z - START_POS.z) * DEPTH_SCALE_RATE,
			MIN_DEPTH_SCALE, MAX_DEPTH_SCALE);
		const FloatXY drawSize = { sideSize.x * scale, sideSize.y * scale };

		// ---- 足元を基準にする ----
		// 大きさが変わった分を頭の側へ伸ばす(縮むときは頭の側から縮む)
		const float grow = (drawSize.y - sideSize.y) / 2.0f;
		const float drawY = (currentGravity == GravityDir::Down) ? worldPos.y - grow : worldPos.y + grow;
		// ワールド座標からスクリーン座標に変換
		FloatXY sPos = camera.WorldToScreen({ worldPos.x, drawY });
		DrawCenterBox( sPos, drawSize, GetColor(255, 0, 0), TRUE);
	}


//	DrawLeftFormat(20, 20, GetColor(255, 255, 255), 20, "%f, %f", pos.x, pos.y);

	// 画面外(上)に飛び出したときに吹き出しでアイコンを描画させる
	if (isOffScreen)
	{
		FloatXY pos = camera.WorldToScreen({ worldPos.x, 50.0f });
		pos.x = std::clamp(pos.x, 50.0f, 1280 - 50.0f);
		pos.y = (currentGravity == GravityDir::Down) ? 50.0f : 720.0f - 50.0f;
		DrawCircle(pos.x, pos.y, 50.0f, GetColor(255, 0, 0), TRUE);
		DrawCircle(pos.x, pos.y, 45.0f, GetColor(255, 255, 0), TRUE);
		DrawCenterBox( pos , {sideSize.x / 2.0f, sideSize.y / 2.0f}, GetColor(255, 0, 0), TRUE);
	}

	DrawFormatString(20, 45, GetColor(255, 255, 255), "%f", worldPos.z);
}

FloatXYZ Player::Move()
{
	FloatXYZ diff = { 0.0f, 0.0f , 0.0f};
	if (currentGravity == GravityDir::Back)
	{
		if (key.Check(UP) || key.Check(W))
		{
			diff.y -= MOVE_SPEED;
		}
		if (key.Check(DOWN) || key.Check(S))
		{
			diff.y += MOVE_SPEED;
		}
		if (key.Check(RIGHT) || key.Check(D))
		{
			diff.x += MOVE_SPEED;
		}
		if (key.Check(LEFT) || key.Check(A))
		{
			diff.x -= MOVE_SPEED;
		}
	}
	else
	{
		if (key.Check(LEFT) || key.Check(A))
		{
			diff.x -= MOVE_SPEED;
		}
		if (key.Check(RIGHT) || key.Check(D))
		{
			diff.x += MOVE_SPEED;
		}
	}

	return diff;
}

FloatXYZ Player::Fall()
{
	FloatXYZ diff = { 0.0f, 0.0f, 0.0f };
	if (!onGround)
	{
		if (currentGravity == GravityDir::Back)
		{
			diff.z -= velocityY;
			velocityY += GRAVITY;
			velocityY = std::clamp(velocityY, -100.0f, MAX_FALL_SPEED);
		}
		else
		{
			diff.y += velocityY * static_cast<float>(currentGravity);
			velocityY += GRAVITY;
			velocityY = std::clamp(velocityY, -100.0f, MAX_FALL_SPEED);
		}
	}
	else
	{
		velocityY = 0.0f;
	}

	return diff;
}

void Player::ResolveHorizontal(FloatXY size)
{
	FloatXYZ nextPos = worldPos + moveDiff;
	nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);

	bool hitHorizontal = stage.HitHorizontal({ nextPos.x, worldPos.y , worldPos.z}, size, moveDiff.x);
	if (hitHorizontal)
	{
		bool dir = (moveDiff.x > 0.0f);
		float side = (dir) ? nextPos.x - 0.01f + size.x / 2.0f : nextPos.x - size.x / 2.0f;
		float blockX = CELL_SIZE * Stage::ToCellIndex(side);
		if (dir)	worldPos.x = blockX - size.x / 2.0f;
		else	worldPos.x = blockX + CELL_SIZE + size.x / 2.0f;

		moveDiff.x = 0.0f;
		nextPos = worldPos + moveDiff;
		nextPos.x = std::clamp(nextPos.x, HalfW, stageSize.x - HalfW);
	}
}

void Player::ResolveVertical(FloatXY size)
{
	FloatXYZ nextPos = worldPos + moveDiff;
	float checkDir = (moveDiff.y != 0.0f) ? moveDiff.y : static_cast<float>(currentGravity);

	if (!stage.HitVertical( nextPos, size, checkDir))
	{
		if (currentGravity != GravityDir::Back)
		{
			onGround = false; // 横視点:何にも当たっていない → 空中
		}
		return;
	}

	// ---- 当たった面に合わせる ----
	bool isUp = (checkDir < 0.0f); // 上側の端を調べたなら true

	// Stage と同じ計算で行を求める(上端は少し引く)
	float edgeY = (isUp) ? nextPos.y - size.y / 2.0f - 0.01f : nextPos.y + size.y / 2.0f;
	float blockY = CELL_SIZE * Stage::ToCellIndex(edgeY);

	if (isUp) worldPos.y = blockY + CELL_SIZE + size.y / 2.0f; // ブロックの下面に頭(上端)を合わせる
	else      worldPos.y = blockY - size.y / 2.0f;             // ブロックの上面に足元(下端)を合わせる

	// ---- 着地か、頭をぶつけたか ----
	// 調べた向きと重力の向きが同じ(掛けて正)なら、重力に引かれて当たった → 着地
	if (currentGravity != GravityDir::Back)
	{
		if (checkDir * static_cast<float>(currentGravity) > 0.0f)
		{
			onGround = true;  // 重力に引かれて当たった → 着地
		}
		else
		{
			velocityY = 0.0f; // ジャンプ中に頭をぶつけた → すぐ落ち始める
		}
	}

	moveDiff.y = 0.0f;
}

void Player::ResolveDepth(FloatXY size)
{
	// z の判定は見下ろし(奥重力)のときだけ
	if (currentGravity != GravityDir::Back) return;

	const float nextZ = worldPos.z + moveDiff.z;

	// 床の上にいるか(横・縦の判定で直したあとの x, y で調べる)
	const bool onFloor = stage.OnFloor({ worldPos.x + moveDiff.x, worldPos.y + moveDiff.y }, size);

	// 上から落ちてきて z = 0 をまたいだ、かつ床がある → 着地
	// (pos.z >= 0 を条件にするのは、穴に落ちて z がマイナスになったあと、
	//  床の下にもぐったときに、床の上へワープしないようにするため)
	if (onFloor && worldPos.z >= 0.0f && nextZ <= 0.0f)
	{
		moveDiff.z = 0.0f - worldPos.z; // ちょうど z = 0 になる移動量にする
		velocityY = 0.0f;
		onGround = true;
		return;
	}

	// 床がない(穴の上)、または空中 → 落ち続ける
	onGround = false;
}

void Player::CheckOutOfStage(FloatXY size)
{
	isOffScreen = false;
	// 落下死処理と画面外(上)の処理
	if (currentGravity == GravityDir::Down)
	{ // 下向き重力
		if (worldPos.y - size.y / 2.0f > stageSize.y + CELL_SIZE / 2.0f)
		{
			state = PlayerState::Dead;
		}

		isOffScreen = (worldPos.y + size.y / 2.0f <= 0.0f);
	}
	else if (currentGravity == GravityDir::Up)
	{ // 上向き重力
		if (worldPos.y + size.y / 2.0f < 0.0f)
		{
			state = PlayerState::Dead;
		}

		isOffScreen = (worldPos.y - size.y / 2.0f >= stageSize.y);
	}
	else
	{ // 奥向き重力
		if (worldPos.z < -300.0f)
		{
			state = PlayerState::Dead;
		}
	}
}