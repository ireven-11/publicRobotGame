#include"DxLib.h"
#include"MapObjectStatus.h"
#include"HousingComplexStatus.h"
#include"ColliderEventHander.h"

//スタティックメンバーを初期化
int HousingComplexStatus::HousingComplexModel_ = 0;

HousingComplexStatus::HousingComplexStatus(const VECTOR initPosition, const float rotationAngleY)
	:init_position(initPosition),
	rotation_angle_y(rotationAngleY)
{
	statusData_				= LoadDataJson("statusData/mapObjectData/housingComplexStatus.json");
	std::string modelPath	= statusData_["model_path"].get<std::string>();

	//最初の一回だけ読み込み
	if (HousingComplexModel_ == 0)
	{
		HousingComplexModel_ = MV1LoadModel(modelPath.c_str());	//dxlibの関数はstringを使えないのでc_str()で変換する
	}

	//スタティックのモデルを再利用して読み込み
	modelHandle_ = MV1DuplicateModel(HousingComplexModel_);

	float modelScale = statusData_["model_scale"].get<float>();
	MV1SetScale(modelHandle_, VGet(modelScale, modelScale, modelScale));

	//モデルをy軸回転
	axis_.rotationY(rotation_angle_y);
	MV1SetRotationXYZ(modelHandle_, VGet(0.0f, rotation_angle_y, 0.0f));

	position_				= init_position;
	cuboidSizeXYZ_.x		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_x"].get<float>();
	cuboidSizeXYZ_.y		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_y"].get<float>();
	cuboidSizeXYZ_.z		= statusData_["model_scale"].get<float>() * statusData_["adjust_cuboid_collider_axis_z"].get<float>();
	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
	init();
}

HousingComplexStatus::~HousingComplexStatus()
{
	MV1DeleteModel(modelHandle_);
}

void HousingComplexStatus::init()
{
}

void HousingComplexStatus::update()
{
	position_ = init_position;

	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
}

const float	HousingComplexStatus::getAdjustingAttackDistance()const noexcept
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