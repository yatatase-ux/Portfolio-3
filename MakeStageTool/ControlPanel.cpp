#include "ControlPanel.h"
#include "PanelLayout.h"
#include "DxLib.h"

ControlPanel::ControlPanel()
{
	vPanel = std::make_unique<VPanel>();

	cPalette = std::make_unique<CPalette>();
	vPalette = std::make_unique<VPalette>();
}

void ControlPanel::Input(FloatXY mouse, Layer layer)
{
	cPalette->Input(mouse, layer);

	// ---- 保存ボタン ----
	const bool left = (GetMouseInput() & MOUSE_INPUT_LEFT) != 0;

	// 押した瞬間だけ反応させる(押しっぱなしで何回も保存しないように)
	// 「今は押されていて、前のフレームは押されていなかった」= 押した瞬間
	saveClicked = left && !prevLeft &&
		PanelLayout::IsInsideSaveButton(mouse.x, mouse.y);

	// 次のフレームのために、今の状態を覚えておく
	prevLeft = left;
}

void ControlPanel::Update()
{

}

void ControlPanel::Draw(Layer layer)
{
	vPanel->Draw(layer);
	vPalette->Draw(cPalette->GetItems(), layer, cPalette->GetOnPalette(), cPalette->GetSelected());
}