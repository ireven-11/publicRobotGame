#pragma once
#include"nlohmannJson.h"
#include<string>

class InstructionUIModel
{
public:
	InstructionUIModel();
	~InstructionUIModel();

	const int getMissionTextColor()const noexcept { return missionTextColor_; }
	const VECTOR getMissionTextPosition()const noexcept { return missionTextPosition_; }
	const int getMissionTextFont()const noexcept { return missonTextFontHandle_; }

private:
	nlohmann::json data_;

	int missionTextColor_;
	VECTOR missionTextPosition_;
	int missonTextFontHandle_;
};