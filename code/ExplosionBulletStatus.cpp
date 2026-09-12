#include"DxLib.h"
#include"BulletStatus.h"
#include"ExplosionBulletStatus.h"
#include"Physics.h"

ExplosionBulletStatus::ExplosionBulletStatus(const float attackPower)
	:BulletStatus(attackPower)
{
	//statusData_ = LoadDataJson("statusData/mapObjectData/targetStatus.json");
	//std::string modelPath = statusData_["model_path"].get<std::string>();
	//modelHandle_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する

	//float modelScale = statusData_["model_scale"].get<float>();
	//MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	init();
}

ExplosionBulletStatus::~ExplosionBulletStatus()
{
}


void ExplosionBulletStatus::init()
{
	isUsing_				= false;
	position_				= VGet(0.0f, 0.0f, 0.0f);
	currentSpeed_			= 0.0f;
	moveVector_				= VGet(0.0f, 0.0f, 0.0f);
	disappearingCounter_	= 0;
	moveDirection_			= VGet(0.0f, 0.0f, 0.0f);
	prevPosition_			= VGet(0.0f, 0.0f, 0.0f);
}

void ExplosionBulletStatus::use(const VECTOR initPosition, const float initSpeed, const VECTOR initDirection)
{
	isUsing_		= true;
	position_		= initPosition;
	prevPosition_	= position_;
	currentSpeed_	= initSpeed;
	moveDirection_	= initDirection;
	moveVector_		= VScale(moveDirection_, currentSpeed_);
}

void ExplosionBulletStatus::setPosition()
{
	currentSpeed_	-= world_air_resistance;
	moveVector_		= VScale(moveDirection_, currentSpeed_);
	prevPosition_	= position_;
	position_		= VAdd(position_, moveVector_);
	position_.y		-= world_gravity;
}

void ExplosionBulletStatus::deathpown()
{
	//時間経過でデスポーン
	++disappearingCounter_;
	if (disappearingCounter_ > disappearing_time)
	{
		init();
	}

	//スピードが0になったらデスポーン
	if (currentSpeed_ <= 0.0f)
	{
		init();
	}
}