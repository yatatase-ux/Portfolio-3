#pragma once

class FloatXY
{
public:
	float x, y;

	// コンストラクタ
	FloatXY(float x_ = 0.0f, float y_ = 0.0f)
		: x(x_), y(y_)
	{
	}

	// ---- 2つの FloatXY 同士の計算(結果は新しい FloatXY) ----

	// 足し算: a + b
	FloatXY operator+(const FloatXY& other) const
	{
		return FloatXY(x + other.x, y + other.y);
	}

	// 引き算: a - b
	FloatXY operator-(const FloatXY& other) const
	{
		return FloatXY(x - other.x, y - other.y);
	}

	FloatXY operator*(const FloatXY other) const
	{
		return FloatXY(x * other.x, y * other.y);
	}

	FloatXY operator/(const FloatXY other) const
	{
		return FloatXY(x / other.x, y / other.y);
	}

	// ---- 自分自身を書き換える計算 ----

	FloatXY& operator=(const FloatXY& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	FloatXY& operator+=(const FloatXY& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	FloatXY& operator-=(const FloatXY& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	FloatXY& operator*=(const FloatXY& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	FloatXY& operator/=(const FloatXY& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}
};