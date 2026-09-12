#pragma once
#include<vector>
#include<string>
#include"TitleUIData.h"

class TitleUIView
{
public:
	TitleUIView();
	~TitleUIView();

	void draw(const std::vector<ButtonData>& buttonsData, const SelectorData selectorData);

private:
	void drawButtons(const std::vector<ButtonData>& buttonsData);
	void drawSelector(const std::vector<ButtonData>& buttonsData, const SelectorData selectorData);
};