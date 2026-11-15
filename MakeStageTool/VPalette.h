#pragma once
#include <vector>
#include "LayerEnum.h"

struct PaletteItem;

class VPalette
{

public:
	VPalette();
	void Draw(const std::vector<PaletteItem>& items, Layer layer, int onPalette, int selected);
};

