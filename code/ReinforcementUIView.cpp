#include"DxLibForIreven.h"
#include"ReinforcementUIView.h"

ReinforcementUIView::ReinforcementUIView()
{
}

ReinforcementUIView::~ReinforcementUIView()
{
}

void ReinforcementUIView::drawBackLayer(const VECTOR drawingPosition, const float widht, const float height, const int color)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UCHAR_MAX * 0.5f);
	DrawBox(drawingPosition.x, drawingPosition.y, drawingPosition.x + widht, drawingPosition.y + height, color, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

void ReinforcementUIView::drawReinforcement(const VECTOR drawingPosition, const float widht, const float height, const int graphHandle)
{
	DrawExtendGraphF(drawingPosition.x, drawingPosition.y, drawingPosition.x + widht, drawingPosition.y + height, graphHandle, true);
}

void ReinforcementUIView::drawLevelUpText(const VECTOR drawingPosition, const std::string& text, const int fontHandle, const int textColor)
{
	DrawStringFToHandle(drawingPosition.x, drawingPosition.y, text.c_str(), textColor, fontHandle);
}