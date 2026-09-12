#pragma once

struct ButtonData
{
	VECTOR position;
	std::string text;
	int colorCode;
	int fontHandle;
};

struct SelectorData
{
	float widht;
	float height;
	int index;
	int color;
};