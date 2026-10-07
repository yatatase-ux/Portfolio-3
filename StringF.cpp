#include "StringF.h"
#include "DxLib.h"

void DrawCenterString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x - GT_w / 2;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawCenterString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x - (float)GT_w / 2.0f;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

void DrawRightString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x - GT_w;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawRightString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x - (float)GT_w;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

void DrawLeftString(int x, int y, const char* text, unsigned int color, int size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	int draw_x = x;
	int draw_y = y - size / 2;

	DrawString(draw_x, draw_y, text, color);
}

void DrawLeftString(float x, float y, const char* text, unsigned int color, float size)
{
	SetFontSize(size);

	int GT_s = strlen(text);
	int GT_w = GetDrawStringWidth(text, GT_s);

	float draw_x = x;
	float draw_y = y - (float)size / 2.0f;

	DrawStringF(draw_x, draw_y, text, color);
}

