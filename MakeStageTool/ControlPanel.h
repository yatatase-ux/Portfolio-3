#pragma once
#include "LayerEnum.h"
#include <memory>

#include "VPanel.h"

#include "CPalette.h"
#include "VPalette.h"

class ControlPanel
{
	std::unique_ptr<VPanel> vPanel;
	std::unique_ptr<CPalette> cPalette;
	std::unique_ptr<VPalette> vPalette;

	bool saveClicked = false;  // このフレームで保存ボタンが押されたか
	bool prevLeft = false;     // 前のフレームで左ボタンが押されていたか

public:

	ControlPanel();
	
	void Input(FloatXY mouse, Layer layer);
	void Update();
	void Draw(Layer layer);

	BlockType GetSelectedBlock() const { return cPalette->GetSelectedBlock(); };
	
	// 保存ボタンが押されたか(押した瞬間の1フレームだけ true)
	bool IsSaveClicked() const { return saveClicked; }

};

