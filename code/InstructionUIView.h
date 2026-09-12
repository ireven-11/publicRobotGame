#pragma once
#include<string>

class InstructionUIView
{
public:
	InstructionUIView();
	~InstructionUIView();

	void drawMissionText(const std::string& text, const VECTOR position, const int font, const int textColor);

private:

};