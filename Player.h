#pragma once
#include "MPlayer.h"
#include "VPlayer.h"
#include "CPlayer.h"

class Camera;

// Model・Controller・View を持って、順番に呼ぶだけのクラス
// (外から見た使い方は、分ける前と同じ)
class Player
{
	// メンバは書いた順に作られるので、model を view より先に置く
	MPlayer      model;
	CPlayer controller;
	VPlayer       view;

public:
	Player(KeyAction& arg_key, Stage& arg_stage, GravityDir gra);

	void GravityChange(GravityDir gravityDir) { model.GravityChange(gravityDir); }

	void Input();
	void Update();
	void Draw(const Camera& camera) const;

	PlayerState GetState() const { return model.GetState(); }
	FloatXYZ    GetPos()   const { return model.GetPos(); }
};