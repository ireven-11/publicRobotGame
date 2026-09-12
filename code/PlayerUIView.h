#pragma once
#include"PlayerUIData.h"
#include<string>

class PlayerUIView
{
public:
	PlayerUIView();
	~PlayerUIView();

	void drawScreenAimingCircle(const ScreenAimingCircle& circle);
	void drawHpGage(const GageData& mainGage, const GageData& backGage, const GageData& layerGage,
		const VECTOR textPosition, const std::string& text, const int fontHandle, const int textColor,
		const VECTOR logoPosition, const float logoScale, const int logoGraph);
	void drawTargetBox(const VECTOR drawingPosition, const float boxSize, const ObjectAxis& axis, const int boxColor);
	void drawLevelGage(const GageData& expGage, const GageData& backGage);

private:

};