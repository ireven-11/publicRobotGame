#pragma once
#include<vector>
#include<string>

class SupportAIUIView
{
public:
	SupportAIUIView();
	~SupportAIUIView();

	void drawTextBox(const std::string text, const VECTOR textposition, VECTOR boxPosition, const float boxWidht, const float boxHeight,
		const int boxColor, const int fontHandle, const int textColor, const int maxMessageSize);
	void drawLogo(const VECTOR position, const float scale, const int graphHandle);

private:
};