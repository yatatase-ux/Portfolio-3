#pragma once
#include "LayerEnum.h"

class VPanel
{
	unsigned int bgColor;

public:

	VPanel();

	void Draw(Layer layer);

private:

	void DrawBackGround();
	
};