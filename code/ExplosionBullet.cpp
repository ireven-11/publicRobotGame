#include"DxLib.h"
#include<memory>
#include"IBullet.h"
#include"BulletStatus.h"
#include"Explosion.h"
#include"ExplosionBullet.h"
#include"ExplosionBulletStatus.h"

ExplosionBullet::ExplosionBullet(const float attackPower, const float explosionRaidus, const BulletOwnerType owner)
	:status_(std::make_shared<ExplosionBulletStatus>(attackPower)),
	explosion_(std::make_shared<Explosion>(explosionRaidus, attackPower)),
	bullet_owner(owner)
{
}

ExplosionBullet::~ExplosionBullet()
{
	status_		= nullptr;
	explosion_	= nullptr;
}

void ExplosionBullet::update()
{
	status_->setPosition();

	status_->deathpown();

	MV1SetPosition(status_->getModelHandle(), status_->getPosition());
}

void ExplosionBullet::draw()const
{
	//ƒeƒXƒg
	DrawSphere3D(status_->getPosition(), 0.5f, 32, GetColor(255, 255, 0), GetColor(255, 255, 0), true);
}