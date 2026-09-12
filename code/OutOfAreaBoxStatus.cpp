#include"DxLib.h"
#include"MapObjectStatus.h"
#include"OutOfAreaBoxStatus.h"
#include"ColliderEventHander.h"

OutOfAreaBoxStatus::OutOfAreaBoxStatus(const VECTOR initPosition, const float mapModelScale, const VECTOR standardSize)
	:init_position(initPosition)
{
	position_			= VScale(init_position, mapModelScale * adjust_position);
	position_.y			= init_position.y;
	cuboidSizeXYZ_.x	= mapModelScale * standardSize.x;
	cuboidSizeXYZ_.y	= mapModelScale * standardSize.y;
	cuboidSizeXYZ_.z	= mapModelScale * standardSize.z;

	//èâä˙âª
	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
	init();
}

OutOfAreaBoxStatus::~OutOfAreaBoxStatus()
{
	MV1DeleteModel(modelHandle_);
}

void OutOfAreaBoxStatus::init()
{
}

void OutOfAreaBoxStatus::update()
{
	position_ = init_position;

	centerColliderPosition_ = VAdd(position_, VGet(0.0f, cuboidSizeXYZ_.y * 0.5f, 0.0f));
}