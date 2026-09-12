#include"DxLibForIreven.h"
#include"PlayerUIModel.h"
#include"PlayerUIView.h"
#include"PlayerStatus.h"
#include"PlayerUIPresenter.h"
#include"PlayerUIData.h"
#include"WeaponStatus.h"
#include"IWeapon.h"

PlayerUIPresenter::PlayerUIPresenter()
	:model_(std::make_shared<PlayerUIModel>()),
	view_(std::make_shared<PlayerUIView>())
{
}

PlayerUIPresenter::~PlayerUIPresenter()
{
	view_	= nullptr;
	model_	= nullptr;
}

void PlayerUIPresenter::update()
{
	
}

void PlayerUIPresenter::draw(const std::shared_ptr<PlayerStatus>& status)
{
	ScreenAimingCircle aimingCircle;
	aimingCircle.graph		= model_->getScreenAimingCircle();
	aimingCircle.position	= init_screen_center_position;
	aimingCircle.radius		= status->getScreenAttackRangeRadius();

	const float currentHpRate			= NormalizeBetween<float>(status->getHp(), 0.0, status->getStatusData()["max_hp"].get<float>());
	const float currentLevelGageRate	= NormalizeBetween<float>(status->getCurrentExp(), status->getPrevLevelUpExp(), status->getCurrentLevelUpExp());

	GageData tempHpGage;
	tempHpGage.position		= model_->getMainHpGage().position;
	tempHpGage.color		= model_->getMainHpGage().color;
	tempHpGage.height		= model_->getMainHpGage().height;
	tempHpGage.width		= model_->getMainHpGage().width * currentHpRate;
	tempHpGage.position.x	= model_->getMainHpGage().position.x - (model_->getMainHpGage().width - tempHpGage.width) * 0.5f;

	GageData expGage;
	expGage.width		= model_->getMainHpGage().width * currentLevelGageRate;
	expGage.height		= model_->getMainHpGage().height * 0.5f;
	expGage.position	= model_->getMainHpGage().position;
	expGage.position.x	= model_->getMainHpGage().position.x - (model_->getMainHpGage().width - expGage.width) * 0.5f;
	expGage.position.y	= model_->getMainHpGage().position.y + model_->getExpGageOffsetY();
	expGage.color		= model_->getExpGageColor();

	GageData expBackGage;
	expBackGage.width		= model_->getBackHpGage().width;
	expBackGage.height		= model_->getBackHpGage().height * 0.5f;
	expBackGage.position.x	= model_->getBackHpGage().position.x;
	expBackGage.position.y	= model_->getBackHpGage().position.y + model_->getExpGageOffsetY();
	expBackGage.color		= model_->getBackHpGage().color;

	ObjectAxis targetBoxAxis;
	static VECTOR rotationAngle = zero_vector;
	rotationAngle				= VAdd(rotationAngle, VGet(model_->getTargetBoxRotationSpeed(), model_->getTargetBoxRotationSpeed(), model_->getTargetBoxRotationSpeed()));
	//オーバーフロー対策
	if (rotationAngle.x >= DX_PI_F * 2.0f)
	{
		rotationAngle.x = 0.0f;
		rotationAngle.y = 0.0f;
		rotationAngle.z = 0.0f;
	}
	targetBoxAxis.rotationX(rotationAngle.x);
	targetBoxAxis.rotationY(rotationAngle.y);
	targetBoxAxis.rotationZ(rotationAngle.z);

	view_->drawScreenAimingCircle(aimingCircle);
	view_->drawHpGage(tempHpGage, model_->getBackHpGage(), model_->getLayerHpGage(),
		model_->getHpTextPosition(), model_->getHpText(), model_->getFontHandle(), model_->getHpTextColor(),
		model_->getRobotLogoPosition(), model_->getRobotLogoScale(), model_->getRobotLogo());
	view_->drawLevelGage(expGage, expBackGage);
	view_->drawTargetBox(status->getRightWeapon()->getWeaponStatus()->getCurrentTargetingPosition(), model_->getTargetBoxSize(), targetBoxAxis, model_->getTargetBoxColor());
	view_->drawTargetBox(status->getLeftWeapon()->getWeaponStatus()->getCurrentTargetingPosition(), model_->getTargetBoxSize(), targetBoxAxis, model_->getTargetBoxColor());
}