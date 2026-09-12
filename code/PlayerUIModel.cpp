#include"DxLibForIreven.h"
#include"PlayerUIModel.h"
#include"string.h"
#include<vector>

PlayerUIModel::PlayerUIModel()
{
	data_ = LoadDataJson("statusData/uiData/playerUIStatus.json");

	screenAimingCircle_ = LoadGraph(data_["screen_aim_circle_path"].get<std::string>().c_str());

	const auto mainGagePosition = data_["main_hp_gage_position"];
	const auto mainGageColor	= data_["main_hp_gage_color"];
	mainHpGage_.position		= VGet(mainGagePosition[0].get<float>(), mainGagePosition[1].get<float>(), mainGagePosition[2].get<float>());
	mainHpGage_.color			= GetColor(mainGageColor[0].get<char>(), mainGageColor[1].get<char>(), mainGageColor[2].get<char>());
	mainHpGage_.width			= data_["max_main_hp_gage_widht"].get<float>();
	mainHpGage_.height			= data_["main_hp_gage_height"].get<float>();

	const auto backGageColor	= data_["back_hp_gage_color"];
	const auto backGageOffset	= data_["back_hp_gage_offset"];
	const auto backGageOffsetX	= backGageOffset[0].get<float>();
	const auto backGageOffsetY	= backGageOffset[1].get<float>();
	backHpGage_.position.x		= mainHpGage_.position.x;
	backHpGage_.position.y		= mainHpGage_.position.y;
	backHpGage_.color			= GetColor(backGageColor[0].get<char>(), backGageColor[1].get<char>(), backGageColor[2].get<char>());
	backHpGage_.width			= mainHpGage_.width + backGageOffsetX * 2.0f;
	backHpGage_.height			= mainHpGage_.height + backGageOffsetY * 2.0f;

	const auto layerGageColor	= data_["layer_hp_gage_color"];
	const auto layerGageOffset	= data_["layer_hp_gage_offset"];
	const auto layerGageOffsetX = layerGageOffset[0].get<float>();
	const auto layerGageOffsetY = layerGageOffset[1].get<float>();
	layerHpGage_.position.x		= mainHpGage_.position.x;
	layerHpGage_.position.y		= mainHpGage_.position.y;
	layerHpGage_.color			= GetColor(layerGageColor[0].get<char>(), layerGageColor[1].get<char>(), layerGageColor[2].get<char>());
	layerHpGage_.width			= mainHpGage_.width + layerGageOffsetX + 2.0f;
	layerHpGage_.height			= mainHpGage_.height + layerGageOffsetY + 2.0f;

	targetBoxSize_ = data_["target_box_size"].get<float>();

	const auto font			= data_["font"];
	const auto textColor	= data_["hp_text_color"];

	fontHandle_			= CreateFontToHandle(Utf8ToSJIS(font["name"].get<std::string>()).c_str(), font["size"].get<int>(),
		font["thick"].get<int>(), DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	hpTextPosition_		= mainHpGage_.position;
	hpTextPosition_.x	+= data_["hp_text_offset_x"].get<float>();
	hpTextPosition_.y	+= data_["hp_text_offset_y"].get<float>();
	hpTextColor_		= GetColor(textColor[0].get<char>(), textColor[1].get<char>(), textColor[2].get<char>());
	hpText_				= data_["hp_text"].get<std::string>();

	robotLogo_				= LoadGraph(data_["robot_logo_path"].get<std::string>().c_str());
	robotLogoPosition_		= mainHpGage_.position;
	robotLogoPosition_.x	+= data_["robot_logo_offset_x"].get<float>();
	robotLogoPosition_.y	+= data_["robot_logo_offset_y"].get<float>();
	robotLogoScale_			= data_["robot_logo_scale"].get<float>();

	const auto expColor = data_["exp_gage_color"];
	expGageOffsetY_		= data_["exp_gage_offset_y"].get<float>();
	expGageColor_		= GetColor(expColor[0].get<short>(), expColor[1].get<short>(), expColor[2].get<short>());

	const auto boxColor = data_["target_box_color"];
	targetBoxColor_		= GetColor(boxColor[0].get<short>(), boxColor[1].get<short>(), boxColor[2].get<short>());

	targetBoxRotationSpeed_ = data_["target_box_rotation_speed"].get<float>();
}

PlayerUIModel::~PlayerUIModel()
{
	DeleteGraph(screenAimingCircle_);
}