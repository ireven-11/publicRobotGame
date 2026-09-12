#pragma once
#include"nlohmannJson.h"
#include"PlayerUIData.h"
#include<string>

class PlayerUIModel
{
public:
	PlayerUIModel();
	~PlayerUIModel();

	const int getScreenAimingCircle()const noexcept { return screenAimingCircle_; }
	const GageData getMainHpGage()const noexcept { return mainHpGage_; }
	const GageData getBackHpGage()const noexcept { return backHpGage_; }
	const GageData getLayerHpGage()const noexcept { return layerHpGage_; }
	const float getTargetBoxSize()const noexcept { return targetBoxSize_; }
	const int getFontHandle()const noexcept { return fontHandle_; }
	const VECTOR getHpTextPosition()const noexcept { return hpTextPosition_; }
	const int getHpTextColor()const noexcept { return hpTextColor_; }
	const std::string getHpText()const noexcept { return hpText_; }
	const int getRobotLogo()const noexcept { return robotLogo_; }
	const VECTOR getRobotLogoPosition()const noexcept { return robotLogoPosition_; }
	const float getRobotLogoScale()const noexcept { return robotLogoScale_; }
	const float getExpGageOffsetY()const noexcept { return expGageOffsetY_; }
	const int getExpGageColor()const noexcept { return expGageColor_; }
	const int getTargetBoxColor()const noexcept { return targetBoxColor_; }
	const float getTargetBoxRotationSpeed()const noexcept { return targetBoxRotationSpeed_; }

private:
	nlohmann::json data_;

	int screenAimingCircle_;
	GageData mainHpGage_;
	GageData backHpGage_;
	GageData layerHpGage_;
	float targetBoxSize_;
	int fontHandle_;
	VECTOR hpTextPosition_;
	int hpTextColor_;
	std::string hpText_;
	int robotLogo_;
	VECTOR robotLogoPosition_;
	float robotLogoScale_;
	float expGageOffsetY_;
	int expGageColor_;
	int targetBoxColor_;
	float targetBoxRotationSpeed_;
};