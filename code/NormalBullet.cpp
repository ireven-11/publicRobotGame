#include"DxLib.h"
#include<memory>
#include"IBullet.h"
#include"BulletStatus.h"
#include"NormalBullet.h"
#include"NormalBulletStatus.h"

NormalBullet::NormalBullet(const float attackPower, const BulletOwnerType owner)
	:status_(std::make_shared<NormalBulletStatus>(attackPower)),
	bullet_owner(owner)
{
}

NormalBullet::~NormalBullet()
{
	status_ = nullptr;
}

void NormalBullet::update()
{
	status_->setPosition();

	status_->deathpown();

	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void NormalBullet::draw()const
{
	//ƒeƒXƒg
	DrawSphere3D(status_->getPosition(), 0.5f, 32, 32, GetColor(255, 255, 0), true);
}