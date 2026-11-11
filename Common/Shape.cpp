#include "Shape.h"
#include "DxLib.h"
#include <cmath>
#include <algorithm>

void DrawCenterBox(IntXY pos, IntXY size, unsigned int color, bool flag, int LineSize)
{
	float sX = pos.x - size.x / 2.0f;
	float sY = pos.y - size.y / 2.0f;
	float eX = sX + size.x;
	float eY = sY + size.y;

	DrawBoxAA(sX, sY, eX, eY, color, flag, LineSize);

}
void DrawCenterBox(FloatXY pos, FloatXY size, unsigned int color, bool flag, float LineSize)
{
	float sX = pos.x - size.x / 2.0f;
	float sY = pos.y - size.y / 2.0f;
	float eX = sX + size.x;
	float eY = sY + size.y;

	DrawBoxAA(sX, sY, eX, eY, color, flag, LineSize);

}

void DrawCheckered(FloatXY leftTop, FloatXY rightBottom,
	unsigned int color1, unsigned int color2, float patSize)
{
	// 縦と横に、四角がいくつ並ぶか(端数があれば1つ多く数えて、最後で切る)
	const int cols = static_cast<int>(std::ceil((rightBottom.x - leftTop.x) / patSize));
	const int rows = static_cast<int>(std::ceil((rightBottom.y - leftTop.y) / patSize));

	for (int r = 0; r < rows; r++)
	{
		for (int c = 0; c < cols; c++)
		{
			// この四角の左上(始点 + 四角の大きさ × 番号)
			const float x1 = leftTop.x + patSize * c;
			const float y1 = leftTop.y + patSize * r;

			// この四角の右下(長方形からはみ出さないように、右下の端で止める)
			const float x2 = (std::min)(x1 + patSize, rightBottom.x);
			const float y2 = (std::min)(y1 + patSize, rightBottom.y);

			// 縦の番号 + 横の番号 が偶数なら color1、奇数なら color2(市松模様)
			const unsigned int color = ((r + c) % 2 == 0) ? color1 : color2;

			DrawBoxAA(x1, y1, x2, y2, color, TRUE);
		}
	}
}