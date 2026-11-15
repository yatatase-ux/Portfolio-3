#include "VPalette.h"
#include "CPalette.h"
#include "DxLib.h"
#include "Shape.h"
#include "StringF.h"
#include "ColorRGB.h"
#include "StageConst.h"
#include "PaletteLayout.h"

VPalette::VPalette()
{

}

void VPalette::Draw(const std::vector<PaletteItem>& items, Layer layer, int onPalette, int selected)
{
	size_t arraySize = items.size();
	for (int i = 0; i < (int)arraySize; i++)
	{
		FloatXY pos = PaletteLayout::GetItemCenter(i);
		FloatXY size = { PaletteLayout::BOX_SIZE, PaletteLayout::BOX_SIZE };
		unsigned int color = (layer == Layer::Side) ? items[i].sideColor : items[i].topColor;

		DrawCenterBox(pos, size, GRAY128, TRUE);
		DrawCenterBox(pos, { CELL_SIZE, CELL_SIZE }, color, TRUE);
		DrawCenterString(pos.x, pos.y + size.y / 2.0f + 20.0f, items[i].name, WHITE, 20.0f);
		if (!CanUse(items[i], layer))
		{
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawCenterBox(pos, size, BLACK, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}
	}
	if(onPalette >= 0)
	{
		FloatXY onPos = PaletteLayout::GetItemCenter(onPalette);
		DrawCenterBox(onPos, { PaletteLayout::BOX_SIZE, PaletteLayout::BOX_SIZE },
			GetColor(128, 128, 0), FALSE, 5.0f);
	}

	FloatXY slectPos = PaletteLayout::GetItemCenter(selected);
	DrawCenterBox(slectPos, { PaletteLayout::BOX_SIZE, PaletteLayout::BOX_SIZE },
		YELLOW, FALSE, 5.0f);
}
