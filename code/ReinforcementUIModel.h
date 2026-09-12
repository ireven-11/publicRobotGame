#pragma once
#include"nlohmannJson.h"
#include<string>
#include<unordered_map>

enum class ReinforcingType;

class ReinforcementUIModel
{
public:
	ReinforcementUIModel();
	~ReinforcementUIModel();

	const int getBackLayerColor()const noexcept { return backLayerColor_; }
	const VECTOR getReinforcementGraphPosition()const noexcept { return reinforcementGraphPosition_; }
	const float getReinforcementGraphWidht()const noexcept { return reinforcementGraphWidht_; }
	const float getReinforcementGraphHeight()const noexcept { return reinforcementGraphHeight_; }
	const float getReinforcementGraphOffset()const noexcept { return reinforcementGraphOffset_; }
	std::unordered_map<ReinforcingType, int> getReinforcementGraphs()const noexcept { return reinforcementGraphs_; }
	const int getTextColor()const noexcept { return textColor_; }
	const int getFontHandle()const noexcept { return fontHandle_; }
	const std::string getLevelText()const noexcept { return levelText_; }
	const VECTOR getLevelTextPosition()const noexcept { return levelTextPosition_; }

private:
	nlohmann::json data_;

	int backLayerColor_;
	VECTOR reinforcementGraphPosition_;
	float reinforcementGraphWidht_;
	float reinforcementGraphHeight_;
	float reinforcementGraphOffset_;
	std::unordered_map<ReinforcingType, int> reinforcementGraphs_;
	int textColor_;
	int fontHandle_;
	std::string levelText_;
	VECTOR levelTextPosition_;
};