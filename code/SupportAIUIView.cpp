#include"DxLibForIreven.h"
#include"SupportAIUIView.h"

SupportAIUIView::SupportAIUIView()
{
}

SupportAIUIView::~SupportAIUIView()
{
}

void SupportAIUIView::drawTextBox(const std::string text, const VECTOR textposition, VECTOR boxPosition, const float boxWidht, const float boxHeight,
	const int boxColor, const int fontHandle, const int textColor, const int maxMessageSize)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, UCHAR_MAX * 0.5f);
	DrawBox(boxPosition.x, boxPosition.y, boxPosition.x + boxWidht, boxPosition.y + boxHeight, boxColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//àÍÅAìÒçsñ⁄Ç…ï`âÊÇ∑ÇÈï∂éöóÒÇï€ë∂
	const auto firstLineText	= TruncateString(text, maxMessageSize);
	const auto secondLineText	= SplitOverLength(text, maxMessageSize);

	DrawStringFToHandle(textposition.x, textposition.y, firstLineText.c_str(), textColor, fontHandle);
	DrawStringFToHandle(textposition.x, textposition.y + GetFontSizeToHandle(fontHandle), secondLineText.c_str(), textColor, fontHandle);
}

void SupportAIUIView::drawLogo(const VECTOR position, const float scale, const int graphHandle)
{
	DrawRotaGraphF(position.x, position.y, scale, 0.0f, graphHandle, false);
}