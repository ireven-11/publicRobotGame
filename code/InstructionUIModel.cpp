#include"DxLibForIreven.h"
#include"InstructionUIModel.h"
#include"string.h"
#include<vector>

InstructionUIModel::InstructionUIModel()
{
	data_ = LoadDataJson("statusData/uiData/instructionUIStatus.json");

	const auto missionColor		= data_["mission_text_color"];
	const auto missionPosition	= data_["misson_text_position"];
	const auto missionFont		= data_["misson_text_font"];

	missionTextColor_		= GetColor(missionColor[0].get<char>(), missionColor[1].get<char>(), missionColor[2].get<char>());
	missionTextPosition_	= VGet(missionPosition[0].get<float>(), missionPosition[1].get<float>(), missionPosition[2].get<float>());
	missonTextFontHandle_	= CreateFontToHandle(Utf8ToSJIS(missionFont["name"].get<std::string>()).c_str(), missionFont["size"].get<int>(),
		missionFont["thick"].get<int>(), DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
}

InstructionUIModel::~InstructionUIModel()
{
	
}