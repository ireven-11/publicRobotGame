#include"DxLibForIreven.h"
#include"IWeapon.h"
#include"PlayerStatus.h"
#include"InputHandler.h"
#include"DInput.h"
#include"Cannon.h"
#include"Physics.h"
#include"LevelUpEvent.h"
#include"PlayerEventHandler.h"
#include"OnDamageEvent.h"

PlayerStatus::PlayerStatus()
{
	statusData_				= LoadDataJson("statusData/playerStatus.json");
	std::string modelPath	= statusData_["model_path"].get<std::string>();
	modelHandle_			= MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	float modelScale		= statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	//ボーンの名前とindexを保存
	MV1SaveModelNameAndIndex(modelHandle_, modelBoneIndex_);

	screenAttackRangeRadius_	= statusData_["screen_attack_range_radius"].get<float>();
	capsuleCollierRadius_		= statusData_["model_scale"].get<float>() * statusData_["adjust_collider_capusule_radius"].get<float>();
	init();
}

PlayerStatus::~PlayerStatus()
{
}

void PlayerStatus::init()
{
	rightWeapon_		= std::make_shared<Cannon>(EquipSlot::RIGHT_HAND);
	leftWeapon_			= std::make_shared<Cannon>(EquipSlot::LEFT_HAND);
	shoulderWeapon_		= std::make_shared<Cannon>(EquipSlot::RIGHT_SHOULDER);
	hp_					= statusData_["max_hp"].get<float>();
	position_			= VGet(0.0f, 0.0f, 0.0f);
	speed_				= 0.0f;
	modelAngleY_		= 0.0f;
	facingDirection_	= VGet(0.0f, 0.0f, 0.0f);
	canDestory_			= false;
	onDamage_			= false;
	currentExp_			= 0.0f;
	currentLevel_		= 1;	//レベルなので初期値は１にする
	currentLevelUpExp_	= 0.0f;
	prevLevelUpExp_		= 0.0f;
	prevLevelUpExp_		= statusData_["need_level_up_exp"].get<float>() * std::pow(currentLevel_ - 1, statusData_["need_level_up_exp_multiplier"].get<float>());
	currentLevelUpExp_	= statusData_["need_level_up_exp"].get<float>() * std::pow(currentLevel_, statusData_["need_level_up_exp_multiplier"].get<float>());
}

void PlayerStatus::rightRotationModelAngleY()
{
	modelAngleY_ -= statusData_["model_rotaion_speed_y"].get<float>();
	if (modelAngleY_ < -DX_PI_F * 2)
	{
		modelAngleY_ = 0.0f;
	}
}

void PlayerStatus::leftRotationModelAngleY()
{
	modelAngleY_ += statusData_["model_rotaion_speed_y"].get<float>();
	if (modelAngleY_ > DX_PI_F * 2)
	{
		modelAngleY_ = 0.0f;
	}
}

void PlayerStatus::decelerate()
{
	speed_ -= statusData_["decelerate_speed"].get<float>();
	speed_ = (std::max)(speed_, 0.0f);
}

void PlayerStatus::addWalkSpeed()
{
	speed_ += statusData_["acceleration"].get<float>();
	speed_ = (std::min)(speed_, statusData_["max_velocity"].get<float>());
}

void PlayerStatus::addDashSpeed()
{
	speed_ += statusData_["acceleration"].get<float>() * statusData_["dash_speed_rate"].get<float>();
	speed_ = (std::min)(speed_, statusData_["max_velocity"].get<float>() * statusData_["dash_speed_rate"].get<float>());
}

void PlayerStatus::movePosition(const VECTOR moveDirection)
{
	//地上にいる間はカメラの向きによるy字句の異同の補正をかけない
	VECTOR tempVector;
	if (position_.y == 0.0f)
	{
		tempVector = VScale(moveDirection, speed_);
	}
	else
	{
		tempVector = VScale(VAdd(moveDirection, facingDirection_), speed_);
	}

	position_ = VAdd(tempVector, position_);

	//ステージ下と高度限界に行かないようにクランプ
	position_.y = std::clamp(position_.y, 0.0f, altitude_limit);
}

void PlayerStatus::raisePosition()
{
	position_.y += statusData_["raise_speed"].get<float>();
}

void PlayerStatus::descentPosition()
{
	position_.y -= world_gravity;
}

void PlayerStatus::setFacingDirection(const VECTOR cameraTargetPosition, const VECTOR cameraPosition)
{
	facingDirection_	= VNorm(VSub(cameraTargetPosition, cameraPosition));
	facingDirection_	= VGet(0.0f, facingDirection_.y, 0.0f);

	//後ろに下がって移動するときは補正をかけない
	if (InputHandler::instance().getDInput()->getProconStickAngle(PROCON::STICK::LEFT_ANGLE) > 0)
	{
		facingDirection_ = zero_vector;
	}
}

void PlayerStatus::setColliderPosition()
{
	//ボトム座標を+Yに補正
	VECTOR adjustBottom = zero_vector;
	adjustBottom.y		= CalculateDistance<float>(capusuleColliderBottomPosition_, capusuleColliderTopPosition_);
	adjustBottom.y		= adjustBottom.y * statusData_["adjust_collider_capusule_bottom"].get<float>();

	capusuleColliderBottomPosition_ = VAdd(position_, adjustBottom);
	capusuleColliderTopPosition_	= MV1GetFramePosition(modelHandle_, modelBoneIndex_["Head"]);
}

void PlayerStatus::pushBack(const VECTOR pushBackVector)
{
	position_ = VAdd(position_, pushBackVector);
}

void PlayerStatus::decreaseHp(const float damage)
{
	hp_ -= damage;
	hp_ = (std::max)(0.0f, hp_);

	PlayerEventHandler::instance().getOnDamageEvent()->occurStartingEvent();
}

void PlayerStatus::destroy()
{
	//hpが0になったら死ぬ
	if (hp_ > 0) return;

	canDestory_ = true;
}

const VECTOR PlayerStatus::getRightBoosterRotationAngle()const noexcept
{
	return GetBoneRotationAngleXZY(modelHandle_, "BackBooster_R");
}

const VECTOR PlayerStatus::getLeftBoosterRotationAngle()const noexcept
{
	return GetBoneRotationAngleXZY(modelHandle_, "BackBooster_L");
}

void PlayerStatus::setOnDamage(const bool isHiting)
{
	onDamage_ = isHiting;
}

void PlayerStatus::addExp(const int value)
{
	currentExp_ += value;
}

void PlayerStatus::levelUp()
{
	if (currentExp_ < currentLevelUpExp_) return;

	++currentLevel_;

	prevLevelUpExp_		= statusData_["need_level_up_exp"].get<float>() * std::pow(currentLevel_ - 1, statusData_["need_level_up_exp_multiplier"].get<float>());
	currentLevelUpExp_	= statusData_["need_level_up_exp"].get<float>() * std::pow(currentLevel_, statusData_["need_level_up_exp_multiplier"].get<float>());

	PlayerEventHandler::instance().getLevelUpEvent()->occurStartingEvent();
}