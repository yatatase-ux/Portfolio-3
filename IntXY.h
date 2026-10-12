#pragma once

class IntXY
{
public:
	int x, y;

	// ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	IntXY(int x_ = 0, int y_ = 0)
		: x(x_), y(y_)
	{
	}

	// ---- 2‚Â‚Ì IntXY “¯m‚ÌŒvZ(Œ‹‰Ê‚ÍV‚µ‚¢ IntXY) ----

	// ‘«‚µZ: a + b
	IntXY operator+(const IntXY& other) const
	{
		return IntXY(x + other.x, y + other.y);
	}

	// ˆø‚«Z: a - b
	IntXY operator-(const IntXY& other) const
	{
		return IntXY(x - other.x, y - other.y);
	}

	// Š|‚¯Z: ¬•ª‚²‚Æ‚ÉŠ|‚¯‚é (x*x, y*y)
	IntXY operator*(const IntXY& other) const
	{
		return IntXY(x * other.x, y * other.y);
	}

	// Š„‚èZ: ¬•ª‚²‚Æ‚ÉŠ„‚é (x/x, y/y)
	IntXY operator/(const IntXY& other) const
	{
		return IntXY(x / other.x, y / other.y);
	}

	// ---- ©•ª©g‚ğ‘‚«Š·‚¦‚éŒvZ ----

	// ‘ã“ü: a = b
	IntXY& operator=(const IntXY& other)
	{
		x = other.x;
		y = other.y;
		return *this;
	}

	// a += b
	IntXY& operator+=(const IntXY& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	// a -= b
	IntXY& operator-=(const IntXY& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	// a *= b
	IntXY& operator*=(const IntXY& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	// a /= b
	// Ø‚èÌ‚Ä‚Æ 0 œZ‚É’ˆÓ(operator/ ‚Æ“¯‚¶)
	IntXY& operator/=(const IntXY& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}
};