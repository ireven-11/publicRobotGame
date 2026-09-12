#pragma once

class ReinforcementUIView
{
public:
	ReinforcementUIView();
	~ReinforcementUIView();

	void drawBackLayer(const VECTOR drawingPosition, const float widht, const float height, const int color);
	void drawReinforcement(const VECTOR drawingPosition, const float widht, const float height, const int graphHandle);
	void drawLevelUpText(const VECTOR drawingPosition, const std::string& text, const int fontHandle, const int textColor);

private:

};