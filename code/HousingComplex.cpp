#include"DxLibForIreven.h"
#include<memory>
#include"MapObjectStatus.h"
#include"HousingComplex.h"
#include"HousingComplexStatus.h"

HousingComplex::HousingComplex(const VECTOR initPosition, const float rotationAngleY)
	:status_(std::make_shared<HousingComplexStatus>(initPosition, rotationAngleY))
{
	//生成された時点でマップの正しい位置に配置しておく（カメラ演出時にマップオブジェクトのupdateが走らないため）
	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

HousingComplex::~HousingComplex()
{
	status_ = nullptr;
}

void HousingComplex::update()
{
	status_->update();

	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void HousingComplex::draw()const
{
	MV1DrawModel(status_->getModelHandle());

	//DrawCuboid3D(status_->getCenterPosition(), status_->getSizeXYZ(), status_->getObjectAxis(), GetColor(255, 255, 255));	//test
}