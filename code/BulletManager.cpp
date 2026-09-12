#include"DxLib.h"
#include"BulletManager.h"
#include"IBullet.h"

BulletManager::BulletManager()
{
}

BulletManager::~BulletManager()
{
}

void BulletManager::init()
{
	activeBullets_.clear();
}

void BulletManager::update()
{
	for (const auto& bullet : activeBullets_)
	{
		bullet->update();
	}
}

void BulletManager::draw()
{
	for (const auto& bullet : activeBullets_)
	{
		//if (!bullet->getStatus()->getIsUsing()) continue;

		bullet->draw();
	}
}

void BulletManager::addActiveBullet(const std::shared_ptr<IBullet>& bullet)
{
	activeBullets_.emplace_back(bullet);
}