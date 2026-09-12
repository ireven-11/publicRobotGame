#include"DxLibForIreven.h"
#include"ReinforcementSystem.h"
#include"ReinforcementUIModel.h"

ReinforcementUIModel::ReinforcementUIModel()
{
	data_ = LoadDataJson("statusData/uiData/reinforcementUIStatus.json");

	const auto position = data_["starndard_drawing_graph_position"];

	const auto backLayerColorData	= data_["back_layer_color"];
	backLayerColor_					= GetColor(backLayerColorData[0].get<short>(), backLayerColorData[1].get<short>(), backLayerColorData[2].get<short>());

	reinforcementGraphPosition_ = VGet(position[0].get<float>(), position[1].get<float>(), position[2].get<float>());
	reinforcementGraphWidht_	= data_["graph_widht"].get<float>();
	reinforcementGraphHeight_	= data_["graph_height"].get<float>();
	reinforcementGraphOffset_	= data_["graph_offset_x"].get<float>();

	const auto leftReinforcement = data_["graph_path"]["left"];
	reinforcementGraphs_[ReinforcingType::LEFT_ATTACK_POWER]		= LoadGraph(leftReinforcement["weapon_power_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::LEFT_ATTACK_RANGE]		= LoadGraph(leftReinforcement["weapon_range_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::LEFT_CHANGE_TO_BOMBBUG]	= LoadGraph(leftReinforcement["change_to_bombbug"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::LEFT_CHANGE_TO_CANNON]	= LoadGraph(leftReinforcement["change_to_cannon"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::LEFT_FIRE_RATE]			= LoadGraph(leftReinforcement["fire_rate_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::LEFT_FIRE_SPEED]			= LoadGraph(leftReinforcement["bullet_speed_up"].get<std::string>().c_str());

	const auto rightReinforcement = data_["graph_path"]["right"];
	reinforcementGraphs_[ReinforcingType::RIGHT_ATTACK_POWER]		= LoadGraph(rightReinforcement["weapon_power_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::RIGHT_ATTACK_RANGE]		= LoadGraph(rightReinforcement["weapon_range_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::RIGHT_CHANGE_TO_BOMBBUG]	= LoadGraph(rightReinforcement["change_to_bombbug"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::RIGHT_CHANGE_TO_CANNON]	= LoadGraph(rightReinforcement["change_to_cannon"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::RIGHT_FIRE_RATE]			= LoadGraph(rightReinforcement["fire_rate_up"].get<std::string>().c_str());
	reinforcementGraphs_[ReinforcingType::RIGHT_FIRE_SPEED]			= LoadGraph(rightReinforcement["bullet_speed_up"].get<std::string>().c_str());

	const auto fontData			= data_["font_status"];
	const auto textColorData	= data_["text_color"];
	auto tempPosition			= VGet(init_screen_width * 0.5f, data_["level_up_position_y"].get<float>(), 0.0f);

	textColor_			= GetColor(textColorData[0].get<short>(), textColorData[1].get<short>(), textColorData[2].get<short>());
	levelText_			= data_["level_up_text"].get<std::string>().c_str();
	fontHandle_			= CreateFontToHandle(Utf8ToSJIS(fontData["name"].get<std::string>()).c_str(), 
		fontData["size"].get<int>(), fontData["thick"].get<int>(), DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	levelTextPosition_	= GetCenteredTextPosition(tempPosition, levelText_, fontHandle_);
}

ReinforcementUIModel::~ReinforcementUIModel()
{
	for (size_t i = 0; i < reinforcementGraphs_[ReinforcingType::COUNT]; i++)
	{
		DeleteGraph(reinforcementGraphs_[static_cast<ReinforcingType>(i)]);
	}
}