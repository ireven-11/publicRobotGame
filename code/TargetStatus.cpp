#include"DxLib.h"
#include"MapObjectStatus.h"
#include"TargetStatus.h"
#include"DestroyObjectEvent.h"
#include"ColliderEventHander.h"

//スタティックメンバーを初期化
int TargetStatus::targetModel_ = 0;

TargetStatus::TargetStatus(const VECTOR initposition)
	:init_position(initposition)
{
	statusData_ = LoadDataJson("statusData/mapObjectData/targetStatus.json");
	std::string modelPath = statusData_["model_path"].get<std::string>();

	//最初の一回だけ読み込み
	if (targetModel_ == 0)
	{
		targetModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(targetModel_);
	
	float modelScale = statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	//初期化
	position_				= init_position;
	colliderPosition_		= VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.75f, 0.0f));
	cuboidSizeXYZ_.x		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_x"].get<float>();
	cuboidSizeXYZ_.y		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_y"].get<float>();
	cuboidSizeXYZ_.z		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_z"].get<float>();
	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
	init();
}

TargetStatus::~TargetStatus()
{
	MV1DeleteModel(modelHandle_);
}

void TargetStatus::init()
{
	hp_				= statusData_["max_hp"].get<float>();
	canDestroy_		= false;
	onDamage_			= false;
}

void TargetStatus::update()
{
	position_ = init_position;

	colliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * adjust_target_collider_position, 0.0f));

	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
}

void TargetStatus::decreaseHp(const float damage)
{
	hp_ -= damage;
}

void TargetStatus::destroy()
{
	if (hp_ > 0) return;

	canDestroy_ = true;

	//壊れた時のイベントを呼ぶ
	ColliderEventHander::instance().getDestroyObjectEvent()->occurStartingEvent();
}

const float	TargetStatus::getAdjustingAttackDistance()const noexcept
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

void TargetStatus::setOnDamage(const bool isHiting)
{
	onDamage_ = isHiting;
}