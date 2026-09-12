#include"DxLib.h"
#include"ExplosionEffect.h"
#include"ExplosionManager.h"
#include"Explosion.h"

ExplosionManager::ExplosionManager(const std::shared_ptr<ExplosionEffect>& explosionEffect, const std::function<void()>& soundPlayer):
	effect_(explosionEffect),
	playExplosionSound_(soundPlayer)
{
}

ExplosionManager::~ExplosionManager()
{
	effect_ = nullptr;
}

void ExplosionManager::init()
{
	activeExplosion_.clear();
}

void ExplosionManager::update()
{
	for (auto& explosion : activeExplosion_)
	{
		explosion->update();
	}
}

void ExplosionManager::addActiveExplosion(const std::shared_ptr<Explosion>& explosion)
{
	activeExplosion_.emplace_back(explosion);

	//爆発範囲に合わせてエフェクトを再生
	effect_->setScale(explosion->getRadius());
	effect_->play();
	effect_->setPosition(explosion->getPosition());

	//スケールに使ってる値が共通なので他の爆発エフェクトに影響が出ないようにスケールをデフォルトに戻す
	effect_->setDefualtScale();

	//爆発音を鳴らす
	playExplosionSound_();
}