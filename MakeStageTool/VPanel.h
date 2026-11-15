#pragma once
#include "LayerEnum.h"
#include "BlockType.h"

class VPanel
{

public:

	VPanel();

	void Draw(Layer layer);

private:

	void DrawBackGround();
	void DrawLayer(Layer layer);
	void DrawSaveButton();

};