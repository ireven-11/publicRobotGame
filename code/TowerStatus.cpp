#include"DxLib.h"
#include"MapObjectStatus.h"
#include"TowerStatus.h"
#include"DestroyObjectEvent.h"
#include"ColliderEventHander.h"

//スタティックメンバーを初期化
int TowerStatus::TowerModel_ = 0;

TowerStatus::TowerStatus(const VECTOR initPosition, const std::function<void()>& hpThreeQuarters, const std::function<void()>& hpOneHalf, const std::function<void()>& hpOneQuarters)
	:init_position(initPosition)
{
	statusData_ = LoadDataJson("statusData/mapObjectData/TowerStatus.json");
	std::string modelPath = statusData_["model_path"].get<std::string>();

	//最初の一回だけ読み込み
	if (TowerModel_ == 0)
	{
		TowerModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(TowerModel_);

	float modelScale = statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	hpThreeQuartersEvent_	= hpThreeQuarters;
	hpOneHalfEvent_			= hpOneHalf;
	hpOneQuartersEvent_		= hpOneQuarters;

	position_				= init_position;
	colliderPosition_		= VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.75f, 0.0f));
	cuboidSizeXYZ_.x		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_x"].get<float>();
	cuboidSizeXYZ_.y		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_y"].get<float>();
	cuboidSizeXYZ_.z		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_z"].get<float>();
	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
	init();
}

TowerStatus::~TowerStatus()
{
	MV1DeleteModel(modelHandle_);
}

void TowerStatus::init()
{
	hp_							= statusData_["max_hp"].get<float>();
	canDestroy_					= false;
	onDamage_					= false;
	isHpThreeQuartersMoment_	= false;
	isHpOneHalfMoment_			= false;
	isHpOneQuartersMoment_		= false;
}

void TowerStatus::update()
{
	position_ = init_position;

	colliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * adjust_Tower_collider_position, 0.0f));

	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
}

void TowerStatus::decreaseHp(const float damage)
{
	const auto maxHp = statusData_["max_hp"].get<float>();

	hp_ -= damage;

	//hp減少時のイベントを呼ぶ
	if (!isHpThreeQuartersMoment_ && hp_ < maxHp * 0.75f)
	{
		isHpThreeQuartersMoment_ = true;
		hpThreeQuartersEvent_();
	}
	else if (!isHpOneHalfMoment_ && hp_ < maxHp * 0.5f)
	{
		isHpOneHalfMoment_ = true;
		hpOneHalfEvent_();
	}
	else if (!isHpOneQuartersMoment_ && hp_ < maxHp * 0.25f)
	{
		isHpOneQuartersMoment_ = true;
		hpOneQuartersEvent_();
	}
}

void TowerStatus::destroy()
{
	if (hp_ > 0) return;

	canDestroy_ = true;

	//壊れた時のイベントを呼ぶ
	ColliderEventHander::instance().getDestroyObjectEvent()->occurStartingEvent();
}

const float	TowerStatus::getAdjustingAttackDistance()const noexcept
{
	//半径がないので一番大きい軸の大きさを調整の値とする
	float biggestSize = cuboidSizeXYZ_.x;
	//大きすぎないようにする
	if (biggestSize > cuboidSizeXYZ_.z * 2.0f)
	{
		biggestSize = cuboidSizeXYZ_.z * 2.0f;
	}

	if (biggestSize < cuboidSizeXYZ_.y)
	{
		biggestSize = cuboidSizeXYZ_.y;

		//大きすぎないようにする
		if (biggestSize > cuboidSizeXYZ_.x * 2.0f)
		{
			biggestSize = cuboidSizeXYZ_.x * 2.0f;
		}
	}
	if (biggestSize < cuboidSizeXYZ_.z)
	{
		biggestSize = cuboidSizeXYZ_.z;

		//大きすぎないようにする
		if (biggestSize > cuboidSizeXYZ_.y * 2.0f)
		{
			biggestSize = cuboidSizeXYZ_.y * 2.0f;
		}
	}

	return biggestSize;
}

void TowerStatus::setOnDamage(const bool isHiting)
{
	onDamage_ = isHiting;
}