#pragma once
#include "DxLib.h"
#include "FloatXY.h"

// マウスのボタンの種類
enum class MouseButton
{
	Left,    // 左ボタン
	Right,   // 右ボタン
	Middle,  // 中ボタン(ホイールの押し込み)

	Max      // ボタンの数(番号としては使わない)
};

// マウスの入力をまとめて扱うクラス
// 毎フレームの最初に Update() を1回だけ呼び、あとは Push / Check などで結果を見る
class MouseAction
{
	static constexpr int BUTTON_MAX = static_cast<int>(MouseButton::Max);

	// MouseButton の並び順に対応する、DXライブラリのボタンの値
	static constexpr int buttonMask[BUTTON_MAX] =
	{
		MOUSE_INPUT_LEFT,    // Left
		MOUSE_INPUT_RIGHT,   // Right
		MOUSE_INPUT_MIDDLE,  // Middle
	};

	bool now[BUTTON_MAX] = {};  // このフレームで押されているか
	bool prev[BUTTON_MAX] = {};  // 前のフレームで押されていたか

	int x = 0;      // マウスの位置
	int y = 0;
	int wheel = 0;  // このフレームで回したホイールの量

	// MouseButton を配列の番号に直す
	static int Index(MouseButton b) { return static_cast<int>(b); }

public:

	/// <summary>
	/// 毎フレームの最初に1回だけ呼ぶ。ボタン・位置・ホイールを読み取る
	/// </summary>
	void Update()
	{
		const int input = GetMouseInput();

		for (int i = 0; i < BUTTON_MAX; i++)
		{
			prev[i] = now[i];                          // 前のフレームの状態を覚える
			now[i] = (input & buttonMask[i]) != 0;    // 今のフレームの状態を読む
		}

		GetMousePoint(&x, &y);

		// ホイールは「前回呼んでから」の量なので、1フレームに1回だけここで読む
		wheel = GetMouseWheelRotVol();
	}

	/// <summary>
	/// 押している間ずっと true(長押し)
	/// </summary>
	bool Check(MouseButton b) const
	{
		return now[Index(b)];
	}

	/// <summary>
	/// 押した瞬間だけ true(短押し)
	/// </summary>
	bool Push(MouseButton b) const
	{
		return now[Index(b)] && !prev[Index(b)];
	}

	/// <summary>
	/// 離した瞬間だけ true
	/// </summary>
	bool Release(MouseButton b) const
	{
		return !now[Index(b)] && prev[Index(b)];
	}

	/// <summary>
	/// マウスの位置(画面上の座標)
	/// </summary>
	FloatXY GetPos() const
	{
		return { static_cast<float>(x), static_cast<float>(y) };
	}

	/// <summary>
	/// このフレームで回したホイールの量(奥に回すとプラス)
	/// </summary>
	int GetWheel() const
	{
		return wheel;
	}

	bool CheckInBox(FloatXY boxCenterPos, FloatXY boxSize)
	{
		float fx = (float)x;
		float fy = (float)y;
		if (fx > boxCenterPos.x - boxSize.x / 2.0f &&
			fx < boxCenterPos.x + boxSize.x / 2.0f &&
			fy > boxCenterPos.y - boxSize.y / 2.0f &&
			fy < boxCenterPos.y + boxSize.y / 2.0f )
		{
			return true;
		}

		return false;
	}
};