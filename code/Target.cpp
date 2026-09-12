#include"DxLibForIreven.h"
#include<memory>
#include"MapObjectStatus.h"
#include"Target.h"
#include"TargetStatus.h"

Target::Target(const VECTOR initPosition)
	:status_(std::make_shared<TargetStatus>(initPosition))
{
	//生成された時点でマップの正しい位置に配置しておく（カメラ演出時にマップオブジェクトのupdateが走らないため）
	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

Target::~Target()
{
	status_ = nullptr;
}

void Target::update()
{
	status_->update();

	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void Target::draw()const
{
	MV1DrawModel(status_->getModelHandle());

	DrawCuboid3D(status_->getCenterPosition(), status_->getSizeXYZ(), status_->getObjectAxis(), GetColor(255, 255, 255));	//test
}