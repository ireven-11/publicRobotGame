#include"DxLibForIreven.h"
#include"InstructionUIView.h"

InstructionUIView::InstructionUIView()
{
}

InstructionUIView::~InstructionUIView()
{
}

void InstructionUIView::drawMissionText(const std::string& text, const VECTOR position, const int font, const int textColor)
{
	DrawStringFToHandle(position.x, position.y, text.c_str(), textColor, font);
}