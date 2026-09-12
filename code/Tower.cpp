#include"DxLibForIreven.h"
#include<memory>
#include"MapObjectStatus.h"
#include"Tower.h"
#include"TowerStatus.h"

Tower::Tower(const VECTOR initPosition, const std::function<void()>& hpThreeQuarters, const std::function<void()>& hpOneHalf, const std::function<void()>& hpOneQuarters)
	:status_(std::make_shared<TowerStatus>(initPosition, hpThreeQuarters, hpOneHalf,hpOneQuarters))
{
	//生成された時点でマップの正しい位置に配置しておく（カメラ演出時にマップオブジェクトのupdateが走らないため）
	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

Tower::~Tower()
{
	status_ = nullptr;
}

void Tower::update()
{
	status_->update();

	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void Tower::draw()const
{
	//ライティングを無効化して描画
	SetUseLighting(false);
	SetUseZBufferFlag(false);
	MV1DrawModel(status_->getModelHandle());
	SetUseZBufferFlag(true);
	SetUseLighting(true);

	//DrawCuboid3D(status_->getCenterPosition(), status_->getSizeXYZ(), status_->getObjectAxis(), GetColor(255, 255, 255));	//test
}