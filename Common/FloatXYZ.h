#pragma once
#pragma once

class FloatXYZ
{
public:
	float x, y, z;

	// コンストラクタ
	FloatXYZ(float x_ = 0.0f, float y_ = 0.0f, float z_ = 0.0f)
		: x(x_), y(y_), z(z_)
	{
	}

	// ---- 2つの FloatXY 同士の計算(結果は新しい FloatXY) ----

	// 足し算: a + b
	FloatXYZ operator+(const FloatXYZ& other) const
	{
		return FloatXYZ(x + other.x, y + other.y, z + other.z);
	}

	// 引き算: a - b
	FloatXYZ operator-(const FloatXYZ& other) const
	{
		return FloatXYZ(x - other.x, y - other.y, z - other.z);
	}

	FloatXYZ operator*(const FloatXYZ other) const
	{
		return FloatXYZ(x * other.x, y * other.y, z * other.z);
	}

	FloatXYZ operator/(const FloatXYZ other) const
	{
		return FloatXYZ(x / other.x, y / other.y, z / other.z);
	}

	// ---- 自分自身を書き換える計算 ----

	FloatXYZ& operator=(const FloatXYZ& other)
	{
		x = other.x;
		y = other.y;
		z = other.z;
		return *this;
	}

	FloatXYZ& operator+=(const FloatXYZ& other)
	{
		x += other.x;
		y += other.y;
		z += other.z;
		return *this;
	}

	FloatXYZ& operator-=(const FloatXYZ& other)
	{
		x -= other.x;
		y -= other.y;
		z -= other.z;
		return *this;
	}

	FloatXYZ& operator*=(const FloatXYZ& other)
	{
		x *= other.x;
		y *= other.y;
		z *= other.z;
		return *this;
	}

	FloatXYZ& operator/=(const FloatXYZ& other)
	{
		x /= other.x;
		y /= other.y;
		z /= other.z;
		return *this;
	}
};